/* Driver code for the ESP32 BLE functionality
   Tailored specifically for the Salish Sea Data System,
   2019 edition.

   Heavily based off the example provided by ESP32 at
   https://github.com/espressif/esp-idf/tree/master/examples/bluetooth/bluedroid/ble/gatt_server

   This code runs a singular service with two characteristics:

   The characteristic with ID 0xABC0 is the control characteristic. This
   characteristic listens for control commands from the app to tell the
   station what to do.

   The characteristic with ID 0xABC1 is the data characteristic. This
   characteristic is meant for the transfer back and forth of large amounts
   of data, such as encryption keys and data points. This characteristic
   should only be accessed after sending an appropriate control message,
   telling the station what to expect.

 */

#include "ssds_global.h"


//--------- Defines ---------//

#define BT_TAG "BLULE"
#define PREPARE_BUF_MAX_SIZE 1024
#define SSDS_APP_ID 0
#define GATTS_HANDLE_NUM 8
#define GATT_CTRL_CHAR_UUID 0xABC0
#define GATT_DATA_CHAR_UUID 0xABC1
#define GATT_CTRL_CHAR_DESC_UUID 0xCBA0
#define GATT_DATA_CHAR_DESC_UUID 0xCBA1
#define DEVICE_NAME "Water Weather Station"
#define GATTS_CHAR_VAL_LEN_MAX 0x40

/* Command values */
#define COMMAND_NONE        0x00
#define RECEIVE_RSA         0x01
#define BEGIN_RECORDING     0x02
#define END_RECORDING       0x03

//--------- Structure Definitions ---------//

typedef struct {
    uint8_t                 *prepare_buf;
    int                     prepare_len;
} prepare_type_env_t;

struct gatts_profile {
    uint16_t gatts_if;
    uint16_t app_id;
    uint16_t conn_id;
    uint16_t service_handle;
    esp_gatt_srvc_id_t service_id;
    uint16_t ctrl_char_handle;
    esp_bt_uuid_t ctrl_char_uuid;
    uint16_t data_char_handle;
    esp_bt_uuid_t data_char_uuid;
    uint16_t ctrl_descr_handle;
    esp_bt_uuid_t ctrl_descr_uuid;
    uint16_t data_descr_handle;
    esp_bt_uuid_t data_descr_uuid;
};


//---------- Global Data ----------//


/* Current command from the android app we are acting on.
 * Can be one of COMMAND_NONE, BEGIN_RECORDING, END_RECORDING, RECEIVE_RSA. */
static uint8_t active_command = COMMAND_NONE;

/* Bitwise flag to determine when both advertisement
 * and scan response configuration is done
 */
static uint8_t adv_config_done = 0;
#define adv_config_flag         (1 << 0)
#define scan_rsp_config_flag    (1 << 1)


/* Service ID being broadcast by the chip */
static uint8_t adv_service_uuid128[16] = {
    0x11, 0x22, 0x33, 0x44, 0x11, 0x22, 0x33, 0x44, 0x11, 0x22, 0x33, 0x44, 0x11, 0x22, 0x33, 0x44
};

/* Advertising data */
static esp_ble_adv_data_t adv_data = {
    .set_scan_rsp = false,
    .include_name = true,
    .include_txpower = true,
    .min_interval = 0x0006, //slave connection min interval, Time = min_interval * 1.25 msec
    .max_interval = 0x0010, //slave connection max interval, Time = max_interval * 1.25 msec
    .appearance = 0x00,
    .manufacturer_len = 0, //TEST_MANUFACTURER_DATA_LEN,
    .p_manufacturer_data =  NULL, //&test_manufacturer[0],
    .service_data_len = 0,
    .p_service_data = NULL,
    .service_uuid_len = ESP_UUID_LEN_128,
    .p_service_uuid = adv_service_uuid128,
    .flag = (ESP_BLE_ADV_FLAG_GEN_DISC | ESP_BLE_ADV_FLAG_BREDR_NOT_SPT),
};

/* Scan response data */
static esp_ble_adv_data_t scan_rsp_data = {
    .set_scan_rsp = true,
    .include_name = true,
    .include_txpower = true,
    .min_interval = 0x0006, //slave connection min interval, Time = min_interval * 1.25 msec
    .max_interval = 0x0010, //slave connection max interval, Time = max_interval * 1.25 msec
    .appearance = 0x00,
    .manufacturer_len = 0, //TEST_MANUFACTURER_DATA_LEN,
    .p_manufacturer_data =  NULL, //&test_manufacturer[0],
    .service_data_len = 0,
    .p_service_data = NULL,
    .service_uuid_len = ESP_UUID_LEN_128,
    .p_service_uuid = adv_service_uuid128,
    .flag = (ESP_BLE_ADV_FLAG_GEN_DISC | ESP_BLE_ADV_FLAG_BREDR_NOT_SPT),
};

/* Advertisement parameters */
static esp_ble_adv_params_t adv_params = {
    .adv_int_min        = 0x20,
    .adv_int_max        = 0x40,
    .adv_type           = ADV_TYPE_IND,
    .own_addr_type      = BLE_ADDR_TYPE_PUBLIC,
    .channel_map        = ADV_CHNL_ALL,
    .adv_filter_policy  = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
};


/* Creating a profile for our BLE app */
static struct gatts_profile ssds_profile = {
    .gatts_if = ESP_GATT_IF_NONE,
    .app_id = SSDS_APP_ID
};

/* Write environment declaration for use in WRITE and EXEC_WRITE events */
static prepare_type_env_t prep_write_env;


/* GATT characteristic properties */
static esp_gatt_char_prop_t ctrl_char_property = 0;
static esp_gatt_char_prop_t data_char_property = 0;



/* Default characteristic data */
static uint8_t ctrl_string[] = {0xDE,0xAD,0xBE,0xEF};
static uint8_t data_string[] = {0xDE,0xAD,0xBE,0xEF};

/* Default characteristic values for the control and data characteristics */
static esp_attr_value_t ctrl_char_val = {
    .attr_max_len   = GATTS_CHAR_VAL_LEN_MAX,
    .attr_len       = sizeof(ctrl_string),
    .attr_value     = ctrl_string,
};

static esp_attr_value_t data_char_val = {
    .attr_max_len   = GATTS_CHAR_VAL_LEN_MAX,
    .attr_len       = sizeof(data_string),
    .attr_value     = data_string,
};

/*--------- FUNCTIONS ---------*/


void send_indicate(uint8_t flag) {
    read_type_flag = flag;
    uint8_t rsp[1] = {0x01};
    esp_ble_gatts_send_indicate(ssds_profile.gatts_if, ssds_profile.conn_id,
                                ssds_profile.data_char_handle, 1, rsp, false);
}


/* Helper method used when client sends data following a command.
 * What we do with this data depends on what command was previously set.
 */
static void handle_data(uint8_t * data, int size) {
	ESP_LOGI(BT_TAG, "Data handle");
	ESP_LOGI(BT_TAG, "Data write size: %d", size);

	switch(active_command) {
		case BEGIN_RECORDING: {
			// Receive time of day
			if(size > 8)
			{
				ESP_LOGE(BT_TAG, "Received message of incorrect length.");
				break;
			}

			struct timeval tv;

			tv.tv_sec = (time_t)data[0] |
			            (time_t)data[1] << 8 |
			            (time_t)data[2] << 16 |
			            (time_t)data[3] << 24;

			if(settimeofday(&tv, NULL) < 0) {
				ESP_LOGE(BT_TAG, "Time failed to update");
			} else {
				ESP_LOGI(BT_TAG, "Updated time: %lu", tv.tv_sec);
			}

            resume_thread_timer();

			active_command = COMMAND_NONE;
			break;
		}

		case END_RECORDING: {
			// No use for data. Print all datapoints for testing.
			dp_iterator_t* iterator = get_dp_iterator(0, 0);
			datapoint_t curr;

			get_next_dp(iterator, &curr);
			while(curr.tv_sec != 0) {
				print_datapoint(&curr);
				get_next_dp(iterator, &curr);
			}

			close_dp_iterator(iterator);
			break;
		}

		case RECEIVE_RSA: {
			ESP_LOGI(BT_TAG, "Recieved RSA public key");

			esp_err_t ret = init_encryption((unsigned char*)data, size);

			if (ret != ESP_OK) {
				ESP_LOGE(BT_TAG, "Error using public key, not sending key back");
			} else {
				send_indicate(KEY_READY);
			}

			active_command = COMMAND_NONE;
			break;
		}

		default: {
			ESP_LOGW(BT_TAG, "No active command when receiving data");
			break;
		}
	}
}

/* Called when the app requests to read some data from the buoy.
 * Response is stored in *rsp.
 * The data we respond with depends on our local state (read_type_flag).
 */
static void ble_data_read(esp_gatt_rsp_t *rsp) {
    /* Supporting variables for reading from the app */
    static uint8_t* data_buf;
    static uint16_t data_offset = 0;
    static uint16_t data_size = 0;
    static dp_iterator_t* iter;

    switch (read_type_flag) {
        case KEY_READY: {
            /* First time we've started sending the key */
            if (data_offset == 0) {
                data_size = get_encrypted_key_length();
                data_buf = get_encrypted_key();
            }

            uint16_t remaining_data = data_size - data_offset;
            uint8_t send_size = remaining_data > 22 ? 22 : remaining_data;

            rsp->attr_value.len = send_size;

            memcpy(rsp->attr_value.value, data_buf + data_offset, send_size);

            data_offset += send_size;
            if (data_offset >= data_size) {
                read_type_flag = IV_READY;
                data_offset = 0;
                data_size = 0;
            }
            break;
        }

        case IV_READY: {
            data_buf = get_aes_iv();

            rsp->attr_value.len = 16;
            memcpy(rsp->attr_value.value, data_buf, 16);

            printf("aes_iv: ");
            for(int i = 0; i < 16; i++) {
                printf("%02x", data_buf[i]);
            }
            printf("\n");

            read_type_flag = DP_COUNT;
            break;
        }

		/* App has requested number of datapoints */
        case DP_COUNT: {
            iter = get_dp_iterator(0, 0);

            rsp->attr_value.len = sizeof(iter->dp_count);
            memcpy(rsp->attr_value.value, &iter->dp_count, rsp->attr_value.len);

            printf("dp_count: %d\n", iter->dp_count);

            read_type_flag = DATA_READY;
            break;
        }

		/* App has requested data */
        case DATA_READY: {        
            datapoint_t data;
            get_next_dp(iter, &data);
            print_datapoint(&data);

            // check for final datapoint
            if(data.tv_sec == 0) {
                close_dp_iterator(iter);
                read_type_flag = READ_NONE;
            }

            //Store data in rsp. 16 bit ints have to be converted to two 8 bit ints
            //First low order bits then high order bits
            rsp->attr_value.len = 16;
            rsp->attr_value.value[0] = data.tv_sec & 0xff;
            rsp->attr_value.value[1] = (data.tv_sec >> 8) & 0xff;
            rsp->attr_value.value[2] = (data.tv_sec >> 16) & 0xff;
            rsp->attr_value.value[3] = (data.tv_sec >> 24) & 0xff;
            rsp->attr_value.value[4] = data.temp1 & 0xff;
            rsp->attr_value.value[5] = (data.temp1 >> 8);
            rsp->attr_value.value[6] = data.temp2 & 0xff;
            rsp->attr_value.value[7] = (data.temp2 >> 8);
            rsp->attr_value.value[8] = data.temp3 & 0xff;
            rsp->attr_value.value[9] = (data.temp3 >> 8);
            rsp->attr_value.value[10] = data.salinity & 0xff;
            rsp->attr_value.value[11] = (data.salinity >> 8);
            rsp->attr_value.value[12] = data.light & 0xff;
            rsp->attr_value.value[13] = (data.light >> 8);
            rsp->attr_value.value[14] = data.turbidity & 0xff;
            rsp->attr_value.value[15] = (data.turbidity >> 8);
            // store encrypted datapoint in response
            /*encrypt_block((unsigned char*)&data, 
                          (unsigned char*)rsp->attr_value.value, 
                          sizeof(datapoint_t));*/

			printf("---Encrypted---\n");
			print_datapoint((datapoint_t*) rsp->attr_value.value);
            
            break;
        }

        default: {
            rsp->attr_value.len = 6;
            rsp->attr_value.value[0] = 0xaa;
            rsp->attr_value.value[1] = 0xbb;
            rsp->attr_value.value[2] = 0xcc;
            rsp->attr_value.value[3] = 0xdd;
            rsp->attr_value.value[4] = 0xee;
            rsp->attr_value.value[5] = 0xff;
        }
    }
}

/* Write Event Environment
* 
*  Initialize the environment for handling write events
*/
static void write_event_env(esp_gatt_if_t gatts_if,
                            prepare_type_env_t *prepare_write_env,
                            esp_ble_gatts_cb_param_t *param) {

    esp_gatt_status_t status = ESP_GATT_OK;
    if (param->write.need_rsp) {
        if (param->write.is_prep) {
            if (prepare_write_env->prepare_buf == NULL) {
			prepare_write_env->prepare_buf = (uint8_t *)malloc(PREPARE_BUF_MAX_SIZE*sizeof(uint8_t));
                prepare_write_env->prepare_len = 0;
                if (prepare_write_env->prepare_buf == NULL) {
                    ESP_LOGE(BT_TAG, "Gatt_server prep no mem\n");
                    status = ESP_GATT_NO_RESOURCES;
                }
            } else {
                if(param->write.offset > PREPARE_BUF_MAX_SIZE) {
                    status = ESP_GATT_INVALID_OFFSET;
                } else if ((param->write.offset + param->write.len) > PREPARE_BUF_MAX_SIZE) {
                    status = ESP_GATT_INVALID_ATTR_LEN;
                }
            }

            // Prepare the GATT response
            esp_gatt_rsp_t *gatt_rsp = (esp_gatt_rsp_t *)malloc(sizeof(esp_gatt_rsp_t));
            gatt_rsp->attr_value.len = param->write.len;
            gatt_rsp->attr_value.handle = param->write.handle;
            gatt_rsp->attr_value.offset = param->write.offset;
            gatt_rsp->attr_value.auth_req = ESP_GATT_AUTH_REQ_NONE;
            memcpy(gatt_rsp->attr_value.value, param->write.value, param->write.len);
            esp_err_t response_err = esp_ble_gatts_send_response(gatts_if, param->write.conn_id, param->write.trans_id, status, gatt_rsp);
            if (response_err != ESP_OK) {
               ESP_LOGE(BT_TAG, "Send response error\n");
            }
            free(gatt_rsp);
            if (status != ESP_GATT_OK) {
                return;
            }
            memcpy(prepare_write_env->prepare_buf + param->write.offset,
                   param->write.value,
                   param->write.len);
            prepare_write_env->prepare_len += param->write.len;

        } else {
            esp_ble_gatts_send_response(gatts_if, param->write.conn_id, param->write.trans_id, status, NULL);
        }
    }
}

/* Execute Write Event Environment
*
*  Handle a writing event.
*/
static void exec_write_event_env(prepare_type_env_t *prepare_write_env, esp_ble_gatts_cb_param_t *param){
    if (param->exec_write.exec_write_flag == ESP_GATT_PREP_WRITE_EXEC){
        //ESP_LOGI(BT_TAG, "Dumping exec write contents");
        //esp_log_buffer_hex(BT_TAG, prepare_write_env->prepare_buf, prepare_write_env->prepare_len);

		handle_data(prepare_write_env->prepare_buf, prepare_write_env->prepare_len);
    }else{
        ESP_LOGI(BT_TAG,"ESP_GATT_PREP_WRITE_CANCEL");
    }
    if (prepare_write_env->prepare_buf) {
            free(prepare_write_env->prepare_buf);
        prepare_write_env->prepare_buf = NULL;
    }
    prepare_write_env->prepare_len = 0;
}


/* GAP Event Handler
*
*  Handle GAP events
*/
static void gap_event_handler (esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param) {
    switch (event) {

    	/* Waiting for both the advertisement and scan
    	 * response configuration to be complete before
    	 * starting to advertise
    	 */
    	case ESP_GAP_BLE_ADV_DATA_SET_COMPLETE_EVT: {
	        adv_config_done &= (~adv_config_flag);
    	    if (adv_config_done == 0) {
        	    esp_ble_gap_start_advertising(&adv_params);
        	}
        	break;
    	}
        
        /* Config for scan response data is complete */
    	case ESP_GAP_BLE_SCAN_RSP_DATA_SET_COMPLETE_EVT: {
        	adv_config_done &= (~scan_rsp_config_flag);
        	if (adv_config_done == 0) {
        	    esp_ble_gap_start_advertising(&adv_params);
        	}
        	break;
    	}

    	/* Advertisement start complete */
    	case ESP_GAP_BLE_ADV_START_COMPLETE_EVT: {
        	if (param->adv_start_cmpl.status != ESP_BT_STATUS_SUCCESS) {
            	ESP_LOGE(BT_TAG, "Advertising start failed");
        	}
        	else {
            	ESP_LOGI(BT_TAG, "GAP advertisement begun");
        	}
        	break;
    	}

    	/* Advertisement stop complete */
    	case ESP_GAP_BLE_ADV_STOP_COMPLETE_EVT: {
        	if (param->adv_stop_cmpl.status != ESP_BT_STATUS_SUCCESS) {
            	ESP_LOGE(BT_TAG, "Advertising stop failed");
        	} else {
            	ESP_LOGI(BT_TAG, "GAP advertisement ended");
        	}
        	break;
    	}

    	/* Connection parameters have been updated */
    	case ESP_GAP_BLE_UPDATE_CONN_PARAMS_EVT: {
        	ESP_LOGI(BT_TAG, "update connection params status = %x, min_int = %d, max_int = %d,conn_int = %d,latency = %d, timeout = %d",
                  param->update_conn_params.status,
                  param->update_conn_params.min_int,
                  param->update_conn_params.max_int,
                  param->update_conn_params.conn_int,
                  param->update_conn_params.latency,
                  param->update_conn_params.timeout);
        	break;
    	}

    	default: {
        	ESP_LOGI(BT_TAG, "GAP event %x", event);
        	break;
		}
    }
}



/* GATTs Event Handler
*
*  Handles GATT events in which two bluetooth devices
*  interact with eachother
*/
static void gatts_event_handler (esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param)
{
    switch (event)
    {

    /* New application registered */
    case ESP_GATTS_REG_EVT:
    {
        if (param->reg.status == ESP_GATT_OK) {
            ESP_LOGI(BT_TAG, "App registered, app_id %d",
                param->reg.app_id);
            ssds_profile.gatts_if = gatts_if;

            ssds_profile.service_id.is_primary = true;
            ssds_profile.service_id.id.inst_id = 0x00;
            ssds_profile.service_id.id.uuid.len = ESP_UUID_LEN_128; // Set to 128 bit to better adhere to BLE standards
            memcpy(
                ssds_profile.service_id.id.uuid.uuid.uuid128,
                adv_service_uuid128,
                16
            );

            // Set device name
            esp_err_t set_name_ret = esp_ble_gap_set_device_name(DEVICE_NAME);
            if (set_name_ret){
                ESP_LOGE(BT_TAG, "set device name failed, error code = %x", set_name_ret);
            }

            esp_err_t ret;

            /* Setting the advertisment data */
            ret = esp_ble_gap_config_adv_data (&adv_data);
            if (ret) {
                ESP_LOGE(BT_TAG, "Config gap advertise data failed, error code = %x", ret);
            }
            adv_config_done |= adv_config_flag;

            /* Setting the scan response data */
            ret = esp_ble_gap_config_adv_data (&scan_rsp_data);
            if (ret) {
                ESP_LOGE(BT_TAG, "Config gap scan response data failed, error code = %x", ret);
            }
            adv_config_done |= scan_rsp_config_flag;

            /* Creating the GATT service */
            esp_ble_gatts_create_service (gatts_if, &ssds_profile.service_id, GATTS_HANDLE_NUM);
        }
        else {
            ESP_LOGI(BT_TAG, "Register app failed, app_id %d, status %04x",
                param->reg.app_id,
                param->reg.status);
        }
        break;
    }

    /* Client connected */
    case ESP_GATTS_CONNECT_EVT:
    {
        esp_ble_conn_update_params_t conn_params = {0};
        memcpy(conn_params.bda, param->connect.remote_bda, sizeof(esp_bd_addr_t));
        conn_params.latency = 0;
        conn_params.max_int = 0x20;
        conn_params.min_int = 0x10;
        conn_params.timeout = 400;
        ESP_LOGI(BT_TAG, "GATT connect, conn_id %d, remote %02x:%02x:%02x:%02x:%02x:%02x",
                 param->connect.conn_id,
                 param->connect.remote_bda[0], param->connect.remote_bda[1], param->connect.remote_bda[2],
                 param->connect.remote_bda[3], param->connect.remote_bda[4], param->connect.remote_bda[5]);
        ssds_profile.conn_id = param->connect.conn_id;
        esp_ble_gap_update_conn_params(&conn_params);

		// pause collection until disconnect
        pause_thread_timer();
        //resume_thread_timer();
        read_type_flag = DP_COUNT;
        break;
    }

    /* Client disconnected */
    case ESP_GATTS_DISCONNECT_EVT:
    {
        ESP_LOGI(BT_TAG, "Disconnect event, disconnect reason 0x%x", param->disconnect.reason);
        if(param->disconnect.reason == 0x13) {
            ESP_LOGI(BT_TAG, "Disconnect reason 0x13, restarting advertising");
            esp_ble_gap_start_advertising(&adv_params);
        }

		// resume collection
		end_encryption();
		//resume_thread_timer();
        break;
    }

    /* Client requested a read */
    case ESP_GATTS_READ_EVT: {
        ESP_LOGI(BT_TAG, "Read event, conn_id %d, trans_id %d, handle %d", param->read.conn_id, param->read.trans_id, param->read.handle);
        esp_gatt_rsp_t rsp;
        memset(&rsp, 0, sizeof(esp_gatt_rsp_t));

        rsp.attr_value.handle = param->read.handle;

        if (param->read.handle == ssds_profile.ctrl_char_handle)
        {

            rsp.attr_value.len = 2;
            rsp.attr_value.value[1] = 0x00;

            /* If we are in the middle of data transfer */
            if (read_type_flag != READ_NONE){
                ESP_LOGI(BT_TAG, "Read EVT during data transfer");
                if(read_type_flag == DATA_READY){
                    rsp.attr_value.value[0] = STAT_PROCESS;
                }
                else{
                    rsp.attr_value.value[0] = 0x00;
                }
            }

            /* Normal status check */
            else {
                rsp.attr_value.value[0] = get_status();
            }

        }

        else if (param->read.handle == ssds_profile.data_char_handle)
        {
            ESP_LOGI(BT_TAG, "Data read");
            ble_data_read(&rsp);
            current_status = STAT_PROCESS;
        }

        esp_ble_gatts_send_response(gatts_if, param->read.conn_id, param->read.trans_id,
                                    ESP_GATT_OK, &rsp);
        break;
    }

    /* Client requested to write */
    case ESP_GATTS_WRITE_EVT:
    {
        ESP_LOGI(BT_TAG, "Write event, conn_id %d, trans_id %d, handle %d", param->write.conn_id, param->write.trans_id, param->write.handle);
        if (!param->write.is_prep){
            ESP_LOGI(BT_TAG, "Write event, value len %d, value :", param->write.len);
            esp_log_buffer_hex(BT_TAG, param->write.value, param->write.len);

            if (param->write.handle == ssds_profile.ctrl_char_handle)
            {
                ESP_LOGI(BT_TAG, "Control handle");
                if (param->write.len != 2) {
                    ESP_LOGW(BT_TAG, "Invalid control sequence");
                }
                else {
					ESP_LOGI(BT_TAG, "Command number: %02x", param->write.value[0]);
					active_command = param->write.value[0];
                }
            }

            else if (param->write.handle == ssds_profile.data_char_handle)
            {
                handle_data(param->write.value, param->write.len);
            }
        }
        write_event_env(gatts_if, &prep_write_env, param);
        break;
    }

    /* Client requested exec write */
    case ESP_GATTS_EXEC_WRITE_EVT:
    {
        ESP_LOGI(BT_TAG,"Exec Write event");
        esp_ble_gatts_send_response(gatts_if, param->write.conn_id, param->write.trans_id, ESP_GATT_OK, NULL);
        exec_write_event_env(&prep_write_env, param);
        break;
    }


    /* Service creation completed */
    case ESP_GATTS_CREATE_EVT:
    {
        ESP_LOGI(BT_TAG, "Service created, status %x", param->create.status);

        ssds_profile.service_handle = param->create.service_handle;

        esp_err_t add_char_ret;

        /* Adding the control characteristic */
        ssds_profile.ctrl_char_uuid.len = ESP_UUID_LEN_16;
        ssds_profile.ctrl_char_uuid.uuid.uuid16 = GATT_CTRL_CHAR_UUID;
        ctrl_char_property = ESP_GATT_CHAR_PROP_BIT_READ | ESP_GATT_CHAR_PROP_BIT_WRITE;

        add_char_ret = esp_ble_gatts_add_char (ssds_profile.service_handle,
                                    &ssds_profile.ctrl_char_uuid,
                                    ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE,
                                    ctrl_char_property,
                                    &ctrl_char_val,
                                    NULL);
        if (add_char_ret){
            ESP_LOGE(BT_TAG, "Add char failed, error code %x", add_char_ret);
        }

        break;
    }


    /* Characteristic addition complete */
    case ESP_GATTS_ADD_CHAR_EVT:
    {
        uint16_t length = 0;
        const uint8_t *prf_char;

        ESP_LOGI (BT_TAG, "Add char event, status %x, attr_handle %d, service_handle %d, char_uuid %x",
            param->add_char.status, param->add_char.attr_handle,
            param->add_char.service_handle, param->add_char.char_uuid.uuid.uuid16);

        /* Control characteristic */
        if (param->add_char.char_uuid.uuid.uuid16 == GATT_CTRL_CHAR_UUID)
        {
            ESP_LOGI (BT_TAG, "Added control characteristic");
            ssds_profile.ctrl_char_handle = param->add_char.attr_handle;
            ssds_profile.ctrl_descr_uuid.len = ESP_UUID_LEN_16;
            ssds_profile.ctrl_descr_uuid.uuid.uuid16 = GATT_CTRL_CHAR_DESC_UUID;


            /* Adding the characteristic descriptor */
            esp_err_t add_descr_ret = esp_ble_gatts_add_char_descr(ssds_profile.service_handle, &ssds_profile.ctrl_descr_uuid,
                                                                    ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE, NULL, NULL);
            if (add_descr_ret){
                ESP_LOGE(BT_TAG, "add char descr failed, error code =%x", add_descr_ret);
            }

            /* Adding the data characteristic */
            ssds_profile.data_char_uuid.len = ESP_UUID_LEN_16;
            ssds_profile.data_char_uuid.uuid.uuid16 = GATT_DATA_CHAR_UUID;
            data_char_property = ESP_GATT_CHAR_PROP_BIT_READ | ESP_GATT_CHAR_PROP_BIT_WRITE | ESP_GATT_CHAR_PROP_BIT_NOTIFY | ESP_GATT_CHAR_PROP_BIT_INDICATE;

            esp_err_t add_char_ret = esp_ble_gatts_add_char (ssds_profile.service_handle,
                                        &ssds_profile.data_char_uuid,
                                        ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE,
                                        data_char_property,
                                        &data_char_val,
                                        NULL);

            if (add_char_ret){
                ESP_LOGE(BT_TAG, "Add char failed, error code %x", add_char_ret);
            }
        }

        else if (param->add_char.char_uuid.uuid.uuid16 == GATT_DATA_CHAR_UUID)
        {
            ESP_LOGI (BT_TAG, "Added data characteristic");
            ssds_profile.data_char_handle = param->add_char.attr_handle;
            ssds_profile.data_descr_uuid.len = ESP_UUID_LEN_16;
            ssds_profile.data_descr_uuid.uuid.uuid16 = GATT_DATA_CHAR_DESC_UUID;

            esp_ble_gatts_start_service(ssds_profile.service_handle);

            /* Adding the characteristic descriptor */
            esp_err_t add_descr_ret = esp_ble_gatts_add_char_descr(ssds_profile.service_handle, &ssds_profile.data_descr_uuid,
                                                                    ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE, NULL, NULL);
            if (add_descr_ret){
                ESP_LOGE(BT_TAG, "add char descr failed, error code =%x", add_descr_ret);
            }
        }

        else{
            ESP_LOGE (BT_TAG, "Invalid characteritsic added");
        }


        esp_err_t get_attr_ret = esp_ble_gatts_get_attr_value(param->add_char.attr_handle,  &length, &prf_char);
        if (get_attr_ret == ESP_FAIL){
            ESP_LOGE(BT_TAG, "ILLEGAL HANDLE");
        }

        break;
    }

    /* Characteristic description addition complete */
    case ESP_GATTS_ADD_CHAR_DESCR_EVT:
    {
        ESP_LOGI(BT_TAG, "GATT Add characteristic descriptor event");

        if (param->add_char_descr.descr_uuid.uuid.uuid16 == ssds_profile.ctrl_descr_uuid.uuid.uuid16)
        {
            ssds_ble_status |= ctrl_char_ready;
        }
        else if (param->add_char_descr.descr_uuid.uuid.uuid16 == ssds_profile.data_descr_uuid.uuid.uuid16)
        {
            ssds_ble_status |= data_char_ready;
        }

        break;
    }
    /* Service started */
    case ESP_GATTS_START_EVT:
    {
        ESP_LOGI(BT_TAG, "SERVICE_START_EVT, status %x, service_handle %d",
                 param->start.status, param->start.service_handle);
        break;
    }

    /* Service stopped */
    case ESP_GATTS_STOP_EVT:
    {
        ESP_LOGI(BT_TAG, "SERVICE_STOP_EVT");
        break;
    }

    case ESP_GATTS_RESPONSE_EVT:
    {
        ESP_LOGI(BT_TAG, "GATT_RESPONSE_EVT");
        break;
    }

    case ESP_GATTS_CONF_EVT:
    {
        ESP_LOGI(BT_TAG, "GATT_CONFIRM_EVT");
        break;
    }

    /* Service deleted */
    case ESP_GATTS_DELETE_EVT:
    /* MTU change complete */
    case ESP_GATTS_MTU_EVT:
    /* Service addition complete */
    case ESP_GATTS_ADD_INCL_SRVC_EVT:
    /* Application unregistered */
    /*
    case ESP_GATTS_UNREG_EVT:
    case ESP_GATTS_OPEN_EVT:
    case ESP_GATTS_CANCEL_OPEN_EVT:
    case ESP_GATTS_CLOSE_EVT:
    case ESP_GATTS_LISTEN_EVT:
    case ESP_GATTS_CONGEST_EVT:
    case ESP_GATTS_CREAT_ATTR_TAB_EVT:
    case ESP_GATTS_SET_ATTR_VAL_EVT:
    case ESP_GATTS_SEND_SERVICE_CHANGE_EVT:
    */
    default:
        ESP_LOGI(BT_TAG, "GATT event %d", event);
        break;

    }
}



/*  Initialize the actual BLE service.
 */

void ssds_ble_init() {
    esp_err_t ret;

    /* Initialize a bluetooth config struct */
    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();


    /* Init and enable bluetooth controller */
    ret = esp_bt_controller_init(&bt_cfg);
    if (ret) {
        ESP_LOGE(BT_TAG, "%s initialize controller failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }

    ret = esp_bt_controller_enable(ESP_BT_MODE_BLE);
    if (ret) {
        ESP_LOGE(BT_TAG, "%s enable controller failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }


    /* Init and enable Bluedroid stack */
    ret = esp_bluedroid_init();
    if (ret) {
        ESP_LOGE(BT_TAG, "%s init bluetooth failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }
    ret = esp_bluedroid_enable();
    if (ret) {
        ESP_LOGE(BT_TAG, "%s enable bluetooth failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }


    /* Register GATT callback */
    ret = esp_ble_gatts_register_callback (gatts_event_handler);
    if (ret)
    {
        ESP_LOGE(BT_TAG, "GATT callback register error, error code = %x", ret);
        return;
    }

    /* Register GAP callback */
    ret = esp_ble_gap_register_callback (gap_event_handler);
    if (ret)
    {
        ESP_LOGE(BT_TAG, "GAP callback register error, error code = %x", ret);
        return;
    }

    /* Register singular app */
    ret = esp_ble_gatts_app_register (SSDS_APP_ID);
    if (ret){
        ESP_LOGE(BT_TAG, "GATT app register error, error code = %x", ret);
        return;
    }


    /* Setting the MTU for the BT controller */
    esp_err_t local_mtu_ret = esp_ble_gatt_set_local_mtu(500);
    if (local_mtu_ret){
        ESP_LOGE(BT_TAG, "set local  MTU failed, error code = %x", local_mtu_ret);
    }

    ret = esp_bt_sleep_enable();
    if (ret){
        ESP_LOGE(BT_TAG, "Bluetooth sleep enable error, error code = %x", ret);
        return;
    }

    return;
}


/*  Deinitialize the whole BLE service.
 *  Only to be called in preparation for
 *  deep sleep.
 */

void ssds_ble_deinit()
{
    ESP_LOGI(BT_TAG, "Disconnecting app");
    esp_ble_gatts_close(ssds_profile.gatts_if, ssds_profile.conn_id);
    ESP_LOGI(BT_TAG, "Disabling bluedroid");
    esp_bluedroid_disable();
    //esp_bluedroid_deinit();
    ESP_LOGI(BT_TAG, "Disabling BT controller");
    esp_bt_controller_disable();
    //esp_bt_controller_deinit();
}

void ssds_ble_gatt_advertise(){
    esp_bt_controller_enable(ESP_BT_MODE_BLE);
    esp_bluedroid_enable();
    esp_ble_gatts_register_callback (gatts_event_handler);
    esp_ble_gap_register_callback (gap_event_handler);
    esp_ble_gatts_app_register (SSDS_APP_ID);
    esp_ble_gatt_set_local_mtu(500);
    esp_ble_gap_start_advertising(&adv_params);
}
