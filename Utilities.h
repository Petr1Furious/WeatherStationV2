#pragma once

struct TrackedData {
  size_t count;
  uint16_t* temperature;
  uint16_t* pressure;
  uint16_t* humidity;
};

struct WeatherData {
  float temperature;
  float pressure;
  float altitude;
  float humidity;
  float pressure_change;
  bool just_updated;
  TrackedData tracked_data;
};
