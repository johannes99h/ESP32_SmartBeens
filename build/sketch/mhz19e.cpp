#line 1 "C:\\Github\\ESP32_SmartBeens_v0\\mhz19e.cpp"
#include "mhz19e.hpp"
#include "definitions.h"


MHZ19 *mhz19_uart = NULL;


int mhz19e_init( void ) 
{
    MHZ19 *mhz19_ptr = new MHZ19(MHZ19x_RX, MHZ19x_TX);
    mhz19_uart = mhz19_ptr;
    delay(500);
    
    mhz19_uart->begin(MHZ19x_RX, MHZ19x_TX);
    mhz19_uart->setAutoCalibration(false);
    delay(500);
    Serial.printf("MHZ19E heating up... Status: %d.\n\r", mhz19_uart->getStatus());

    Serial.printf("MHZ19E initialized.\n\r");

    return 0;
}


measurement_t mhz19e_get_temperature_and_co2( void )
{
    measurement_t m = mhz19_uart->getMeasurement();
    
    Serial.printf("CO2 concentration: %d PPM\n\r", m.co2_ppm);
    Serial.printf("Temperature at CO2 sensor: %d °C\n\r", m.temperature);
  
  return m;
}


float mhz19e_get_temperature( void )
{
  measurement_t m = mhz19_uart->getMeasurement();
  
  float temperature = m.temperature;

  Serial.printf("Temperature at CO2 sensor: %.1f °C\n\r", m.temperature);

  return temperature;
}


int mhz19e_get_co2( void )
{
  measurement_t m = mhz19_uart->getMeasurement();

  int co2 = m.co2_ppm;

  Serial.printf("CO2 concentration: %dppm\n\r", m.co2_ppm);
  
  return co2;
}


void mhz19e_deinit( void )
{
  delete mhz19_uart;
}