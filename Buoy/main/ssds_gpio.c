#include "ssds_global.h"
#include "sensor_conversion.c"

//All ESP32s run their ADC at 1.1V by default, but this can vary by chip
#define DEFAULT_VREF    1100

//#define NO_OF_SAMPLES   64          			//Multisampling
#define NO_OF_SAMPLES    20
#define SSDS_ADC_WIDTH	ADC_WIDTH_BIT_12 

#define TEMP1_CHANNEL 		ADC_CHANNEL_0
#define TEMP2_CHANNEL 		ADC_CHANNEL_1
#define TEMP3_CHANNEL 		ADC_CHANNEL_2
#define SALINITY1_CHANNEL	ADC_CHANNEL_5
#define SALINITY2_CHANNEL	ADC_CHANNEL_6
#define LIGHT1_CHANNEL 		ADC_CHANNEL_4
#define TURBD1_CHANNEL		ADC_CHANNEL_3

#define GPIO_TAG "GPIOW"

static uint8_t adc_configured = 0;

esp_err_t init_gpio()
{
	if (adc_configured){
		ESP_LOGW(GPIO_TAG, "ADC already configured");
		return ESP_FAIL;
	}
	adc1_config_width(SSDS_ADC_WIDTH);
	// these attenuation values were attained primarily through trial and error
	// TODO: Look into "ADC calibration" API so we can do a bit better than trial and error attenuation values
	adc1_config_channel_atten(TEMP1_CHANNEL, ADC_ATTEN_DB_6);
	adc1_config_channel_atten(TEMP2_CHANNEL, ADC_ATTEN_DB_6);
	adc1_config_channel_atten(TEMP3_CHANNEL, ADC_ATTEN_DB_6);
	adc1_config_channel_atten(SALINITY1_CHANNEL, ADC_ATTEN_DB_11);
	adc1_config_channel_atten(SALINITY2_CHANNEL, ADC_ATTEN_DB_11);
	adc1_config_channel_atten(LIGHT1_CHANNEL, ADC_ATTEN_DB_6);
	adc1_config_channel_atten(TURBD1_CHANNEL, ADC_ATTEN_DB_6);

  adc_configured = 1;

	return ESP_OK;
}

void delay(int number_of_seconds)
{
    // Converting time into milli_seconds
    int milli_seconds = 500 * number_of_seconds;
 
    // Storing start time
    clock_t start_time = clock();
 
    // looping till required time is not achieved
    while (clock() < start_time + milli_seconds);
}

/* Utilized formulas are as follows:
	* ADC to voltage (mV): voltage = adc_out * (attenuation_max_in / attenuation_max_out)
	* Temperature sensor voltage (mV) to degrees C: (voltage - 500) / 10
	* Salinity sensor: implemented.
	* Light sensor voltage (mV) to lux: Requires reference resistance.
	* Turbidity sensor voltage (mV) to NTU: turbidity = -1120.4(voltage^2) + 5742.3(voltage) - 4352.9
	*      NOTE: The above formula is a placeholder obtained from online references using the TSD-10, and will only
	*      provide an estimate of turbidity. It should only be used until a calibrated formula is obtained.
	*/

/* Refer to this link for attenuation ranges:
	* https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/adc.html
	* Note that the attenuation range for 11 dB is actually varies by board, and may not be consistent with this table
	*/


/* 
Some ideas to make the code look better... This will also help when adding more sensor to the prototype.
	1. Make more function where it will be called 
		Minium Function Parameters Requirement: &chars_6_db or &chars_11_db.
			RETURN VALUE: Reading of the sensor after conversion.
	2. Try to cut down the size of gather data code to make it easier to read and understand.
*/

esp_err_t gather_data(datapoint_t* dp)
{
    esp_adc_cal_characteristics_t chars_6_dB;
    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_6, SSDS_ADC_WIDTH, DEFAULT_VREF, &chars_6_dB);

	esp_adc_cal_characteristics_t chars_11_dB;
    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, SSDS_ADC_WIDTH, DEFAULT_VREF, &chars_11_dB);

	esp_adc_cal_characteristics_t chars_10_dB;
    esp_adc_cal_characterize(ADC_UNIT_1, ADC_WIDTH_10Bit, ADC_WIDTH_BIT_10 , DEFAULT_VREF, &chars_10_dB);

	float temp1 = 0;
	uint32_t temp1_raw = 0;
	float temp1_reading = 0;
	float temp2 = 0;
	uint32_t temp2_raw = 0;
	float temp3 = 0;
	uint32_t temp3_raw = 0;
	float light1 = 0;
	uint32_t light1_raw = 0;
	float turbd1 = 0;
	uint32_t turbd1_raw = 0;
	float salin1 = 0;
	float salin2 = 0;

	for (int i = 0; i < NO_OF_SAMPLES; i++) 
	{
		temp1_raw = adc1_get_raw((adc1_channel_t)TEMP1_CHANNEL);
		temp1_reading = get_raw_data_temp_convert_to_celsius(chars_6_dB, temp1_raw);
		temp1 += temp1_reading;

		temp2_raw = adc1_get_raw((adc1_channel_t)TEMP2_CHANNEL);
		temp2 += get_raw_data_temp_convert_to_celsius(chars_6_dB, temp2_raw);

		temp3_raw = adc1_get_raw((adc1_channel_t)TEMP3_CHANNEL);
		temp3 += get_raw_data_temp_convert_to_celsius(chars_6_dB, temp3_raw);
	
		light1_raw = adc1_get_raw((adc1_channel_t)LIGHT1_CHANNEL);
		light1 += get_raw_data_light_convert_to_lux(chars_11_dB, light1_raw);

		turbd1_raw = adc1_get_raw((adc1_channel_t)TURBD1_CHANNEL);
		turbd1 += get_raw_data_turbidity_convert_to_ntu(chars_6_dB, turbd1_raw);

		salin1 += get_raw_data_conductivity_convert_to_salinity(chars_11_dB, temp1_reading, 25, (adc1_channel_t)SALINITY1_CHANNEL);
		salin2 += get_raw_data_conductivity_convert_to_salinity(chars_11_dB, temp1_reading, 23, (adc1_channel_t)SALINITY2_CHANNEL);
		esp_task_wdt_reset();
	}

	temp1 /= NO_OF_SAMPLES;
	temp2 /= NO_OF_SAMPLES;
	temp3 /= NO_OF_SAMPLES;
	salin1 /= NO_OF_SAMPLES;
	light1 /= NO_OF_SAMPLES;
	turbd1 /= NO_OF_SAMPLES;

	dp->temp1 = temp1;
	dp->temp2 = temp2;
	dp->temp3 = temp3;
	dp->salinity = salin1;
	dp->light = light1;
	dp->turbidity = turbd1;

	esp_logi_result(temp1, temp2, temp3, turbd1, light1, salin1);

	struct timeval tv;
	gettimeofday(&tv, NULL);

	dp->tv_sec = tv.tv_sec;

	ESP_LOGI(GPIO_TAG, "Time: %ld", dp->tv_sec);

	return ESP_OK;
}

/*
This method takes the raw reading from the temperature sensor so it can convert it into
celsius.
Parameters:
	Attenuation range (chars_6_db): esp_adc_cal_characteristics_t
	Raw Temperature: uint_32_t
Return:
	Temperature Reading: float
*/
float get_raw_data_temp_convert_to_celsius(esp_adc_cal_characteristics_t chars_6_dB, uint32_t temp_raw)
{
    uint32_t temp_voltage = 0;
	float temp_reading = 0;
	temp_voltage = esp_adc_cal_raw_to_voltage(temp_raw, &chars_6_dB);
	temp_reading = voltagesConvertTemperature(temp_voltage);
	printf("Temp_reading: %f\n", temp_reading);
	return temp_reading;
}

/*
This method turn on the water sensor and takes the raw reading from the water sensor so it
can call the other function to converting conductivity to salinity and then turn off.
Temperature will need to be the closest to the water sensor.
Parameters:
	Attenuation range (chars_11_db): esp_adc_cal_characteristics_t
	Temperature: float
	GPIO Pin: int
	Salin Sensor Number: int
Return:
	Salinity Reading: float
*/
float get_raw_data_conductivity_convert_to_salinity(esp_adc_cal_characteristics_t chars_11_dB, float temp_reading, int gpio_pin, adc1_channel_t salin_sensor)
{
    uint32_t salin_voltage = 0;
    float salin_reading = 0;
	uint32_t salin_raw = 0;	
	//Turn off salinity when not in use
	gpio_set_direction(gpio_pin, GPIO_MODE_OUTPUT);
	gpio_set_level(gpio_pin, 1);
	delay(1);
	
	
	salin_raw = adc1_get_raw(salin_sensor);
	
	salin_voltage = esp_adc_cal_raw_to_voltage(salin_raw, &chars_11_dB);
	gpio_set_level(gpio_pin, 0);
	delay(1);
	/*
	Formula to converting conductivity to salinity.
	Using temp1 because that is the closest sensor to the water sensor.
	When deployed then make sure that the temp sensor is use is closest to the water sensor.
	*/
	salin_reading = conductivityConvSalinity(temp_reading, salin_voltage); // Salinity will need more work as we are using an unconventional sensor.
	return salin_reading;
}

/*
This method takes the raw reading from the light sensor so it can convert it into
lux.
Parameters:
	Attenuation range (chars_11_db): esp_adc_cal_characteristics_t
	Raw Light: uint_32_t
Return:
	Light Reading: float
*/
float get_raw_data_light_convert_to_lux(esp_adc_cal_characteristics_t chars_11_dB, uint32_t light_raw)
{
	uint32_t light_voltage = 0;
	float light_reading = 0;
	light_voltage = esp_adc_cal_raw_to_voltage(light_raw, &chars_11_dB);
	light_reading = voltagesConvertLight(light_voltage); // Unsure about reference resistance for ohm's law calculation.
	return light_reading;
}

/*
This method takes the raw reading from the turbidity sensor so it can convert it into
ntu.
Parameters:
	Attenuation range (chars_6_db): esp_adc_cal_characteristics_t
	Raw Turbidity: uint_32_t
Return:
	Turbidity Reading: float
*/
float get_raw_data_turbidity_convert_to_ntu(esp_adc_cal_characteristics_t chars_6_dB, uint32_t turbd_raw)
{
	uint32_t turbd_voltage = 0;
	float turbd_reading = 0;
	float check = 0;
	//for(int j =0; j < 800; j++)
	//{
		//turbd_voltage = esp_adc_cal_raw_to_voltage(turbd_raw, &chars_6_dB);
		//printf("Turbidity Raw: %d\n Turbidity Volt: %d\n", turbd_raw, turbd_voltage);
		//check += ((float)(turbd_raw/1023.0)) * 5.0;
	//}
	//check = check / 800;
	check = ((float)(turbd_raw/4095.0)) * 5.0;
	printf("Voltage: %f\n", check);
	if(check < 2.5)
	{
		return 3000.0;
	}
	else
	{
		voltagesConvertTurbidity(check);
		//printf("Turbidity: %f\n", turbd_reading);
	}
	return turbd_reading;
}

/* 
This prints the result after gathering all the sample and getting the average.
Add more parameters as needed based on how many sensor you have.
Parameters:
	Temperature (temp1, temp2, temp3): float
	Turbidity (turbd1): float
	Light (light1): float
	Salinity (salin1): float
Return:
	N/A
*/
void esp_logi_result(float temp1, float temp2, float temp3, float turbd1, float light1, float salin1)
{
	ESP_LOGI(GPIO_TAG, "temp1: %f", temp1);
    //ESP_LOGI(GPIO_TAG, "temp1_voltage: %f", temp1_voltage);
	ESP_LOGI(GPIO_TAG, "temp2: %f", temp2);
    //ESP_LOGI(GPIO_TAG, "temp2_voltage: %f", temp2_voltage);
	ESP_LOGI(GPIO_TAG, "temp3: %f", temp3);
    //ESP_LOGI(GPIO_TAG, "temp3_voltage: %f", temp3_voltage);
	ESP_LOGI(GPIO_TAG, "Salinity: %f", salin1);
    //ESP_LOGI(GPIO_TAG, "Light_raw: %d", light_raw);
	ESP_LOGI(GPIO_TAG, "Light: %f", light1);
    //ESP_LOGI(GPIO_TAG, "Light_voltage: %f", light_voltage);
	ESP_LOGI(GPIO_TAG, "Turbidity: %f", turbd1);	
}