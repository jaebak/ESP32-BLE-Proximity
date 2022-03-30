# ESP32 BLE Proximity library

A library that measures the Received Signal Strength Indicator (RSSI) of the connected BLE device (example: iphone).
The RSSI can be used to measure the distance from the ESP32 to the connected BLE device.

The code was developed by referencing the [ESP32-BLE-Keyboard library](https://github.com/T-vK/ESP32-BLE-Keyboard) 
and a [post from MelodyApril](https://community.appinventor.mit.edu/t/arduino-distance-meassuring-through-bluetooth-low-energy-signal-strength-rssi-project/24830).

The library was tested with a ESP32 DOIT DEVKIT V1 board and a iPhone X.

## Installation
- Download this library as a zip file.
- In Arduino IDE, press "Sketch" -> "Include Library" -> "Add .ZIP Library..." and select the zip file.

## Example of using the library

This example prints the RSSI of the connected BLE device.

``` C++
#include "BleProximity.h"

// Initialize BleProximity
int16_t BleProximity::m_rssiValue = -999;
bool BleProximity::m_rssiValid = false;
BleProximity bleProximity ("ESP32_proximity");

void setup() {
  Serial.begin(115200);
  Serial.println("Start BLE proximity");
  bleProximity.begin();
}

void loop() {
  if(bleProximity.isConnected()) {
    Serial.println("BLE is connected");
    bleProximity.requestRssi();
    while (!bleProximity.isRssiValid()) delay(50);
    Serial.printf("rssi: %i\r\n", bleProximity.getRssi());
  } else {
    Serial.printf("BLE is not connected\r\n");
  }
  delay(1000);
}
```
