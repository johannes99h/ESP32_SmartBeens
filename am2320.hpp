#if !defined INCLUDED__ESP32_SMART_BEENS_V0__AM2320_HPP
#define INCLUDED__ESP32_SMART_BEENS_V0__AM2320_HPP


/* i2c temperature sensor pinout
  - SDA: D21
  - SCL: D22
*/


int am2320_i2c_init( void );
float am2320_i2c_get_temperature( void );
float am2320_i2c_get_humidity( void );


#endif /* INCLUDED__ESP32_SMART_BEENS_V0__AM2320_HPP */