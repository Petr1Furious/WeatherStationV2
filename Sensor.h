#pragma once

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#include "EEPROMStructures.h"
#include "Settings.h"

class Sensor {
public:
  Sensor() = default;

  void init();

  float getTemperature();

  float getPressure();

  float getAltitude();

  float getHumidity();

private:
  Adafruit_BME280 bme;
};
