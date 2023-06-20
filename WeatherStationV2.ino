#include "WeatherStation.h"

WeatherStation weather_station;

void setup() {
  weather_station.init();
}

void loop() {
  weather_station.tick();
}
