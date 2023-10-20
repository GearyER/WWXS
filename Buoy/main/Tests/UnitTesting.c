/* 
Unit Testing for the Buoy Sensor
*/
#define Testing
#include <stdio.h>
#include <math.h>
#include "../sensor_conversion.c"

float one_decimal_point(float value);
void testing_salinity();
void testing_temperature();
void testing_turbidity();
void testing_light();

int main()
{
    testing_salinity();
    testing_temperature();
    testing_turbidity();
    testing_light();
            
    return 0;
}

/* Helper Method to convert float to a one decimal place*/
float one_decimal_point(float value)
{
    char str[20];
    sprintf(str, "%.1f", value);
    sscanf(str, "%f", &value);
    return value;
}

/*
Salinity TESTING!
Comparing result to https://www.fivecreeks.org/monitor/sal.shtml
Look at the excel document for real testing result using the temperature sensor
and salinity sensor.
*/
void testing_salinity()
{
    printf("------------------Initializing Test For Salinity Conversion------------------\n");

    float salinity = conductivityConvSalinity(23, 467.5);
    salinity = one_decimal_point(salinity);
    if(salinity == 2.6f)
    {
        printf("Passed Test 1!\n");
    }
    else
    {
        printf("Failed Test 2!\n");
    }
    salinity = conductivityConvSalinity(30, 657.9);
    salinity = one_decimal_point(salinity);
    if(salinity == 3.2f)
    {
        printf("Passed Test 2!\n");
    }
    else
    {
        printf("Failed Test 2!\n");
    }
    salinity = conductivityConvSalinity(15, 1566.9);
    salinity = one_decimal_point(salinity);
    if(salinity == 11.6f)
    {
        printf("Passed Test 3!\n");
    }
    else
    {
        printf("Failed Test 3!\n");
    }
}

/*Testing temperature voltage conversion to celsius*/
void testing_temperature()
{
    printf("\n------------------Initializing Test For Temperature Conversion------------------\n");
    float temperature = voltagesConvertTemperature(737);
    temperature = one_decimal_point(temperature);
    if(temperature == 23.7f)
    {
        printf("Passed Test 1!\n");
    }
    else
    {
        printf("Failed Test 1!\n");
    }
    temperature = voltagesConvertTemperature(740);
    temperature = one_decimal_point(temperature);
    if(temperature == 24.0f)
    {
        printf("Passed Test 2!\n");
    }
    else
    {
        printf("Failed Test 2!\n");
    }
    temperature = voltagesConvertTemperature(865);
    temperature = one_decimal_point(temperature);
    if(temperature == 36.5f)
    {
        printf("Passed Test 3!\n");
    }
    else
    {
        printf("Failed Test 3!\n");
    }
}

/*Testing Turbidity Conversion to ntu*/
void testing_turbidity()
{
    printf("\n------------------Initializing Test For Turbidity Conversion------------------\n");
    float turbidity = voltagesConvertTurbidity(2.5);
    turbidity = one_decimal_point(turbidity);
    if(turbidity == 2999.4f)
    {
        printf("Passed Test 1!\n");
    }
    else
    {
        printf("Failed Test 1!\n");
        printf("%f\n", turbidity);
    }
    turbidity = voltagesConvertTurbidity(3.5);
    turbidity = one_decimal_point(turbidity);
    if(turbidity == 2019.3f)
    {
        printf("Passed Test 2!\n");
    }
    else
    {
        printf("Failed Test 2!\n");
        printf("%f\n", turbidity);
    }
    turbidity = voltagesConvertTurbidity(4.2);
    turbidity = one_decimal_point(turbidity);
    if(turbidity == 0.0f)
    {
        printf("Passed Test 3!\n");
    }
    else
    {
        printf("Failed Test 3!\n");
    }
}

/*Testing Light Conversion to lux*/
void testing_light()
{
    printf("\n------------------Initializing Test For Light Conversion------------------\n");
    float light = voltagesConvertLight(166);
    light = one_decimal_point(light);
    if(light == 1.6f)
    {
        printf("Passed Test 1!\n");
    }
    else
    {
        printf("Failed Test 1!\n");
        printf("%f\n", light);
    }
    light = voltagesConvertLight(212);
    light = one_decimal_point(light);
    if(light == 1.8f)
    {
        printf("Passed Test 2!\n");
    }
    else
    {
        printf("Failed Test 2!\n");
        printf("%f\n", light);
    }
    light = voltagesConvertLight(580);
    light = one_decimal_point(light);
    if(light == 3.7f)
    {
        printf("Passed Test 3!\n");
    }
    else
    {
        printf("Failed Test 3!\n");
        printf("%f\n", light);
    }
}