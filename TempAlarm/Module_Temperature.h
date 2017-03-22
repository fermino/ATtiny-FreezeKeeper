#ifndef TemperatureAlarm_h
#define TemperatureAlarm_h

	#include "TempAlarmModule.h"

	#include <avr/eeprom.h>
	#include <OneWire.h>

	#include "Configuration.h"
	#include "config/Temperature.h"

	class TemperatureAlarm : public TempAlarmModule
	{
		//using TempAlarmModule::TempAlarmModule;

		private:
			bool AlarmOn = false;

			OneWire OW = OneWire(TEMPERATURE_ONE_WIRE_BUS);
			byte SensorAddress[TEMPERATURE_SENSOR_AMOUNT][8];

			/*bool Enabled = false;
			bool DirectionUp = true;

			int8_t Threshold;

*/
			// Timers

			uint32_t UpdateTemperature_StartedAt = 0;
			void UpdateTemperature();

			/*uint8_t CurrentStatusCharacter = 1;
			uint32_t UpdateStatus_StartedAt = 0;
			void UpdateStatus();

			void UpdateDirection();*/

		public:
			TemperatureAlarm(LCD_CLASS* L, OneWireSwitches<SWITCHES_AMOUNT, SWITCHES_INPUT_PIN>* S);

			void init();
			void loop();

			bool isAlarmOn();
	};

#endif