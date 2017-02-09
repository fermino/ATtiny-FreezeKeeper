	// EEPROM
	#include <avr/eeprom.h>

	// DS18B20 (DS18S20 and DS1820 should work too)
	#include <OneWire.h>
	#include <DallasTemperature.h>

	// This lib executes a callback every x ms
	#include <SimpleCallbackTimer.h>

	// OneWire and DallasTemperature
	OneWire OneWireBus(ONE_WIRE_BUS);
	DallasTemperature Sensors(&OneWireBus);

	// Timers
	SimpleCallbackTimer T_UpdateTemperature(500, F_UpdateTemperature);
	SimpleCallbackTimer T_UpdateAlarmStatus(BLINKING_CHARACTER_DELAY, F_UpdateAlarmStatus);

	// Other variables

	int8_t TemperatureThreshold;

	bool TemperatureAlarmEnabled = false;
	byte TemperatureAlarmDirection = 0;

	bool TemperatureAlarmOn = false;

	void InitTemperature()
	{
		// Print the template in the LCD
		PrintTemperatureTemplate();

		// Init the OneWire bus and DallasTemperature
		Sensors.begin();
		// We need to perform other things while the sensor making the conversion
		Sensors.setWaitForConversion(false);
		// Start a conversion, we want to have a stable value as soon as possible
		// Note: Sensors.requestTemperaturesByIndex(0) uses around 200 more bytes of flash
		Sensors.requestTemperatures();

		// Get last used data from EEPROM

		TemperatureThreshold = eeprom_read_byte(EEPROM_ADDRESS_TEMP_THRESHOLD);
		if(TemperatureThreshold < TEMP_LOWEST_THRESHOLD || TemperatureThreshold > TEMP_HIGHEST_THRESHOLD)
			TemperatureThreshold = TEMP_DEFAULT_THRESHOLD;

		TemperatureAlarmDirection = eeprom_read_byte(EEPROM_ADDRESS_TEMP_DIRECTION);
		if(TemperatureAlarmDirection != 0 && TemperatureAlarmDirection != 1)
			TemperatureAlarmDirection = 0;

		// Start the timers, this will update the temperature every 1 second
		T_UpdateTemperature.start();

		// This, will update the character that indicates if the alarm is enabled or not
		T_UpdateAlarmStatus.start();
	}

	void PrintTemperatureTemplate()
	{
		ClearLCDRow(0);
		ClearLCDRow(1);

		LCD.setCursor(0, 0);
		LCD.print("Alarm: ");
		LCD.setCursor(12, 0);
		LCD.print(LCD_DEGREE_CHAR);
		LCD.print('C');

		LCD.setCursor(0, 1);
		LCD.print("Temp: ");
		LCD.setCursor(12, 1);
		LCD.print(LCD_DEGREE_CHAR);
		LCD.print('C');
	}

	void TemperatureLoop()
	{
		T_UpdateTemperature.run();
		T_UpdateAlarmStatus.run();

		// ReadPulse should not return anything higher than Timeout, but well, using >= instead ==
		// has no effect over flash/memory usage, so, it can avoid further problems if code gets changed

		if(ReadPulse(CHANGE_DIR_ID, CHANGE_DIR_BUTTON_THRESHOLD) >= CHANGE_DIR_BUTTON_THRESHOLD) // If the user pressed ChangeDir
		{
			TemperatureAlarmDirection = !TemperatureAlarmDirection;

			delay(CHANGE_DIR_BUTTON_DELAY);
		}
		else if(ReadPulse(TEMP_MINUS_ID, TEMP_BUTTON_THRESHOLD) >= TEMP_BUTTON_THRESHOLD) // If the user pressed Temp--
		{
			// Limit temperature
			if(TemperatureThreshold > TEMP_LOWEST_THRESHOLD)
				TemperatureThreshold--;

			delay(TEMP_BUTTON_DELAY);
		}
		else if(ReadPulse(TEMP_PLUS_ID, TEMP_BUTTON_THRESHOLD) >= TEMP_BUTTON_THRESHOLD) // If the user pressed Temp++
		{
			// Limit temperature
			if(TemperatureThreshold < TEMP_HIGHEST_THRESHOLD)
				TemperatureThreshold++;

			delay(TEMP_BUTTON_DELAY);
		}
		else if(ReadPulse(START_STOP_ID, START_STOP_BUTTON_THRESHOLD) >= START_STOP_BUTTON_THRESHOLD) // If the user pressed Start/Stop
		{
			TemperatureAlarmEnabled = !TemperatureAlarmEnabled;

			TemperatureAlarmOn = false;

			// Store the temperature and the direction in the EEPROM
			// We do this here and not when the user changes the temperature to preserve the EEPROM life cycle
			eeprom_update_byte(EEPROM_ADDRESS_TEMP_THRESHOLD, TemperatureThreshold);
			eeprom_update_byte(EEPROM_ADDRESS_TEMP_DIRECTION, TemperatureAlarmDirection);

			delay(START_STOP_BUTTON_DELAY);
		}

		// Show TemperatureThreshold

		LCD.setCursor(7, 0);
		if(TemperatureThreshold >= 0 && TemperatureThreshold < 100)
			LCD.print(' ');
		LCD.print(TemperatureThreshold);
		LCD.print(' ');
	}

	void F_UpdateTemperature()
	{
		float Temperature = Sensors.getTempCByIndex(0);

		LCD.setCursor(6, 1);
		
		if(Temperature != DEVICE_DISCONNECTED_C)
		{
			LCD.print(Temperature, 1);
			LCD.print(' ');

			// If the temperature is above or below the given temperature, activate the alarm
			if(TemperatureAlarmEnabled && ((TemperatureAlarmDirection == 0 && Temperature >= TemperatureThreshold) || (TemperatureAlarmDirection == 1 && Temperature <= TemperatureThreshold)))
				TemperatureAlarmOn = true;
		}
		else
			LCD.print("-----");

		// Schedule another conversion
		Sensors.requestTemperatures();
	}

	uint8_t TemperatureStateCharacter = 1;
	void F_UpdateAlarmStatus()
	{
		LCD.setCursor(15, 0);
		
		if(TemperatureAlarmEnabled)
		{
			LCD.print(TemperatureStateCharacter ? LCD_ALARM_ENABLED_0 : LCD_ALARM_ENABLED_1);

			TemperatureStateCharacter = !TemperatureStateCharacter;
		}
		else
			LCD.print(LCD_ALARM_DISABLED);

		LCD.setCursor(15, 1);
		LCD.print(TemperatureAlarmDirection ? LCD_TEMP_ALARM_DIRECTION_1 : LCD_TEMP_ALARM_DIRECTION_0);
	}