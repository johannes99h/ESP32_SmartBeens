#line 1 "C:\\Github\\ESP32_SmartBeens_v0\\mhz19c.hpp"
#if !defined INCLUDED__ESP32_SMART_BEENS_V0__MHZ19C_HPP
#define INCLUDED__ESP32_SMART_BEENS_V0__MHZ19C_HPP


/* analog input for co2 detecting sensor
  - D13
*/


int mhz19_init( void );
int mhz19_get_co2_reading_analog( void );


#endif /* INCLUDED__ESP32_SMART_BEENS_V0__MHZ19C_HPP */