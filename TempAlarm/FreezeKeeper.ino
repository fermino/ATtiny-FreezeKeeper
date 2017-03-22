/**
 * To do: 
 * Add PWM capabilities (instead of active buzzer, configurable)
 * Add Fahrenheit (maybe Kelvin?) capabilities
 * Software backlight control
 * Calculate remaining time
 * Add DHT22 (environment temperature)
 * Compile modules only when enabled
 */
	
	#ifndef cbi
		#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
	#endif
	#ifndef sbi
		#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
	#endif

	#include <avr/io.h>

	#include "Configuration.h"

 	// LCD libs
 	#ifndef USE_GROVE_LCD
 		#include <TinyWireM.h>
		#include <LiquidCrystal_I2C.h>
	#else
		#include <SerialLCD.h>
		#include <SoftwareSerial.h>
	#endif

	// One analog input, a lot of switches :P
	#include <OneWireSwitches.h>
	
	// Modules
	#include "Module_Temperature.h"

	// LCD
	#ifdef USE_GROVE_LCD
		// Same pins as I2C, #0 is TX, #2 is RX
		SerialLCD LCD(2, 0);
	#else
		LiquidCrystal_I2C LCD(LCD_I2C_ADDRESS, LCD_COLUMNS, LCD_ROWS);
	#endif

	// OneWireSwitches
	static const uint32_t SwitchesR1[SWITCHES_AMOUNT] SWITCHES_R1;
	OneWireSwitches<SWITCHES_AMOUNT, SWITCHES_INPUT_PIN> Switches(SwitchesR1, SWITCHES_R2, SWITCHES_READ_TOLERANCE);

	// Modules

	TemperatureAlarm M_Temperature(&LCD, &Switches);

	void setup()
	{
		// I2C interface and LCD init

		#ifdef USE_GROVE_LCD
			delay(100);
			LCD.begin();
		#else
			LCD.init();
		#endif

		#ifdef LCD_BACKLIGHT_ON
			LCD.backlight();
		#else
			LCD.noBacklight();
		#endif

		// Set buzzer pin as output
		// pinMode(BUZZER_PIN, OUTPUT);
		sbi(BUZZER_DDR, BUZZER_BIT);

		// Init modules

		M_Temperature.init();

		for(;;)
		{
			M_Temperature.loop();

			setAlarmStatus(M_Temperature.isAlarmOn());
		}
	}

	void setAlarmStatus(bool Enabled)
	{
		// digitalWrite(BUZZER_PIN, Enabled);

		if(Enabled)
			sbi(BUZZER_PORT, BUZZER_BIT);
		else
			cbi(BUZZER_PORT, BUZZER_BIT);
	}

	void loop()
	{}