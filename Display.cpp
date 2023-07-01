#include <Arduino.h>
#include <stdio.h>
#include "float16.h"

#include "EEPROMStructures.h"
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

void Display::drawMode(const DisplayData& display_data) {
  u8g.drawStr(123, 9, String(display_data.mode + 1).c_str());
  u8g.drawVLine(121, 0, 11);
  u8g.drawHLine(121, 11, 7);
}

float toMm(float hPa) {
  return hPa * .75006157584566;
}

void Display::draw(const DisplayData& display_data) {
  const WeatherData& weather_data = display_data.weather_data;
  if (display_data.mode == 0 || display_data.mode == 1) {
    u8g.firstPage();
    do {
      u8g.setFont(u8g_font_6x13);
      u8g_uint_t pos = 9;
      u8g_uint_t temp_len = u8g.drawStr(0, pos, ("Temp: " + String(weather_data.temperature)).c_str());
      u8g.drawCircle(temp_len + 3, 2, 2);

      u8g.drawStr(0, pos += 12, ("Hum: " + String(weather_data.humidity) + "%").c_str());

      u8g.drawStr(0, pos += 12, ("Pres: " + String(toMm(weather_data.pressure)) + " mm").c_str());

      float change = weather_data.pressure_change;
      u8g_uint_t change_len = u8g.drawStr(0, pos + 12, "Change: ");
      u8g.drawStr(change_len, pos += 12, (String(change) + " Pa/h").c_str());

      char* change_string;
      if (display_data.mode == 0) {
        if (change < -150) {
          change_string = ">> storm";
        } else if (change < -75) {
          change_string = "> rain";
        } else if (change <= 75) {
          change_string = "= same";
        } else if (change <= 150) {
          change_string = "< clear";
        } else {
          change_string = "<< clear";
        }
        u8g.drawStr(change_len, pos += 12, change_string);
      } else {
        u8g.drawStr(0, pos += 12, ("Alt: " + String(weather_data.altitude) + "/" +
                                   String(weather_data.altitude - display_data.altitude_offset) + " m").c_str());
      }

      drawMode(display_data);
    } while (u8g.nextPage());
  }

  if (2 <= display_data.mode && display_data.mode <= 4) {
    u8g.firstPage();
    do {
      u8g.setFont(u8g_font_6x13);

      const TrackedData& tracked_data = display_data.weather_data.tracked_data;

      const EEPROMQueue<uint16_t>& tracked = (display_data.mode == 2 ? tracked_data.pressure :
                                              display_data.mode == 3 ? tracked_data.humidity :
                                              tracked_data.temperature);

      float mn = 1e6, mx = -1e6;

      for (size_t i = 0; i < tracked_data.size(); i++) {
        float16 f;
        f.setBinary(tracked[i]);
        mn = min(mn, f.toDouble());
        mx = max(mx, f.toDouble());
      }

      const size_t graph_height_offset = 15;
      const size_t graph_hline_length = 15;
      size_t h_offset = u8g.getWidth() - MAX_TRACKING_COUNT;
      for (size_t i = 0; i < tracked_data.size(); i++) {
        float16 f;
        f.setBinary(tracked[i]);
        u8g.drawPixel(h_offset + i, map(f.toDouble() * 1000, mn * 1000, mx * 1000, u8g.getHeight() - 1, graph_height_offset));
      }

      for (int i = 1;; i++) {
        int hour_pos = ((int)u8g.getWidth() - (i * 60) / (int)TRACKING_STEP_MINUTES);
        if (hour_pos < h_offset) {
          break;
        }
        u8g.drawPixel(hour_pos, u8g.getHeight() - 1);
        u8g.drawPixel(hour_pos, u8g.getHeight() - 2);
      }

      u8g.drawStr(0, u8g.getHeight(), "L");
      u8g.drawHLine(h_offset - 22, u8g.getHeight() - 1, graph_hline_length);
      u8g.drawStr(0, graph_height_offset + 9, "H");
      u8g.drawHLine(h_offset - 22, graph_height_offset, graph_hline_length);

      String type;
      if (display_data.mode == 2) {
        type = "Pres";
        mn = toMm(mn + PRESSURE_DELTA);
        mx = toMm(mx + PRESSURE_DELTA);
      }
      if (display_data.mode == 3) {
        type = "Hum";
      }
      if (display_data.mode == 4) {
        type = "Temp";
      }
      u8g.drawStr(0, 9, (type + " L=" + String(mn, 1) + " H=" + String(mx, 1)).c_str());

      drawMode(display_data);
    } while (u8g.nextPage());
  }
}

void Display::clearDisplay() {
  u8g.firstPage();
  while (u8g.nextPage());
}
