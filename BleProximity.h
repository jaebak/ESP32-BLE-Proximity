#ifndef ESP32_BLE_PROXIMITY_H
#define ESP32_BLE_PROXIMITY_H

#include "BLEDevice.h"
#include "BLEHIDDevice.h"

class BleProximity : public BLEServerCallbacks, public BLECharacteristicCallbacks {
private:
  BLEServer*         m_pServer;
  BLEHIDDevice* m_hid;
  BLECharacteristic* m_inputMediaKeys;
  BLEAdvertising*    m_advertising;
  std::string        m_deviceName;
  std::string        m_deviceManufacturer;
  bool               m_connected;
  esp_bd_addr_t      m_remote_addr;
  static int16_t            m_rssiValue;
  static bool               m_rssiValid;

public:
  BleProximity(std::string deviceName = "ESP32 Proximity", std::string deviceManufacturer = "Espressif");
  void begin(void);
  bool isConnected(void);
  esp_err_t requestRssi(void);
  bool isRssiValid(void);
  int16_t getRssi(void);

protected:
  virtual void onConnect(BLEServer* pServer, esp_ble_gatts_cb_param_t *param) override;
  virtual void onDisconnect(BLEServer* pServer) override;
  static void rssiEvent(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param);
};

#endif // ESP32_BLE_PROXIMITY_H
