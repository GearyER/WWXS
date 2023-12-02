/* SSDS Main Driver
 *  This code contains the main driver
 */


#define MAIN
#include "ssds_global.h"
#include "esp_sleep.h"

#define SSDS_TAG        "WXMAI"

// Program entry point

void app_main()
{
    ESP_LOGI(SSDS_TAG, "Chip powered up");


    // Init our non-volatile storage
    esp_err_t ret = init_storage();
    if (ret != ESP_OK){
        ESP_LOGE(SSDS_TAG, "Storage failed to initialize");
    }


    // Determine why we woke up
    esp_sleep_wakeup_cause_t wakeup = esp_sleep_get_wakeup_cause();

    if (wakeup == ESP_SLEEP_WAKEUP_UNDEFINED)
    {
        uint8_t temp_status = get_status();
        ESP_LOGI(SSDS_TAG, "Unknown wakeup source - first boot?");
        ESP_LOGI(SSDS_TAG, "Wakeup status: %u", temp_status);

        /* Most likely a first power on, so we can clear storage */
        reset_storage();

        /* If the board was booted without a wakeup cause, we can assume its idle */
        set_status(STAT_IDLE);
        current_status = STAT_IDLE;
    }
    else
    {
        ESP_LOGI(SSDS_TAG, "Wakeup cause %d", wakeup);
        current_status = get_status();
    }

    // Init BLE
    ssds_ble_init();
    while (ssds_ble_status != SSDS_BLE_READY);
    ESP_LOGI(SSDS_TAG, "BLE ready to use");

    // Init GPIO (for data collection) and the timer (to determine when data gets collected)
    init_gpio();
    init_thread_timer();
    
    /* 
    Need to do a while true statement to allow the buoy to
    constantly collecting data every five minutes.
    Maybe use a timer or a delay function. Either one should work.
    */
    // Calling gather_data from ssds_gpio.c to collect the data from the buoy data
    //esp_err_t result;
    //datapoint_t data = {0};
    //result = gather_data(&data);

    // Print MAC address for testing
    const uint8_t* point = esp_bt_dev_get_address();
    for(int i = 0; i < 5; i++) {
		ESP_LOGI(SSDS_TAG, "%02X:", point[i]);
    }
	ESP_LOGI(SSDS_TAG, "%02X", point[5]);

	// Main method ends - all further work is either started due to the timer or a BLE callback
    ESP_LOGI(SSDS_TAG, "Ending main task");
    fflush(stdout);
    vTaskDelete(NULL);
}
