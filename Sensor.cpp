#include "Sensor.h"

const float SEALEVELPRESSURE_HPA = 1013.25;
const int SENSOR_ADDRESS = 0x76;
const int MEASUREMENT_COUNT = 10;

void Sensor::init() {
  if (!bme.begin(SENSOR_ADDRESS)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (true)
      ;
  }
}

float Sensor::getTemperature() {
  float sum = 0;
  for (int i = 0; i < MEASUREMENT_COUNT; i++) {
    sum += bme.readTemperature();
  }
  return sum / MEASUREMENT_COUNT;
}

float Sensor::getPressure() {
  float sum = 0;
  for (int i = 0; i < MEASUREMENT_COUNT; i++) {
    sum += bme.readPressure() / 100.0f;
  }
  return sum / MEASUREMENT_COUNT;
}

float Sensor::getAltitude() {
  float sum = 0;
  for (int i = 0; i < MEASUREMENT_COUNT; i++) {
    sum += bme.readAltitude(SEALEVELPRESSURE_HPA);
  }
  return sum / MEASUREMENT_COUNT;
}

float Sensor::getHumidity() {
  float sum = 0;
  for (int i = 0; i < MEASUREMENT_COUNT; i++) {
    sum += bme.readHumidity();
  }
  return sum / MEASUREMENT_COUNT;
}
