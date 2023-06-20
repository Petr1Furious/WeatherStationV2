#include "WeatherForecaster.h"

#include "Settings.h"

float WeatherForecaster::forecast(const TrackedData& tracked_data) {
  uint32_t n = tracked_data.count - 1;

  uint32_t sumx = (n * (n + 1) / 2) * TRACKING_STEP_MINUTES;
  uint32_t sumx2 = (n * (n + 1) * (2 * n + 1) / 6) * (uint32_t)TRACKING_STEP_MINUTES * TRACKING_STEP_MINUTES;
  float sumy = 0;
  float sumxy = 0;
  
  for (size_t i = 0; i < tracked_data.count; i++) {
    float16 y;
    y.setBinary(tracked_data.pressure[i]);
    float yf = 100 * y.toDouble();
    sumy += yf;
    sumxy += (TRACKING_STEP_MINUTES * i) * yf;
  }

  float denominator = tracked_data.count * sumx2 - sumx * sumx;
  if (denominator == 0) {
    return 0;
  }
  return 60 * (tracked_data.count * sumxy - sumx * sumy) / denominator;
}
