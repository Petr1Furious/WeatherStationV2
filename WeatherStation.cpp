#include "PinChangeInterrupt.h"

#include "Settings.h"
#include "WeatherStation.h"

volatile uint8_t mode = 0;
volatile unsigned long last_pressed = 0;
volatile bool held = false;

void onButton(void) {
  if (getPinChangeInterruptTrigger(digitalPinToPCINT(BUTTON_PIN)) == FALLING) {
    last_pressed = millis();
  } else {
    if (millis() - last_pressed >= HOLD_TIMEOUT) {
      held = true;
    } else {
      mode++;
      if (mode == MODES_COUNT) {
        mode = 0;
      }
    }
  }
}

void WeatherStation::init() {
  Serial.begin(9600);

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  sensor.init();

  display.init();

  attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(BUTTON_PIN), onButton, CHANGE);
}

void WeatherStation::tick() {
  bool get_current_weather = (mode != last_mode);
  last_mode = mode;

  if (mode == 0) {
    if (millis() - last_update_time >= REGULAR_MODE_UPDATE_TIME_MS) {
      last_update_time = millis();
      get_current_weather = true;
    }
  }
  if (mode == 1) {
    last_update_time = millis();
    get_current_weather = true;
  }
  if (2 <= mode && mode <= 4) {
    if (millis() - last_update_time >= GRAPH_UPDATE_TIME_MS) {
      last_update_time = millis();
      get_current_weather = true;
    }
  }
  
  DisplayData display_data {
    .mode = mode,
    .weather_data = sensor.tick(get_current_weather)
  };
  display.tick(display_data);
}
