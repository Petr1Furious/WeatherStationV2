#pragma once

#include "Display.h"
#include "Sensor.h"
#include "WeatherForecaster.h"

class WeatherStation {
  void clearGraphData();

  bool shouldGetCurrentWeather();

  bool tickSensor();

public:
  WeatherStation() = default;

  void init();

  void tick();

private:
  Sensor sensor;
  Display display;

  WeatherData weather_data;
  WeatherForecaster weather_forecaster;

  float altitude_offset;

  unsigned long last_button_press = 0;
  unsigned long last_forecast_time = 0;
  unsigned long last_update_time = -GRAPH_UPDATE_TIME_MS;

  uint8_t mode;
  uint8_t old_mode = -1;

  bool on = true;
  bool old_on = false;
};
