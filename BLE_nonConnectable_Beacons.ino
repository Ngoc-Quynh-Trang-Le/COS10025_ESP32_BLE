#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEAdvertising.h>
#include <esp_bt_device.h>
#include <esp_bt_main.h>

// Select beacon name: uncomment ONE only
#define BEACON_NAME "TraKieu_Apsara_Relief"
// #define BEACON_NAME "Tara_Bodhisattva_Statue"

BLEAdvertising *pAdvertising;

void setup() {
  // Initialize serial logging
  Serial.begin(115200);
  delay(1000); // Allow time for serial monitor to open
  Serial.println("Initializing non-connectable BLE beacon...");

  // Initialize BLE device with specified name
  BLEDevice::init(BEACON_NAME);

  // Retrieve advertising object
  pAdvertising = BLEDevice::getAdvertising();

  // Create new advertisement data
  BLEAdvertisementData advData;
  advData.setFlags(ESP_BLE_ADV_FLAG_GEN_DISC); // General discoverable mode
  advData.setName(BEACON_NAME); // Include device name in advertisement

  // Configure advertising parameters
  pAdvertising->setAdvertisementType(ADV_TYPE_NONCONN_IND); // Non-connectable, undirected
  pAdvertising->setMinInterval(0x00A0); // 100ms in 0.625ms units
  pAdvertising->setMaxInterval(0x00C8); // 200ms in 0.625ms units
  pAdvertising->setAdvertisementData(advData);

  // Start advertising
  BLEDevice::startAdvertising();
  Serial.print("Beacon started. Broadcasting name: ");
  Serial.println(BEACON_NAME);
}

void loop() {
  // Non-blocking loop; BLE runs in background
}
