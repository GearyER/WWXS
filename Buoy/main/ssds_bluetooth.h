#ifndef __SSDS_BLUETOOTH__
#define __SSDS_BLUETOOTH__

#include "nvs_flash.h"
#include "esp_bt.h"
#include "esp_gap_ble_api.h"
#include "esp_gatts_api.h"
#include "esp_bt_defs.h"
#include "esp_bt_main.h"
#include "esp_gatt_common_api.h"
#include "esp_bt_device.h"



/* Function definitions */


/* Intializes the bluetooth controller for use in the SSDS station.
 */
void ssds_ble_init();

/* Deinitialize the bluetooth to prepare for deep sleep
 */
void ssds_ble_deinit();

/* Starts advertising the bluetooth controller as a GATT server
*/
void ssds_ble_gatt_advertise();


/* Global variables */

/* Used to detect when the bluetooth controller is fully ready
 * Can begin to use BLE when ssds_ble_status == SSDS_BLE_READY
 */
extern uint8_t ssds_ble_status;
#define ctrl_char_ready         (1 << 0)
#define data_char_ready         (1 << 1)

#define SSDS_BLE_READY (ctrl_char_ready | data_char_ready)



#endif
