#pragma once

#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <SPI.h>
#include <Wire.h>
#include "float16.h"

#include "Utilities.h"

class WeatherForecaster {
public:
  WeatherForecaster() = default;

  float forecast(const TrackedData& tracked_data);

private:
  Adafruit_BME280* bme;
};
