#include "BleProximity.h"

// Initialize BleProximity
int16_t BleProximity::m_rssiValue = -999;
bool BleProximity::m_rssiValid = false;
BleProximity bleProximity ("ESP32_proximity");

// For non blocking proximity measurement
bool rssiRequested = false;

void setup() {
  Serial.begin(115200);
  Serial.println("Start BLE proximity");
  bleProximity.begin();
}

void loop() {
  if(bleProximity.isConnected()) {
    Serial.println("BLE is connected");

    // Non blocking proximity measurement
    if (rssiRequested == false) {
      bleProximity.requestRssi();
      rssiRequested = true;
    } else { // rssiRequest == true
      if (bleProximity.isRssiValid()) {
        Serial.printf("rssi: %i\r\n", bleProximity.getRssi());
        rssiRequested = false;
      }
    }

  } else {
    Serial.printf("BLE is not connected\r\n");
  }
  delay(1000);
}
