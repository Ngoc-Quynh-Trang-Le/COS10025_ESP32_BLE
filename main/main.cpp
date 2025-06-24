/*
COS10025 BLE-to-Web Cultural Storytelling System
`Layer 1:` ESP32 BLE beacon firmware for non-contact Cham artifact storytelling.
- Non-connectable BLE advertising only (ADV_NONCONN_IND)
- Human-readable artifact name (e.g., "TraKieu_Apsara_Relief")
- No GATT, no pairing, no connectable services
- Broadcast interval: 100–200 ms
- ESP-IDF v5.4.1, ESP32-D0WD-V3
- See README.md for full project details
*/

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_bt.h"
#include "esp_gap_ble_api.h"
#include "esp_bt_main.h"
#include <string.h>

// #include "esp_log.h" // Optional: Uncomment if logging is needed during debugging
// static const char* TAG = "BLE_BEACON"; // Logging tag, only needed if ESP_LOG is used

// ─────────────────────────────────────────────────────────────────────────────
// Define the beacon name corresponding to a Cham artifact.
// Exactly one DEVICE_NAME should be active at a time.
#define DEVICE_NAME "TraKieu_Apsara_Relief"
// #define DEVICE_NAME "Tara_Bodhisattva_Statue"

// ─────────────────────────────────────────────────────────────────────────────
// BLE Advertisement Parameters Configuration
// - Non-connectable advertising type (ADV_NONCONN_IND)
// - Interval range: 160–200 ms (0x00A0–0x00C8 in 0.625 ms units)
// - Broadcasts on all 3 advertising channels (37, 38, 39)
// - No filter restrictions on scan/connection requests (though connection is disabled)
static esp_ble_adv_params_t adv_params = {
    .adv_int_min        = 0x00A0, // Minimum advertising interval: 160 * 0.625ms = 100ms
    .adv_int_max        = 0x00C8, // Maximum advertising interval: 200 * 0.625ms = 125ms
    .adv_type           = ADV_TYPE_NONCONN_IND, // Non-connectable, undirected advertising
    .own_addr_type      = BLE_ADDR_TYPE_PUBLIC, // Use the device's public BLE address
    .peer_addr_type     = BLE_ADDR_TYPE_PUBLIC, // Required to suppress uninitialized warning
    .channel_map        = ADV_CHNL_ALL,         // Use channels 37, 38, and 39
    .adv_filter_policy  = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY // Allow any device to scan
};

// ─────────────────────────────────────────────────────────────────────────────
// GAP (Generic Access Profile) event handler
// - Required by ESP-IDF BLE stack but unused in this minimal passive beacon
void gap_event_handler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t* param) {
    // No event handling or logging required for passive broadcasting use case
}

// ─────────────────────────────────────────────────────────────────────────────
// Entry Point: app_main
// - Initializes and enables BLE controller and Bluedroid stack
// - Configures and starts BLE advertising with human-readable artifact name
extern "C" void app_main() {
    // Step 1: Free memory reserved for Bluetooth Classic, not used in this project
    esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT);

    // Step 2: Load default configuration for BLE controller
    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();

    // Step 3: Initialize BLE controller
    esp_err_t ret = esp_bt_controller_init(&bt_cfg);
    if (ret) return;

    // Step 4: Enable BLE mode (BLE-only operation)
    ret = esp_bt_controller_enable(ESP_BT_MODE_BLE);
    if (ret) return;

    // Step 5: Initialize the Bluedroid stack (ESP's BLE host stack)
    ret = esp_bluedroid_init();
    if (ret) return;

    // Step 6: Enable Bluedroid
    ret = esp_bluedroid_enable();
    if (ret) return;

    // Step 7: Register a GAP callback — mandatory even if unused
    esp_ble_gap_register_callback(gap_event_handler);

    // Step 8: Set the BLE device name (visible in advertising packet)
    esp_ble_gap_set_device_name(DEVICE_NAME);

    // Step 9: Configure advertising data structure
    // - No scan response
    // - Broadcast only the device name (no UUIDs, services, TX power)
    esp_ble_adv_data_t adv_data = {};
    adv_data.set_scan_rsp = false;                     // No scan response packet
    adv_data.include_name = true;                      // Include device name in ADV payload
    adv_data.include_txpower = false;                  // Do not include TX power
    adv_data.min_interval = 0x0006;                    // Optional: placeholder for iOS compliance
    adv_data.max_interval = 0x0010;                    // Optional: placeholder for iOS compliance
    adv_data.appearance = 0x00;                        // No specific icon/appearance
    adv_data.manufacturer_len = 0;                     // No manufacturer-specific data
    adv_data.p_manufacturer_data = nullptr;
    adv_data.service_data_len = 0;                     // No service-specific data
    adv_data.p_service_data = nullptr;
    adv_data.service_uuid_len = 0;                     // No UUIDs advertised
    adv_data.p_service_uuid = nullptr;
    adv_data.flag = (ESP_BLE_ADV_FLAG_GEN_DISC | ESP_BLE_ADV_FLAG_BREDR_NOT_SPT);
    // - General discoverable mode
    // - BR/EDR (classic Bluetooth) not supported

    // Step 10: Apply advertising data configuration
    esp_ble_gap_config_adv_data(&adv_data);

    // Step 11: Start advertising immediately (without waiting for config event)
    // Note: In production systems, wait for ESP_GAP_BLE_ADV_DATA_SET_COMPLETE_EVT
    esp_ble_gap_start_advertising(&adv_params);
}
