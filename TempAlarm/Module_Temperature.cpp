/**
 * Thanks for helping me to optimize the code (more then 1.3K flash saved!) to
 * 
 * https://edwardmallon.wordpress.com/2014/03/12/using-a-ds18b20-temp-sensor-without-a-dedicated-library/
 * http://bildr.org/2011/07/ds18b20-arduino/
 */

	#include "Module_Temperature.h"

	TemperatureAlarm::TemperatureAlarm(LCD_CLASS* L, OneWireSwitches<SWITCHES_AMOUNT, SWITCHES_INPUT_PIN>* S)
	: TempAlarmModule(L, S)
	{}

	void TemperatureAlarm::init()
	{
		// Print template

		for(uint8_t i = 0; i < TEMPERATURE_SENSOR_AMOUNT; i++)
		{
			float r = 10.4f;
			LCD->setCursor(0, i);
			LCD->print('#');
			LCD->print((uint32_t) i + 1, DEC);
			LCD->print(": ");
		}

		for(uint8_t i = 0; i < TEMPERATURE_SENSOR_AMOUNT; )
		{
			// If there are no devices left in the bus, stop 
			if(!OW.search(SensorAddress[i]))
				break;

			// If the device is a recognized one (temp sensor), keep counting
			if(SensorAddress[i][0] == 0x10 || SensorAddress[i][0] == 0x28)
				i++;
		}

		
/*		UpdateDirection();

		// Find sensor's address
		OW.search(SensorAddress);

		// Get last used data from EEPROM

		Threshold = eeprom_read_byte(TEMPERATURE_EEPROM_ADDRESS_THRESHOLD);
		if(Threshold < TEMPERATURE_LOWEST_THRESHOLD || Threshold > TEMPERATURE_HIGHEST_THRESHOLD)
			Threshold = TEMPERATURE_DEFAULT_THRESHOLD;

		DirectionUp = eeprom_read_byte(TEMPERATURE_EEPROM_ADDRESS_DIRECTION);
		if(DirectionUp != false && DirectionUp != true)
			DirectionUp = true;*/
	}

	void TemperatureAlarm::loop()
	{
		// Timers

		if((millis() - UpdateTemperature_StartedAt) >= TEMPERATURE_CONVERSION_TIME)
		{
			UpdateTemperature_StartedAt = millis();

			UpdateTemperature();
		}
		/*if((millis() - UpdateStatus_StartedAt) >= TEMPERATURE_STATUS_UPDATE_DELAY)
		{
			UpdateStatus_StartedAt = millis();

			UpdateStatus();
		}

		// Buttons

		// ReadPulse should not return anything higher than Timeout, but well, using >= instead ==
		// has no effect over flash/memory usage, so, it can avoid further problems if code gets changed

		if(Switches->readKeyPulse(TEMPERATURE_BUTTON_DIRECTION_ID, TEMPERATURE_BUTTON_DIRECTION_THRESHOLD) >= TEMPERATURE_BUTTON_DIRECTION_THRESHOLD)
		{
			DirectionUp = !DirectionUp;
			UpdateDirection();

			delay(TEMPERATURE_BUTTON_DIRECTION_DELAY);
		}
		else if(Switches->readKeyPulse(TEMPERATURE_BUTTON_MINUS_ID, TEMPERATURE_BUTTON_MINUS_THRESHOLD) >= TEMPERATURE_BUTTON_MINUS_THRESHOLD)
		{
			if(Threshold > TEMPERATURE_LOWEST_THRESHOLD)
				Threshold--;

			delay(TEMPERATURE_BUTTON_MINUS_DELAY);
		}
		else if(Switches->readKeyPulse(TEMPERATURE_BUTTON_PLUS_ID, TEMPERATURE_BUTTON_PLUS_THRESHOLD) >= TEMPERATURE_BUTTON_PLUS_THRESHOLD)
		{
			if(Threshold < TEMPERATURE_HIGHEST_THRESHOLD)
				Threshold++;

			delay(TEMPERATURE_BUTTON_PLUS_DELAY);
		}
		else if(Switches->readKeyPulse(TEMPERATURE_BUTTON_STARTSTOP_ID, TEMPERATURE_BUTTON_STARTSTOP_THRESHOLD) >= TEMPERATURE_BUTTON_STARTSTOP_THRESHOLD)
		{
			AlarmOn = false;

			Enabled = !Enabled;

			// Store the temperature and the direction in the EEPROM
			// We do this here and not when the user changes the temperature to preserve the EEPROM life cycle
			eeprom_update_byte(TEMPERATURE_EEPROM_ADDRESS_THRESHOLD, Threshold);
			eeprom_update_byte(TEMPERATURE_EEPROM_ADDRESS_DIRECTION, DirectionUp);

			delay(TEMPERATURE_BUTTON_STARTSTOP_DELAY);
		}

		// Show Threshold temperature

		LCD->setCursor(14, 1);
		if(Threshold >= 0 && Threshold < 100)
			LCD->print(' ');
		LCD->print(Threshold);
		LCD->print(TEMPERATURE_DEGREE_CHAR);
		LCD->print(' ');*/
	}

	void TemperatureAlarm::UpdateTemperature()
	{
		for(uint8_t i = 0; i < TEMPERATURE_SENSOR_AMOUNT; i++)
		{
			float Temperature = -127.0f;

			if(OW.reset())
			{
				// Select the device and read the scratchpad
				OW.select(SensorAddress[i]);
				OW.write(0xBE); // Read Scratchpad [BEh] command

				uint8_t LSB = OW.read();

				Temperature = ((OW.read() << 8) | LSB); 
				Temperature = Temperature / 16;

				if(OW.reset())
				{
					// Start another conversion; will be used in the next function call
					OW.select(SensorAddress[i]);
					OW.write(0x44); // Convert T [44h] command
				}
			}

			LCD->setCursor(4, i);

			if(Temperature != -127.0f)
			{
				if(Temperature >= 0.0f && Temperature < 100.0f)
					LCD->print(' ');

				LCD->print(Temperature, 1);
				LCD->print(TEMPERATURE_DEGREE_CHAR);
				LCD->print(' ');

				/*
					// If the temperature is above or below the given temperature, activate the alarm
					if(Enabled && ((DirectionUp == 0 && Temperature <= Threshold) || (DirectionUp == 1 && Temperature >= Threshold)))
						AlarmOn = true;
					else
						AlarmOn = false;
				*/
			}
			else
			{
				LCD->print(" ----- ");

				//if(Enabled)
				//	AlarmOn = true;
			}
		}
	}

	bool TemperatureAlarm::isAlarmOn()
	{ return AlarmOn; }