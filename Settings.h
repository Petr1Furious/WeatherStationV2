#pragma once

// Weather Station (general) settings
constexpr uint8_t BUTTON_PIN = 12;
constexpr uint8_t MODES_COUNT = 5;
constexpr unsigned long HOLD_TIMEOUT = 500;
constexpr unsigned long LONG_HOLD_TIMEOUT = 10000;
constexpr unsigned long MIN_RELEASE_TIME = 200;

// Display settings
constexpr uint8_t SCREEN_WIDTH = 128;
constexpr uint8_t SCREEN_HEIGHT = 64;
constexpr int8_t OLED_RESET = -1;
constexpr uint8_t SCREEN_ADDRESS = 0x3C;
constexpr unsigned long REGULAR_MODE_UPDATE_TIME_MS = 30000;
constexpr unsigned long GRAPH_UPDATE_TIME_MS = 240000;
constexpr unsigned long SLEEP_TIMEOUT_MS = 30000;

// Sensor settings
constexpr size_t TRACKING_TIME_MINUTES = 480;
constexpr size_t TRACKING_STEP_MINUTES = 5;
static const size_t MAX_TRACKING_COUNT = TRACKING_TIME_MINUTES / TRACKING_STEP_MINUTES;
constexpr float PRESSURE_DELTA = 1013.25;
constexpr size_t PREDICT_TIME_MINUTES = 180;
static const size_t PREDICT_TRACKING_COUNT = PREDICT_TIME_MINUTES / TRACKING_STEP_MINUTES;

// EEPROM settings
constexpr size_t EEPROM_MODE_POS = 0;
constexpr size_t EEPROM_TRACKED_TEMPERATURE_POS = 2;
constexpr size_t EEPROM_TRACKED_PRESSURE_POS =
  EEPROM_TRACKED_TEMPERATURE_POS + (TRACKING_TIME_MINUTES / TRACKING_STEP_MINUTES + 1) * sizeof(uint16_t);
constexpr size_t EEPROM_TRACKED_HUMIDITY_POS =
  EEPROM_TRACKED_PRESSURE_POS + (TRACKING_TIME_MINUTES / TRACKING_STEP_MINUTES + 1) * sizeof(uint16_t);
constexpr size_t EEPROM_ALTITUDE_OFFSET_POS =
  EEPROM_TRACKED_HUMIDITY_POS + (TRACKING_TIME_MINUTES / TRACKING_STEP_MINUTES + 1) * sizeof(uint16_t);
