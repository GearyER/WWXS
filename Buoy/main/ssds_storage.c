/* SSDS Storage
*  the program is responsible for managing the storage and retrieval of datapoints, 
*  such as environmental sensor readings, and provides functionalities for initializing, clearing, writing, iterating, 
*  and displaying the stored data.
*/


#include "ssds_global.h"


static const char *STORAGE_TAG = "STORE";
//const esp_partition_t *partition;
static unsigned active_iterator = 0;


esp_err_t init_storage()
{
    /* NVS */
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }

    ESP_ERROR_CHECK (err);

    return ESP_OK;
}


void wipe_nvs_storage()
{
    ESP_ERROR_CHECK(nvs_flash_erase());
}


esp_err_t reset_storage()
{
    nvs_handle handle;
    const esp_partition_t *partition;

    /* Resetting NVS values */
    ESP_ERROR_CHECK(nvs_open("nvs", NVS_READWRITE, &handle));

    uint32_t dp_offset = 0;
    ESP_ERROR_CHECK(nvs_set_u32(handle, "dp_offset", dp_offset));

    uint32_t dp_count = 0;
    ESP_ERROR_CHECK(nvs_set_u32(handle, "dp_count", dp_count));

    /* Closing NVS */
    ESP_ERROR_CHECK(nvs_commit(handle));
    nvs_close(handle);
    
    partition = esp_partition_find_first(0x40, ESP_PARTITION_SUBTYPE_ANY, "storage");
    assert(partition != NULL);

    ESP_ERROR_CHECK(esp_partition_erase_range(partition, 0, partition->size));

    return ESP_OK;
}


esp_err_t write_dp (datapoint_t *dp)
{
    const esp_partition_t *partition;
    nvs_handle handle;

    // Open NVS for retreiving offset and count

    ESP_ERROR_CHECK( nvs_open("nvs", NVS_READWRITE, &handle) );

    uint32_t dp_offset = 0;
    ESP_ERROR_CHECK( nvs_get_u32(handle, "dp_offset", &dp_offset) );

    uint32_t dp_count = 0;
    ESP_ERROR_CHECK( nvs_get_u32(handle, "dp_count", &dp_count) );

    ESP_LOGI(STORAGE_TAG, "Writing at offset %x", dp_offset);

    // Write new datapoint
    partition = esp_partition_find_first(0x40, ESP_PARTITION_SUBTYPE_ANY, "storage");
    assert(partition != NULL);

    ESP_ERROR_CHECK( esp_partition_write(partition, dp_offset, dp, sizeof(datapoint_t)) );
    dp_count = dp_count + 1;
    dp_offset = dp_offset + sizeof(datapoint_t);

    // Update and close NVS
    ESP_ERROR_CHECK( nvs_set_u32(handle, "dp_offset", dp_offset) );
    ESP_ERROR_CHECK( nvs_set_u32(handle, "dp_count", dp_count) );

    ESP_ERROR_CHECK( nvs_commit(handle) );
    nvs_close(handle);

    return ESP_OK;
}


dp_iterator_t* get_dp_iterator (uint32_t curr_count, uint32_t curr_offset)
{
    const esp_partition_t *partition;
    nvs_handle handle;

    ESP_ERROR_CHECK( nvs_open("nvs", NVS_READWRITE, &handle) );

    uint32_t dp_count = 0;
    ESP_ERROR_CHECK( nvs_get_u32(handle, "dp_count", &dp_count) );

    ESP_ERROR_CHECK( nvs_commit(handle) );
    nvs_close(handle);

    partition = esp_partition_find_first(0x40, ESP_PARTITION_SUBTYPE_ANY, "storage");
    assert(partition != NULL);

    dp_iterator_t *iterator;
    iterator = malloc(sizeof(dp_iterator_t));
    iterator->curr_dp = curr_count;
    iterator->curr_offset = curr_offset;
    iterator->dp_count = dp_count;
    iterator->partition = partition;

    active_iterator = 1;

    return iterator;
}


void get_next_dp (dp_iterator_t *iterator, datapoint_t *data)
{
    if (iterator->curr_dp < iterator->dp_count && active_iterator)
    {
        ESP_LOGI(STORAGE_TAG, "Reading from offset %x", iterator->curr_offset);
        ESP_ERROR_CHECK ( esp_partition_read(iterator->partition, iterator->curr_offset, data, sizeof(datapoint_t)) );
        iterator->curr_dp = iterator->curr_dp + 1;
        iterator->curr_offset = iterator->curr_offset + sizeof(datapoint_t);
    }
    else
    {   /*TURB2 TESTING*/
        *data = (datapoint_t){0,0,0,0,0,0,0,0,0};
    }
}


void close_dp_iterator (dp_iterator_t *iterator)
{
    if (active_iterator)
    {
        free(iterator);
        active_iterator=0;
    }
    return;
}


esp_err_t set_status(uint8_t status)
{
    nvs_handle handle;
    esp_err_t ret;

    ret = nvs_open("nvs", NVS_READWRITE, &handle);
    if (ret != ESP_OK){
        return ret;
    }

    ret = nvs_set_u8(handle, status_key, status);
    if (ret != ESP_OK){
        return ret;
    }

    nvs_commit(handle);
    nvs_close(handle);

    return ESP_OK;
}


uint8_t get_status()
{
    uint8_t return_status = 0;
    uint8_t status;
    nvs_handle handle;
    esp_err_t ret;

    ret = nvs_open("nvs", NVS_READWRITE, &handle);
    if (ret != ESP_OK){
        return_status = -1;
    }

    if (return_status == 0)
    {
        ret = nvs_get_u8(handle, status_key, &status);
        if (ret != ESP_OK){
            return_status = -1;
        }
        nvs_commit(handle);
        nvs_close(handle);
    }

    if (return_status == 0){
        return status;
    }
    else {
        return return_status;
    }
}



void print_datapoint(datapoint_t *data)
{
    printf("Time: %u\n", (unsigned)data->tv_sec);
    printf("\ttemp1: %u\n", data->temp1);
    printf("\ttemp2: %u\n", data->temp2);
    printf("\ttemp3: %u\n", data->temp3);
    printf("\tsalinity1: %u\n", data->salinity1);
    printf("\tsalinity2: %u\n", data->salinity2); /*ADDED ANOTHER SALINITY*/
    printf("\tlight: %u\n", data->light);
    printf("\tturbidity1: %u\n", data->turbidity1);
    printf("\tturbidity2: %u\n", data->turbidity2);
    printf("\n");
}
