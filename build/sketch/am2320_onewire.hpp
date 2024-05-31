#line 1 "C:\\Github\\ESP32_SmartBeens_v0\\am2320_onewire.hpp"
#if !defined INCLUDED__ESP32_SMART_BEENS_V0__AM2320_ONEWIRE_HPP
#define INCLUDED__ESP32_SMART_BEENS_V0__AM2320_ONEWIRE_HPP


/* onewire temperature sensor pinout
  - sensor 1 data in: D35
  - sensor 2 data in: D25
  - sensor 3 data in: D27
*/


#define AM2320_1_DATA_PIN 25
#define AM2320_2_DATA_PIN 26
#define AM2320_3_DATA_PIN 27


struct data {
    int debug_val;
    float temperature; 
    float humidity;
};


int am2320_init( void );
int am2320_get_sensor_vals( void );
int am2320_deinit( void );


#endif /* INCLUDED__ESP32_SMART_BEENS_V0__AM2320_ONEWIRE_HPP */