#line 1 "C:\\Github\\ESP32_SmartBeens_v0\\am2320.hpp"
#if !defined INCLUDED__ESP32_SMART_BEENS_V0__AM2320_HPP
#define INCLUDED__ESP32_SMART_BEENS_V0__AM2320_HPP


/* onewire temperature sensor pinout
  - sensor 1 data in: D35
  - sensor 2 data in: D25
  - sensor 3 data in: D27
*/


struct data {
    int debug_val;
    float temperature; 
    float humidity;
};


int am2320_init( void );
int am2320_get_sensor_vals( void );
int am2320_deinit( void );


#endif /* INCLUDED__ESP32_SMART_BEENS_V0__AM2320_ONEWIRE_HPP */