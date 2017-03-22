#ifndef TempAlarm_Temperature_Configuration_h
#define TempAlarm_Temperature_Configuration_h
	#include <inttypes.h>

	/**
	 * Module: Temperature
	 * ===================
	 * 
	 * This module is a temperature-triggered alarm, it can alert you
	 * when something reachs the given temperature. 
	 * 
	 * The sensor is a DS18B20, but the DS18S20 and the DS1820 should work too. 
	 * It is connected through a OneWire bus. 
	 */

		// How many sensors are connected? Be sure to not exceed LCD_ROWS
		#define TEMPERATURE_SENSOR_AMOUNT 2

		// Between -127 and 127 (as Threshold is an 8-bit integer)
		#define TEMPERATURE_HIGHEST_THRESHOLD 50
		#define TEMPERATURE_LOWEST_THRESHOLD -50
		// Default threshold if the one stored in the EEPROM is not valid
		#define TEMPERATURE_DEFAULT_THRESHOLD 20

	/**
	 * LCD configuration
	 */

		#define TEMPERATURE_DEGREE_CHAR ((char) 223)

	/**
	 * OneWireSwitches configuration
	 * 
	 * ID: The index of the resistor configured in OneWireSwitches, which
	 * is tied to the switch we want to control. 
	 * THRESHOLD: The shortest pulse detected as a press by the firmware. 
	 * DELAY: Time (ms) to wait after the press without reading any button. 
	 */

		#define TEMPERATURE_BUTTON_DIRECTION_ID 0
		#define TEMPERATURE_BUTTON_DIRECTION_THRESHOLD 50
		#define TEMPERATURE_BUTTON_DIRECTION_DELAY 100

		#define TEMPERATURE_BUTTON_MINUS_ID 1
		#define TEMPERATURE_BUTTON_MINUS_THRESHOLD 20
		#define TEMPERATURE_BUTTON_MINUS_DELAY 20

		#define TEMPERATURE_BUTTON_PLUS_ID 2
		#define TEMPERATURE_BUTTON_PLUS_THRESHOLD 20
		#define TEMPERATURE_BUTTON_PLUS_DELAY 20

		#define TEMPERATURE_BUTTON_STARTSTOP_ID 3
		#define TEMPERATURE_BUTTON_STARTSTOP_THRESHOLD 50
		#define TEMPERATURE_BUTTON_STARTSTOP_DELAY 100

	/**
	 * EEPROM
	 * 
	 * The EEPROM can store information for a loong time. 
	 * Here, we define in which direction will we store the 
	 * last used temperature and alarm direction. 
	 */

		#define TEMPERATURE_EEPROM_ADDRESS_THRESHOLD 0
		#define TEMPERATURE_EEPROM_ADDRESS_DIRECTION 1

	/**
	 * DS18B20 / DS18S20 / DS1820
	 */

		// It disturbs the SPI signals when programming if tied to some SPI pin
		#define TEMPERATURE_ONE_WIRE_BUS 3

		// The temperature conversion takes about 750ms, so, I think this is pretty reasonable
		#define TEMPERATURE_CONVERSION_TIME 775
#endif