/*
 * GPS.h
 *
 *  Created on: Jul 18, 2014
 *      Author: Grzegorz Wojcik
 */

#ifndef GPS_H_
#define GPS_H_

/*** Volatile variables ***/
volatile char GPS_DataFrame[100];		//Should start with '$', end with 0x0A
volatile uint8_t GPS_flag;				//GPS_flag is initialized with 0 during GPS_VariablesInit() execution

/*** Initialization functions ***/
void GPS_GPIOinit(void);
void GPS_USARTinit(void);
typedef struct{
		uint8_t Longitude_degrees;	/* Szerokosc geograficzna DMM [DegreesMinutesMinutes] */
		uint8_t Latitude_degrees;	/* Dlugosc geograficzna DMM [DegreesMinutesMinutes] */
		float Longitude_minutes;
		float Latitude_minutes;

		float Latitude;
		float Longitude;
		float Altitude;		/* Above mean sea level [meters] */

		/* UTC Time */
		uint8_t Time_hours;
		uint8_t Time_minutes;
		uint8_t Time_seconds;

}GPS;
GPS GPS_StructInit(void);


// 50 17.30118, 18 40.66723


/*** Functions ***/
void GPS_ClearDataFrame(void);
void GPS_Parse(GPS* GPS_Structure);
void GPS_ParseGGA(GPS* GPS_Structure);
void GPS_ProcessGGA(GPS* GPS_Structure);




#endif /* GPS_H_ */
