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