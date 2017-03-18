#include "TempAlarmModule.h"

	TempAlarmModule::TempAlarmModule(LCD_CLASS* L, OneWireSwitches<SWITCHES_AMOUNT, SWITCHES_INPUT_PIN>* S)
	{
		LCD = L;
		Switches = S;
	}

	void TempAlarmModule::printZerofill(uint8_t Number)
	{
		if(Number < 10)
			LCD->print('0');

		LCD->print(Number);
	}