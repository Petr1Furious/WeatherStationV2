#pragma once

// Weather Station (general) settings
constexpr uint8_t BUTTON_PIN = 12;
constexpr unsigned long HOLD_TIMEOUT = 500;
constexpr uint8_t MODES_COUNT = 5;

// Display settings
constexpr uint8_t SCREEN_WIDTH = 128;
constexpr uint8_t SCREEN_HEIGHT = 64;
constexpr int8_t OLED_RESET = -1;
constexpr uint8_t SCREEN_ADDRESS = 0x3C;
constexpr unsigned long REGULAR_MODE_UPDATE_TIME_MS = 30000;
constexpr unsigned long GRAPH_UPDATE_TIME_MS = 240000;

// Sensor settings
constexpr size_t TRACKING_TIME_MINUTES = 480;
constexpr size_t TRACKING_STEP_MINUTES = 5;
constexpr float PRESSURE_DELTA = 1013.25;
