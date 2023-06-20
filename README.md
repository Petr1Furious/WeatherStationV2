# WeatherStationV2

A complete overhaul of the first weather station project.

## Device and files

Requires a device, which consists of Arduino Nano, I2C BME280 sensor, I2C SSD1306 128x64 monochrome OLED screen and a button. All the components should be connected in the following scheme:

<img width="642" alt="image" src="https://github.com/Petr1Furious/WeatherStationV2/assets/35459417/2bd86146-7950-4084-8952-760f44273726">

Requires Adafruit_BME280, U8glib, PinChangeInterrupt and float16 libraries.

The sketch has to be loaded into the microcontroller via Arduino IDE.

## Usage

To display correct weather prediction and graphs, the weather station should be always powered on.

Weather can be predicted with the current pressure change. If the value is lower than -50 Pa/hr, then the lower the value, the greater the probability of overcast/rain/storm with higher humidity in the next couple of hours. If the value is higher than 50 Pa/hr, then the higher the value, the greater the probability of clear sunny weather with lower humidity.

The weather station has 5 modes, which can be switched with the button press:
- Mode 0. Displays current pressure change, temperature, humidity and pressure; updates once every 30 seconds.
- Mode 1. Displays current pressure change, temperature, humidity, pressure and height relatively to the sea level; updates multiple times a second.
- Mode 2. Displays pressure graph during the last 8 hours; updates once every 5 minutes.
- Mode 3. Displays humidity graph during the last 8 hours; updates once every 5 minutes.
- Mode 4. Displays temperature graph during the last 8 hours; updates once every 5 minutes.

## Implementation

Some fancy features I had to implement to make this work as I wanted it to:
- Due to the fact that most display libraries use internal buffer, they take more than half of the available RAM (2 KB). Bufferless libraries are very slow (simple text is rendered in about 220 ms), and a button press during this 220 ms may be lost, so the device uses hardware interrupts on the button to handle button press even during frame rendering.
- Storing 3 arrays of pressure, humidity and temperature with the length of 96 floats (8 hours / 5 minutes) would take 1152 bytes, which is too much. That is why the data is stored in float16 (5 bit exponent, 10 bit mantissa + sign bit).
- To calculate the current pressure change approximation (and predict weather in the future with it) [linear regression](https://en.wikipedia.org/wiki/Linear_regression) is used.

## TODO

- Save all the data into EEPROM (non-volatile memory) in case the power is lost to load it on startup
- Fancier graphs
- Show the explicit weather prediction
- Turn off the screen to prevent OLED burn in

## Examples

Mode 0:
![20230620_225921](https://github.com/Petr1Furious/WeatherStationV2/assets/35459417/fc3b08e0-584b-4153-977c-e3accd430035)

Mode 1:
![20230620_225826](https://github.com/Petr1Furious/WeatherStationV2/assets/35459417/c63a9770-eaee-40a1-9a6e-fdb3a0dca8da)

Mode 2:
![20230620_225835](https://github.com/Petr1Furious/WeatherStationV2/assets/35459417/19fba092-4162-4cbb-84c1-6cbf5b76e635)

Mode 3:
![20230620_225843](https://github.com/Petr1Furious/WeatherStationV2/assets/35459417/34828096-ae7e-405a-8a24-7e1a135969e0)

Mode 4:
![20230620_225851](https://github.com/Petr1Furious/WeatherStationV2/assets/35459417/80850c87-bfaf-49f8-8c4a-bf6011d7cfc2)

[Video example](https://drive.google.com/file/d/1KHey0TmH1Hum_z-NL_RsqZlufXgc8cn4/view?usp=sharing)
