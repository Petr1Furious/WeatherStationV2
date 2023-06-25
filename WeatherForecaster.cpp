#include "WeatherForecaster.h"

#include "Settings.h"

float WeatherForecaster::forecast(const TrackedData& tracked_data) {
  uint32_t n = min(tracked_data.size(), PREDICT_TRACKING_COUNT);
  size_t offset = tracked_data.size() - n;

  uint32_t sumx = ((n - 1) * n / 2) * TRACKING_STEP_MINUTES;
  uint32_t sumx2 = ((n - 1) * n * (2 * n - 1) / 6) * (uint32_t)TRACKING_STEP_MINUTES * TRACKING_STEP_MINUTES;
  float sumy = 0;
  float sumxy = 0;
  
  for (size_t i = 0; i < n; i++) {
    float16 y;
    y.setBinary(tracked_data.pressure[offset + i]);
    float yf = 100 * y.toDouble();
    sumy += yf;
    sumxy += (TRACKING_STEP_MINUTES * i) * yf;
  }

  float denominator = n * sumx2 - sumx * sumx;
  if (denominator == 0) {
    return 0;
  }
  return 60 * (n * sumxy - sumx * sumy) / denominator;
}
