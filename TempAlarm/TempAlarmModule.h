#ifndef TempAlarmModule_h
#define TempAlarmModule_h

	#include "Configuration.h"

	#ifndef USE_GROVE_LCD
 		#include <TinyWireM.h>
		#include <LiquidCrystal_I2C.h>

		#define LCD_CLASS LiquidCrystal_I2C
	#else
		#include <SerialLCD.h>
		#include <SoftwareSerial.h>

		#define LCD_CLASS SerialLCD
	#endif

	#include <OneWireSwitches.h>

	class TempAlarmModule
	{
		protected: 
			LCD_CLASS* LCD;
			OneWireSwitches<SWITCHES_AMOUNT, SWITCHES_INPUT_PIN>* Switches;

			// Some utils

			void printZerofill(uint8_t Number);

		public:
			// Can't we mark this as final
			TempAlarmModule(LCD_CLASS* L, OneWireSwitches<SWITCHES_AMOUNT, SWITCHES_INPUT_PIN>* S);

			virtual void init() = 0;
			virtual void loop() = 0;

			virtual bool isAlarmOn() = 0;
	};

#endif