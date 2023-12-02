#ifndef __SSDS_STORAGE
#define __SSDS_STORAGE

#include <sys/time.h>
#include "esp_system.h"
#include "esp_partition.h"
#include "nvs_flash.h"
#include "nvs.h"

#define status_key "board_status"


/* Structs */
typedef struct {
    time_t tv_sec;
    uint16_t temp1;
    uint16_t temp2;
    uint16_t temp3;
    uint16_t salinity1; //added another salinity
    uint16_t salinity2;
    uint16_t light;
    uint16_t turbidity1; //adding another turb
    uint16_t turbidity2;
} datapoint_t;

typedef struct {
    const esp_partition_t *partition;
    uint32_t dp_count;
    uint32_t curr_dp;
    uint32_t curr_offset;
} dp_iterator_t;

/* Function declarations */

/* 
 * Procedure: init_storage
 *  Sets up non-volatile storage to be used for storing key-value pairs
 *  and opens storage partition
 *
 * Arguments: none
 *
 * Returns:
 *  ESP_OK: NVS and partition storage were initialized
 */
esp_err_t init_storage ();

/* 
 * Procedure: reset_storage
 *  Erases all datapoint storage and resets
 *  dp_count and dp_offset to 0
 *
 * Arguments: none
 *
 * Returns:
 *  ESP_OK: Storage was reset
 *  Other: Errors from one of the contained functions
 */
esp_err_t reset_storage();

/*
 * Procedure: write_dp
 * Arguments:
 *  datapoint_t *dp: Pointer to the datapoint to be inserted
 *
 * Returns:
 *  ESP_OK: Datapoint was written to storage
 */
esp_err_t write_dp (datapoint_t *dp);

/*
 * Function: get_dp_iterator
 *  Creates a struct used to iterate through the datapoints
 *  in storage
 * 
 * Arguments:
 *  curr_offset: Offset into the storage partition to start from,
 *      usually assumed to be 0
 *  curr_count: Data point count to start from, usually 0
 *
 * Returns:
 *  Pointer to a dp_iterator struct, or NULL on error
 */
dp_iterator_t* get_dp_iterator (uint32_t curr_count, uint32_t curr_offset);

/*
 * Function: get_next_dp
 *  Using the passed datapoint iterator, returns a
 *  pointer to the next datapoint_t stored in flash
 *
 * Arguments:
 *  iterator: A non-null pointer to a dp_iterator_t struct
 *  data: A pointer to a malloc'd datapoint_t struct
 * 
 * Returns:
 *  None
 */
void get_next_dp (dp_iterator_t *iterator, datapoint_t *data);

/*
 * Function: close_dp_iterator
 *  Frees a datapoint iterator struct in memory
 *
 * Arguments:
 *  iterator: A non-null pointer to a dp_iterator_t struct
 * 
 * Returns:
 *  Nothing
 */
void close_dp_iterator (dp_iterator_t *iterator);

/*
 * Function: print_datapoint
 *	Prints out a datapoint struct in an easy to read
 *	format to stdout
 *
 * Arguments:
 * 	data: Non-null pointer to a datapoint_t struct
 *
 * Returns:
 *	Nothing
 */
void print_datapoint(datapoint_t *data);


/*
 * Function: set_status
 *  Sets the board status in NVS storage for retrieval
 *  at a later date.
 *  Requires that NVS is already initialized
 *
 * Arguments:
 *  status: The new system status
 *
 * Returns:
 *  ESP_OK: status was updated
 *  Other error if something went wrong
 */
esp_err_t set_status(uint8_t status);

/*
 * Function: get_status
 *  Reads the system status out of NVS
 *  Requires that NVS already is initialized
 *
 * Arguments:
 *  None
 *
 * Returns:
 *  System status, or -1 if there was an error
 */
uint8_t get_status();

#endif /* __SSDS_STORAGE */