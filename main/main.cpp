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
            if (param->adv_start_cmpl.status == ESP_BT_STATUS_SUCCESS) {
                ESP_LOGI(TAG, "Non-connectable advertising started");
            } else {
                ESP_LOGE(TAG, "Failed to start advertising");
            }
            break;
        default:
            break;
    }
}

extern "C" void app_main() {
    // Initialize BT controller
    esp_err_t ret = esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT);
    if (ret) {
        ESP_LOGE(TAG, "Bluetooth controller release failed: %s", esp_err_to_name(ret));
    }

    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    ret = esp_bt_controller_init(&bt_cfg);
    if (ret) {
        ESP_LOGE(TAG, "BT controller init failed: %s", esp_err_to_name(ret));
        return;
    }

    ret = esp_bt_controller_enable(ESP_BT_MODE_BLE);
    if (ret) {
        ESP_LOGE(TAG, "BT controller enable failed: %s", esp_err_to_name(ret));
        return;
    }

    ret = esp_bluedroid_init();
    if (ret) {
        ESP_LOGE(TAG, "Bluedroid init failed: %s", esp_err_to_name(ret));
        return;
    }

    ret = esp_bluedroid_enable();
    if (ret) {
        ESP_LOGE(TAG, "Bluedroid enable failed: %s", esp_err_to_name(ret));
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

    // Start advertising after config is done
    vTaskDelay(pdMS_TO_TICKS(100));
    esp_ble_gap_start_advertising(&adv_params);
}
