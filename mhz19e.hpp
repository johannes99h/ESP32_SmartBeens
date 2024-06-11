#if !defined INCLUDED__ESP32_SMART_BEENS_V0__MHZ19E_HPP
#define INCLUDED__ESP32_SMART_BEENS_V0__MHZ19E_HPP


#include "MHZ19.h"


/* uart input for co2 detecting sensor */
#define MHZ19E_RX 16       // D16
#define MHZ19E_TX 17       // D17


int mhz19e_init( void );
measurement_t mhz19e_get_temperature_and_co2( void );
float mhz19e_get_temperature( void );
int mhz19e_get_co2( void ); 


#endif /* INCLUDED__ESP32_SMART_BEENS_V0__MHZ19E_HPP */