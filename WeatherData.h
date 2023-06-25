#pragma once

#include "EEPROMStructures.h"
#include "Settings.h"

struct TrackedData {
  EEPROMQueue<uint16_t> temperature = EEPROMQueue<uint16_t>(MAX_TRACKING_COUNT, EEPROM_TRACKED_TEMPERATURE_POS, 65535);
  EEPROMQueue<uint16_t> pressure = EEPROMQueue<uint16_t>(MAX_TRACKING_COUNT, EEPROM_TRACKED_PRESSURE_POS, 65535);
  EEPROMQueue<uint16_t> humidity = EEPROMQueue<uint16_t>(MAX_TRACKING_COUNT, EEPROM_TRACKED_HUMIDITY_POS, 65535);
  
  size_t size() const {
    return temperature.size();
  }
};

struct WeatherData {
  float temperature;
  float pressure;
  float altitude;
  float humidity;
  float pressure_change;
  TrackedData tracked_data;
};
