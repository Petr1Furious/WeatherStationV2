#include "Sensor.h"

const float SEALEVELPRESSURE_HPA = 1013.25;
const int SENSOR_ADDRESS = 0x76;
const int MEASUREMENT_COUNT = 10;

void Sensor::init() {
  if (!bme.begin(SENSOR_ADDRESS)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (true)
      ;
  }
  weather_data.tracked_data.temperature = tracked_temperature;
  weather_data.tracked_data.pressure = tracked_pressure;
  weather_data.tracked_data.humidity = tracked_humidity;
}

float Sensor::getTemperature() {
  float sum = 0;
  for (int i = 0; i < MEASUREMENT_COUNT; i++) {
    sum += bme.readTemperature();
  }
  return sum / MEASUREMENT_COUNT;
}

float Sensor::getPressure() {
  float sum = 0;
  for (int i = 0; i < MEASUREMENT_COUNT; i++) {
    sum += bme.readPressure() / 100.0f;
  }
  return sum / MEASUREMENT_COUNT;
}

float Sensor::getAltitude() {
  float sum = 0;
  for (int i = 0; i < MEASUREMENT_COUNT; i++) {
    sum += bme.readAltitude(SEALEVELPRESSURE_HPA);
  }
  return sum / MEASUREMENT_COUNT;
}

float Sensor::getHumidity() {
  float sum = 0;
  for (int i = 0; i < MEASUREMENT_COUNT; i++) {
    sum += bme.readHumidity();
  }
  return sum / MEASUREMENT_COUNT;
}

WeatherData Sensor::tick(bool get_current_weather) {
  weather_data.just_updated = false;

  bool track_now = false;
  if (millis() - last_forecast_time >= TRACKING_STEP_MINUTES * 60000) {
    last_forecast_time = millis();
    track_now = true;
  }

  if (get_current_weather || track_now) {
    weather_data.just_updated = true;
    weather_data.temperature = getTemperature();
    weather_data.pressure = getPressure();
    weather_data.altitude = getAltitude();
    weather_data.humidity = getHumidity();
  }

  if (track_now) {
    last_forecast_time = millis();

    weather_data.just_updated = true;

    size_t& tracking_count = weather_data.tracked_data.count;

    if (tracking_count == max_tracking_count) {
      for (int i = 1; i < max_tracking_count; i++) {
        tracked_temperature[i - 1] = tracked_temperature[i];
        tracked_pressure[i - 1] = tracked_pressure[i];
        tracked_humidity[i - 1] = tracked_humidity[i];
      }
      tracking_count--;
    }
    tracked_temperature[tracking_count] = float16(weather_data.temperature).getBinary();
    tracked_pressure[tracking_count] = float16(weather_data.pressure - PRESSURE_DELTA).getBinary();
    tracked_humidity[tracking_count] = float16(weather_data.humidity).getBinary();
    tracking_count++;

    weather_data.pressure_change = weather_forecaster.forecast(weather_data.tracked_data);
  }

  return weather_data;
}
