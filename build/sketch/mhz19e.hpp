#line 1 "C:\\Github\\ESP32_SmartBeens_v0\\mhz19e.hpp"
#if !defined INCLUDED__ESP32_SMART_BEENS_V0__MHZ19E_HPP
#define INCLUDED__ESP32_SMART_BEENS_V0__MHZ19E_HPP


#include "MHZ19.h"


int mhz19e_init( void );
measurement_t mhz19e_get_temperature_and_co2( void );
float mhz19e_get_temperature( void );
int mhz19e_get_co2( void ); 
void mhz19e_deinit( void );


#endif /* INCLUDED__ESP32_SMART_BEENS_V0__MHZ19E_HPP */