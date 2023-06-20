#pragma once

#include "U8glib.h"

#include "Utilities.h"
#include "Settings.h"

struct DisplayData {
  uint8_t mode;
  WeatherData weather_data;
};

class Display {
public:
  Display() = default;

  void init();

  void tick(const DisplayData& display_data);

private:
  U8GLIB_SSD1306_128X64 u8g;
};
