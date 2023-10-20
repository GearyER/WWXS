#include <math.h>
#include <stdint.h>

//This function is to make correction to the salinity to get a more accurate reading.
float salinityCorrection(float salinity)
{
	/*
	Leaving this blank because some sensor are calibrated a little differently so 
	you will need to run some test can compare your buoy result with a salinity device.
	After that you will need to correct the turbidity to fit better with the device.
	*/
	return salinity;
}

//This function is to convert conductivity values to salinity values.
float conductivityConvSalinity(float temp, float cond)
{
	//Variable need for converting conductivity to salinity
	float r,ds;
	float a0,a1,a2,a3,a4,a5;
	float b0,b1,b2,b3,b4,b5;
	float c0,c1,c2,c3,c4;
	float r2;

	//Coefficients for the conversion equation
	a0 = 0.008;
	a1 = -0.1692;
	a2 = 25.3851;
	a3 = 14.0941;
	a4 = -7.0261;
	a5 = 2.7081;

	b0 = 0.0005;
	b1 = -0.0056;
	b2 = -0.0066;
	b3 = -0.0375;
	b4 = 0.0636;
	b5 = -0.0144;

	c0 = 0.6766097;
	c1 = 0.0200564;
	c2 = 0.0001104259;
	c3 = -0.00000069698;
	c4 = 0.0000000010031;

	float salin_reading = 0;
	cond = cond * 10; //Convert SI unit of conductivity 1 (Siemens per meter) to 10 (MilliSiemens per centimeter)
	r = cond / 42914;
	r /= (c0 + temp * (c1 + temp * (c2 + temp * (c3 + temp * c4))));
	r2 = sqrt(r);
	ds = b0 + r2 * (b1 + r2 * (b2 + r2 * (b3 + r2 * (b4 + r2 * b5))));
	ds *= ((temp - 15.0) / (1.0 + 0.0162 * (temp - 15.0)));
	salin_reading = a0 + r2 * (a1 + r2 * (a2 + r2 * (a3 + r2 * (a4 + r2 * a5)))) + ds; // Salinity will need more work as we are using an unconventional sensor.
	salin_reading = salinityCorrection(salin_reading); //Salinity correction formula
	return salin_reading;
}

//This function is to convert voltages from the temperature sensor to Celsius (C).
float voltagesConvertTemperature(uint32_t volt)
{
	return (float)(volt - 500) / 10;
}

//This function is to make correction to the turbidity to get a more accurate reading.
float turbidityCorrection(float turbidity)
{
	/*
	Leaving this blank because some sensor are calibrated a little differently so 
	you will need to run some test can compare your buoy result with a turbidity device.
	After that you will need to correct the turbidity to fit better with the device.
	*/
	return turbidity;
}

//This function is to convert analog raw voltages from the turbidity sensor to ntu.
float voltagesConvertTurbidity(float volt)
{
	volt = (float) -1120.4 * (volt * volt) + 5742.3 * volt - 4352.9 - 0.9000;
	//volt = (float) -2572.2 * (volt * volt) + 8700.5 * volt - 4352.9;
	float turbd = turbidityCorrection(volt); //Turbidity correction formula
	return turbd;
}

//This function is to convert voltages from the light sensor to lux.
float voltagesConvertLight(uint32_t volt)
{
	return (float)0.005167 * volt + 0.7315;
}