#pragma once

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#include "Settings.h"
#include "Utilities.h"
#include "WeatherForecaster.h"

static const size_t max_tracking_count = TRACKING_TIME_MINUTES / TRACKING_STEP_MINUTES;

class Sensor {
public:
  Sensor() = default;

  void init();

  float getTemperature();

  float getPressure();

  float getAltitude();

  float getHumidity();

  WeatherData tick(bool get_current_weather);

private:
  Adafruit_BME280 bme;

  WeatherData weather_data;

  uint16_t tracked_temperature[max_tracking_count];

  uint16_t tracked_pressure[max_tracking_count];

  uint16_t tracked_humidity[max_tracking_count];

  WeatherForecaster weather_forecaster;

  unsigned long last_forecast_time = -TRACKING_STEP_MINUTES * 60000;
};
