#include <Arduino.h>
#include "float16.h"

#include "Display.h"
#include "Settings.h"

void Display::init() {
  u8g = U8GLIB_SSD1306_128X64(U8G_I2C_OPT_NONE | U8G_I2C_OPT_DEV_0);

  if (u8g.getMode() == U8G_MODE_R3G3B2) {
    u8g.setColorIndex(255);
  } else if (u8g.getMode() == U8G_MODE_GRAY2BIT) {
    u8g.setColorIndex(3);
  } else if (u8g.getMode() == U8G_MODE_BW) {
    u8g.setColorIndex(1);
  } else if (u8g.getMode() == U8G_MODE_HICOLOR) {
    u8g.setHiColorByRGB(255, 255, 255);
  }

  pinMode(8, OUTPUT);
}

void Display::tick(const DisplayData& display_data) {
  if (display_data.weather_data.just_updated) {
    const WeatherData& weather_data = display_data.weather_data;
    if (display_data.mode == 0 || display_data.mode == 1) {
      u8g.firstPage();
      do {
        u8g.setFont(u8g_font_6x13);
        int8_t pos = 10;
        u8g.drawStr(0, pos, ("Temp: " + String(weather_data.temperature)).c_str());
        u8g.drawCircle(69, 2, 2);

        u8g.drawStr(0, pos += 12, ("Pres: " + String(weather_data.pressure) + " hPa").c_str());

        if (display_data.mode == 1) {
          u8g.drawStr(0, pos += 12, ("Alt: " + String(weather_data.altitude) + " m").c_str());
        }

        u8g.drawStr(0, pos += 12, ("Hum: " + String(weather_data.humidity) + "%").c_str());

        u8g.drawStr(0, pos += 12, ("Change: " + String(weather_data.pressure_change) + " Pa/h").c_str());

        u8g.drawStr(123, 10, String(display_data.mode).c_str());
      } while (u8g.nextPage());
    }
    if (2 <= display_data.mode && display_data.mode <= 4) {
      u8g.firstPage();
      do {
        u8g.setFont(u8g_font_6x13);

        uint16_t* tracked;

        if (display_data.mode == 2) {
          u8g.drawStr(0, 10, "Pressure");
          tracked = display_data.weather_data.tracked_data.pressure;
        }
        if (display_data.mode == 3) {
          u8g.drawStr(0, 10, "Humidity");
          tracked = display_data.weather_data.tracked_data.humidity;
        }
        if (display_data.mode == 4) {
          u8g.drawStr(0, 10, "Temperature");
          tracked = display_data.weather_data.tracked_data.temperature;
        }

        size_t count = display_data.weather_data.tracked_data.count;
        float mn = 1e6, mx = -1e6;

        for (size_t i = 0; i < count; i++) {
          float16 f;
          f.setBinary(tracked[i]);
          mn = min(mn, f.toDouble());
          mx = max(mx, f.toDouble());
        }

        for (size_t i = 0; i < count; i++) {
          float16 f;
          f.setBinary(tracked[i]);
          u8g.drawPixel(i, map(f.toDouble() * 1000, mn * 1000, mx * 1000, u8g.getHeight() - 1, 12));
        }

        u8g.drawStr(123, 10, String(display_data.mode).c_str());
      } while (u8g.nextPage());
    }
  }
}
