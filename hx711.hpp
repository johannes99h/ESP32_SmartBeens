#if !defined INCLUDED__ESP32_SMART_BEENS__HX711_HPP
#define INCLUDED__ESP32_SMART_BEENS__HX711_HPP


void hx711_init( void );
void hx711_calibrate( void );
float hx711_get_weight( void );


#endif  /* INCLUDED__ESP32_SMART_BEENS__HX711_HPP */ 