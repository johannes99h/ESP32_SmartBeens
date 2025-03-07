#if !defined INCLUDED__ESP32_SMART_BEENS_V0__RTC_HPP
#define INCLUDED__ESP32_SMART_BEENS_V0__RTC_HPP


#include <Arduino.h>


/* DS3231 I2C Pinout (equivalent to scale)
   - SDA: D21
   - SCL: D22
*/

int rtc_init( void );
String rtc_get_timestamp( void );


#endif /* INCLUDED__ESP32_SMART_BEENS_V0__RTC_HPP */