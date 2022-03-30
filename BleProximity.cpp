#include "BleProximity.h"

// Report ID:
#define MEDIA_KEYS_ID 0x02

static const uint8_t _hidReportDescriptor[] = {
  // ------------------------------------------------- Media Keys
  USAGE_PAGE(1),      0x0C,          // USAGE_PAGE (Consumer)
  USAGE(1),           0x01,          // USAGE (Consumer Control)
  COLLECTION(1),      0x01,          // COLLECTION (Application)
  REPORT_ID(1),       MEDIA_KEYS_ID, //   REPORT_ID (3)
  USAGE_PAGE(1),      0x0C,          //   USAGE_PAGE (Consumer)
  LOGICAL_MINIMUM(1), 0x00,          //   LOGICAL_MINIMUM (0)
  LOGICAL_MAXIMUM(1), 0x01,          //   LOGICAL_MAXIMUM (1)
  REPORT_SIZE(1),     0x01,          //   REPORT_SIZE (1)
  HIDINPUT(1),        0x02,          //   INPUT (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
  END_COLLECTION(0)                  // END_COLLECTION
};

BleProximity::BleProximity(std::string deviceName, std::string deviceManufacturer)
    : m_pServer(0), m_hid(0), m_advertising(0), m_deviceName(std::string(deviceName).substr(0, 15)),
      m_deviceManufacturer(std::string(deviceManufacturer).substr(0,15)), m_connected(false) {}

void BleProximity::begin(void) {
  BLEDevice::init(m_deviceName);
  m_pServer = BLEDevice::createServer();
  m_pServer->setCallbacks(this);

  m_hid = new BLEHIDDevice(m_pServer);
  m_inputMediaKeys = m_hid->inputReport(MEDIA_KEYS_ID); // This makes pairing.
  m_hid->manufacturer()->setValue(m_deviceManufacturer);
  m_hid->pnp(0x02, 0xe502, 0xa111, 0x0210);
  m_hid->hidInfo(0x00, 0x01);
  m_hid->reportMap((uint8_t*)_hidReportDescriptor, sizeof(_hidReportDescriptor)); // This made autoconnect
  m_hid->startServices();

  m_advertising = m_pServer->getAdvertising();
  m_advertising->addServiceUUID(m_hid->hidService()->getUUID());
  m_advertising->setScanResponse(false); // Do not allow response when scanned
  m_advertising->start();
}

bool BleProximity::isConnected(void) {return this->m_connected;}

esp_err_t BleProximity::requestRssi(void) { 
  m_rssiValid = false;
  m_rssiValue = -999;
  return esp_ble_gap_read_rssi(m_remote_addr); 
} // Request reading RSS

bool BleProximity::isRssiValid(void) {return m_rssiValid;}

int16_t BleProximity::getRssi(void) {return m_rssiValue;}

void BleProximity::onConnect(BLEServer* pServer, esp_ble_gatts_cb_param_t *param) {
  m_connected = true;
  memcpy(&m_remote_addr, param->connect.remote_bda, sizeof(m_remote_addr)); // Copy "remote device bluetooth device address" to m_remote_addr
  BLEDevice::setCustomGapHandler(&BleProximity::rssiEvent); // Install the RSSI callback
}

void BleProximity::onDisconnect(BLEServer* pServer) {
  m_rssiValid = false;
  m_rssiValue = -999;
  m_connected = false;
  BLEDevice::setCustomGapHandler(nullptr);
  m_advertising->start();
}

void BleProximity::rssiEvent(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param) {
  if (event == ESP_GAP_BLE_READ_RSSI_COMPLETE_EVT) {
    m_rssiValue = param->read_rssi_cmpl.rssi;
    m_rssiValid = true;
  }
}
