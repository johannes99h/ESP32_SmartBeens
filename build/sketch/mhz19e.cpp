#line 1 "C:\\Github\\ESP32_SmartBeens_v0\\mhz19e.cpp"
#include "mhz19e.hpp"
#include "definitions.h"


MHZ19 *mhz19_uart = new MHZ19(MHZ19E_RX, MHZ19E_TX);


int mhz19e_init( void ) 
{
    mhz19_uart->begin(MHZ19E_RX, MHZ19E_TX);
    mhz19_uart->setAutoCalibration(false);
    delay(500);
    Serial.printf("MHZ19E heating up... Status: %d.\n\r", mhz19_uart->getStatus());

    Serial.printf("MHZ19E initialized.\n\r");

    return 0;
}


measurement_t mhz19e_get_temperature_ppm( void )
{
    measurement_t m = mhz19_uart->getMeasurement();
    
    Serial.printf("CO2 concentration: %d PPM\n\r", m.co2_ppm);
    Serial.printf("Temperature at CO2 sensor: %d °C\n\r", m.temperature);
  
  return m;
}