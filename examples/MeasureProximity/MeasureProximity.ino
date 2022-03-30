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
