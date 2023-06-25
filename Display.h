#pragma once

#include "U8glib.h"

#include "WeatherData.h"
#include "Settings.h"

struct DisplayData {
  const WeatherData& weather_data;
  uint8_t mode;
  float altitude_offset;
};

class Display {
public:
  Display() = default;

  void init();

  void drawMode(const DisplayData& display_data);

  void draw(const DisplayData& display_data);

  void clearDisplay();

private:
  U8GLIB_SSD1306_128X64 u8g;
};
