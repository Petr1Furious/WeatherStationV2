#include <EEPROM.h>
#include "PinChangeInterrupt.h"

#include "Settings.h"
#include "WeatherStation.h"

enum {
  BUTTON_RELEASED = 0,
  BUTTON_PRESS = 1,
  BUTTON_HELD = 2,
  BUTTON_HELD_LONG = 3
};

volatile unsigned long last_released = 0;
volatile unsigned long last_pressed = 0;
volatile uint8_t press_type = 0;

void onButton(void) {
  if (getPinChangeInterruptTrigger(digitalPinToPCINT(BUTTON_PIN)) == FALLING) {
    last_pressed = millis();
  } else {
    if (millis() - last_released >= MIN_RELEASE_TIME) {
      if (millis() - last_pressed >= LONG_HOLD_TIMEOUT) {
        press_type = BUTTON_HELD_LONG;
      } else if (millis() - last_pressed >= HOLD_TIMEOUT) {
        press_type = BUTTON_HELD;
      } else {
        press_type = BUTTON_PRESS;
      }
    }
    last_released = millis();
  }
}

void WeatherStation::clearGraphData() {
  TrackedData& tracked_data = weather_data.tracked_data;
  tracked_data.temperature.clearMemory();
  tracked_data.pressure.clearMemory();
  tracked_data.humidity.clearMemory();
  last_forecast_time = millis() - TRACKING_STEP_MINUTES * 60000;
}

void WeatherStation::init() {
  Serial.begin(9600);
  while (!Serial);
  
  Serial.println("Weather Station has started.");

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  sensor.init();
  display.init();

  EEPROM.get(EEPROM_MODE_POS, mode);
  mode = mode % MODES_COUNT;
  EEPROM.get(EEPROM_ALTITUDE_OFFSET_POS, altitude_offset);
  if (isnan(altitude_offset) || isinf(altitude_offset)) {
    altitude_offset = 0;
  }

  attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(BUTTON_PIN), onButton, CHANGE);

  if (weather_data.tracked_data.size() == 0) {
    last_forecast_time = -TRACKING_STEP_MINUTES * 60000;
  }
  else {
    weather_data.pressure_change = weather_forecaster.forecast(weather_data.tracked_data);
  }
}

bool WeatherStation::shouldGetCurrentWeather() {
  if (!on) {
    return false;
  }
  if (on == true && old_on == false) {
    return true;
  }

  if (mode != old_mode) {
    last_update_time = millis();
    return true;
  }

  if (mode == 0) {
    if (millis() - last_update_time >= REGULAR_MODE_UPDATE_TIME_MS) {
      last_update_time = millis();
      return true;
    }
  }
  if (mode == 1) {
    last_update_time = millis();
    return true;
  }
  if (2 <= mode && mode <= 4) {
    if (millis() - last_update_time >= GRAPH_UPDATE_TIME_MS) {
      last_update_time = millis();
      return true;
    }
  }
  return false;
}

bool WeatherStation::tickSensor() {
  bool was_updated = false;

  bool track_now = false;
  if (millis() - last_forecast_time >= TRACKING_STEP_MINUTES * 60000) {
    last_forecast_time = millis();
    track_now = true;
  }

  if (shouldGetCurrentWeather() || track_now) {
    was_updated = true;
    weather_data.temperature = sensor.getTemperature();
    weather_data.pressure = sensor.getPressure();
    weather_data.altitude = sensor.getAltitude();
    weather_data.humidity = sensor.getHumidity();
  }

  if (track_now) {
    last_forecast_time = millis();

    was_updated = true;
    TrackedData& tracked_data = weather_data.tracked_data;
    if (tracked_data.size() == MAX_TRACKING_COUNT) {
      tracked_data.temperature.pop();
      tracked_data.pressure.pop();
      tracked_data.humidity.pop();
    }
    tracked_data.temperature.push(float16(weather_data.temperature).getBinary());
    tracked_data.pressure.push(float16(weather_data.pressure - PRESSURE_DELTA).getBinary());
    tracked_data.humidity.push(float16(weather_data.humidity).getBinary());

    weather_data.pressure_change = weather_forecaster.forecast(weather_data.tracked_data);
  }

  return was_updated;
}

void WeatherStation::tick() {
  on = millis() - last_button_press < SLEEP_TIMEOUT_MS;
  if (press_type != BUTTON_RELEASED) {
    last_button_press = millis();
    if (!on) {
      on = true;
      press_type = BUTTON_RELEASED;
    }
  }

  if (!on && old_on) {
    display.clearDisplay();
  }

  if (on) {
    if (press_type == BUTTON_PRESS) {
      mode++;
      if (mode == MODES_COUNT) {
        mode = 0;
      }
      EEPROM.put(EEPROM_MODE_POS, mode);
    }
    if (press_type == BUTTON_HELD) {
      if (mode == 1) {
        altitude_offset = weather_data.altitude;
        EEPROM.put(EEPROM_ALTITUDE_OFFSET_POS, altitude_offset);
      }
    }
    if (press_type == BUTTON_HELD_LONG) {
      if (2 <= mode && mode <= 4) {
        clearGraphData();
      }
    }
    press_type = BUTTON_RELEASED;

    bool shouldDraw = tickSensor();
    if (shouldDraw) {
      DisplayData display_data {
        .weather_data = weather_data,
        .mode = mode,
        .altitude_offset = altitude_offset
      };

      display.draw(display_data);
    }
  } else {
    tickSensor();
  }

  old_on = on;
  old_mode = mode;
}
