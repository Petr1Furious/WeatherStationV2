#pragma once

#include "Display.h"
#include "Sensor.h"

class WeatherStation {
public:
  WeatherStation() = default;

  void init();

  void tick();

private:
  Sensor sensor;

  Display display;

  unsigned long last_update_time = -GRAPH_UPDATE_TIME_MS;

  uint8_t last_mode = 0;
};
