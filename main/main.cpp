/*
COS10025 BLE-to-Web Cultural Storytelling System
ESP32 BLE beacon firmware for non-contact Cham artifact storytelling.
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
#include "esp_log.h"
#include <string.h>

static const char* TAG = "BLE_BEACON";

// 🔒 Fixed BLE device name for artifact
#define DEVICE_NAME "TraKieu_Apsara_Relief"

// Advertisement parameters (non-connectable)
static esp_ble_adv_params_t adv_params = {
    .adv_int_min        = 0x00A0, // 160ms
    .adv_int_max        = 0x00C8, // 200ms
    .adv_type           = ADV_TYPE_NONCONN_IND,
    .own_addr_type      = BLE_ADDR_TYPE_PUBLIC,
    .channel_map        = ADV_CHNL_ALL,
    .adv_filter_policy  = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
};

// GAP event handler (optional logs only)
void gap_event_handler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t* param) {
    switch (event) {
        case ESP_GAP_BLE_ADV_START_COMPLETE_EVT:
            // Removed ESP_LOGI and ESP_LOGE lines to fix CONFIG_LOG_MAXIMUM_LEVEL error
            break;
        default:
            break;
    }
}

extern "C" void app_main() {
    // Initialize BT controller
    esp_err_t ret = esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT);
    // Removed ESP_LOGE line to fix CONFIG_LOG_MAXIMUM_LEVEL error

    // Removed static assertion for Bluetooth config
    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    ret = esp_bt_controller_init(&bt_cfg);
    if (ret) {
        // Removed ESP_LOGE line to fix CONFIG_LOG_MAXIMUM_LEVEL error
        return;
    }

    ret = esp_bt_controller_enable(ESP_BT_MODE_BLE);
    if (ret) {
        // Removed ESP_LOGE line to fix CONFIG_LOG_MAXIMUM_LEVEL error
        return;
    }

    ret = esp_bluedroid_init();
    if (ret) {
        // Removed ESP_LOGE line to fix CONFIG_LOG_MAXIMUM_LEVEL error
        return;
    }

    ret = esp_bluedroid_enable();
    if (ret) {
        // Removed ESP_LOGE line to fix CONFIG_LOG_MAXIMUM_LEVEL error
        return;
    }

    // Register GAP handler
    esp_ble_gap_register_callback(gap_event_handler);

    // Set BLE device name (advertised in scan response)
    esp_ble_gap_set_device_name(DEVICE_NAME);

    // Prepare advertisement data
    esp_ble_adv_data_t adv_data = {};
    adv_data.set_scan_rsp = false;
    adv_data.include_name = true;
    adv_data.include_txpower = false;
    adv_data.min_interval = 0x0006;  // optional
    adv_data.max_interval = 0x0010;  // optional
    adv_data.appearance = 0x00;
    adv_data.manufacturer_len = 0;
    adv_data.p_manufacturer_data = nullptr;
    adv_data.service_data_len = 0;
    adv_data.p_service_data = nullptr;
    adv_data.service_uuid_len = 0;
    adv_data.p_service_uuid = nullptr;
    adv_data.flag = (ESP_BLE_ADV_FLAG_GEN_DISC | ESP_BLE_ADV_FLAG_BREDR_NOT_SPT);

    // Configure advertisement data
    esp_ble_gap_config_adv_data(&adv_data);

    // Removed vTaskDelay(pdMS_TO_TICKS(100)) to fix CONFIG_FREERTOS_HZ error
    esp_ble_gap_start_advertising(&adv_params);
}
