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
#include <string.h>

// #include "esp_log.h" // Uncomment if you want to use ESP_LOGI/ESP_LOGE
// static const char* TAG = "BLE_BEACON"; // Remove if not using logging

// Select beacon name: uncomment ONE only
#define DEVICE_NAME "TraKieu_Apsara_Relief"
// #define DEVICE_NAME "Tara_Bodhisattva_Statue"

// Advertisement parameters (non-connectable)
static esp_ble_adv_params_t adv_params = {
    .adv_int_min        = 0x00A0, // 160ms
    .adv_int_max        = 0x00C8, // 200ms
    .adv_type           = ADV_TYPE_NONCONN_IND,
    .own_addr_type      = BLE_ADDR_TYPE_PUBLIC,
    .peer_addr_type     = BLE_ADDR_TYPE_PUBLIC, // Fix missing initializer warning
    .channel_map        = ADV_CHNL_ALL,
    .adv_filter_policy  = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
};

void gap_event_handler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t* param) {
    // No logging needed for minimal beacon
}

extern "C" void app_main() {
    // Release memory for classic BT
    esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT);

    // Use default controller config macro (requires Bluetooth enabled in menuconfig)
    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    esp_err_t ret = esp_bt_controller_init(&bt_cfg);
    if (ret) return;

    ret = esp_bt_controller_enable(ESP_BT_MODE_BLE);
    if (ret) return;

    ret = esp_bluedroid_init();
    if (ret) return;

    ret = esp_bluedroid_enable();
    if (ret) return;

    esp_ble_gap_register_callback(gap_event_handler);
    esp_ble_gap_set_device_name(DEVICE_NAME);

    esp_ble_adv_data_t adv_data = {};
    adv_data.set_scan_rsp = false;
    adv_data.include_name = true;
    adv_data.include_txpower = false;
    adv_data.min_interval = 0x0006;
    adv_data.max_interval = 0x0010;
    adv_data.appearance = 0x00;
    adv_data.manufacturer_len = 0;
    adv_data.p_manufacturer_data = nullptr;
    adv_data.service_data_len = 0;
    adv_data.p_service_data = nullptr;
    adv_data.service_uuid_len = 0;
    adv_data.p_service_uuid = nullptr;
    adv_data.flag = (ESP_BLE_ADV_FLAG_GEN_DISC | ESP_BLE_ADV_FLAG_BREDR_NOT_SPT);

    esp_ble_gap_config_adv_data(&adv_data);

    // Wait for adv data config event before starting advertising in production.
    // For simple demo, start advertising immediately:
    esp_ble_gap_start_advertising(&adv_params);
}
