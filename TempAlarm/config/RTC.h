	/**
	 * Module: RTC
	 * ===========
	 */

		//#define RTC_HW DS3231
		#define RTC_I2C_ADDRESS 0x68

		#define RTC_DAYS "Dom\0", "Lun\0", "Mar\0", "Mie\0", "Jue\0", "Vie\0", "Sab\0"
		#define RTC_MONTHS "ene\0", "feb\0", "mar\0", "abr\0", "may\0", "jun\0", "jul\0", "ago\0", "sep\0", "oct\0", "nov\0", "dic\0"

	/**
	 * LCD configuration
	 */

		#define RTC_DEGREE_CHAR ((char) 223)

	/**
	 * OneWireSwitches configuration
	 */

		#define RTC_BUTTON_CONFIGURE_ID TEMPERATURE_BUTTON_DIRECTION_ID
		#define RTC_BUTTON_CONFIGURE_BUZZER_TIME 5

		#define RTC_BUTTON_MINUS_ID TEMPERATURE_BUTTON_MINUS_ID
		#define RTC_BUTTON_MINUS_THRESHOLD 100

		#define RTC_BUTTON_PLUS_ID TEMPERATURE_BUTTON_PLUS_ID
		#define RTC_BUTTON_PLUS_THRESHOLD 100

		#define RTC_BUTTON_NEXT_ID TEMPERATURE_BUTTON_STARTSTOP_ID
		#define RTC_BUTTON_NEXT_THRESHOLD 50
		#define RTC_BUTTON_NEXT_DELAY 150
		#define RTC_BUTTON_NEXT_BUZZER_TIME 5