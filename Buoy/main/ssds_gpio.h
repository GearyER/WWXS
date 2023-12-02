#ifndef __SSDS_GPIO
#define __SSDS_GPIO

#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "esp_err.h"

/* 
 * Initializes the GPIO module for data collection.
 *
 * Arguments: none
 *
 * Returns:
 *   ESP_OK: initialization successful.
 *   ESP_FAIL: GPIO was already initialized prior to call to init_gpio.
 */
esp_err_t init_gpio();

/* 
This function is to convert conductivity values to salinity values.
Arguments:
    temp is the temperature value closest to the water sensor (Salinity).
    cond is the water sensor (Salinity) value.

*/
float conductivityConvSalinity(float temp, float cond);

/*
This function is to be able to use delay between gathering data
Arguments:
    number_of_seconds is the number of seconds to delay
*/
void delay(int number_of_seconds);

/* 
 * Collects one datapoint and stores it in the address pointed to by dp.
 * 
 * Arguments:
 *  dp: A pointer to an already allocated datapoint_t.
 *
 * Returns: 
 *   ESP_OK: data collection successful.
 *   ESP_FAIL: GPIO was not initialized prior to call to gather_data.
 */
esp_err_t gather_data(datapoint_t* dp);


/*
This method takes the raw reading from the temperature sensor so it can convert it into
celsius.
Parameters:
	Attenuation range (chars_6_db): esp_adc_cal_characteristics_t
Return:
	Temperature Reading: float
*/
float get_raw_data_temp_convert_to_celsius(esp_adc_cal_characteristics_t chars_6_dB, uint32_t temp_raw);

/*
This method turn on the water sensor and takes the raw reading from the water sensor so it
can call the other function to converting conductivity to salinity and then turn off.
Temperature will need to be the closest to the water sensor.
Parameters:
	Attenuation range (chars_11_db): esp_adc_cal_characteristics_t
	Temperature: float
	GPIO Pin: int
Return:
	Salinity Reading: float
*/
float get_raw_data_conductivity_convert_to_salinity(esp_adc_cal_characteristics_t chars_11_dB, float temp_reading, int gpio_pin, adc1_channel_t salin_sensor);

/*
This method takes the raw reading from the light sensor so it can convert it into
lux.
Parameters:
	Attenuation range (chars_11_db): esp_adc_cal_characteristics_t
Return:
	Light Reading: float
*/
float get_raw_data_light_convert_to_lux(esp_adc_cal_characteristics_t chars_11_dB, uint32_t light_raw);

/*
This method takes the raw reading from the turbidity sensor so it can convert it into
ntu.
Parameters:
	Attenuation range (chars_6_db): esp_adc_cal_characteristics_t
Return:
	Turbidity Reading: float
*/
float get_raw_data_turbidity_convert_to_ntu(esp_adc_cal_characteristics_t chars_6_dB, uint32_t turbd_raw);

/* 
This prints the result after gathering all the sample and getting the average.
Add more parameters as needed based on how many sensor you have.
Parameters:
	Temperature (temp1, temp2, temp3): float
	Turbidity (turbd): float
	Light (light1): float
	Salinity (salin1): float
Return:
	N/A
*/
void esp_logi_result(float temp1, float temp2, float temp3, float turbd1, float turbd2, float light1, float salin1, float salin2);


#endif
