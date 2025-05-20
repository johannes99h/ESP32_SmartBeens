#if !defined ESP32_SMART_BEENS_V0__NTP
#define ESP32_SMART_BEENS_V0__NTP


#include "definitions.h"
// #include <Arduino_JSON.h>


/* network clock */
void ntp_clock_init( void );
void ntp_clock_print_local_time( void ); 
const char* ntp_clock_get_local_time( void ); 
const char* ntp_clock_return_iso_8601( void ); 


#endif /* ESP32_SMART_BEENS_V0__NTP */