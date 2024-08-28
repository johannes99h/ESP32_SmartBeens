#line 1 "C:\\Github\\ESP32_SmartBeens_v0\\mhz19c.cpp"
#include "mhz19c.hpp"
#include <MHZ.h>
#include <SoftwareSerial.h>
#include "definitions.h"


MHZ co2(MHZ19x_RX, MHZ19x_TX, MHZ19C_CO2_IN, MHZ19C);


int mhz19_init( void ) 
{
    // TODO: library doesn't check preheating status when using pwm...
    if (true == MHZ19C_USE_PREHEATING) {
      if (co2.isPreHeating()) {
        Serial.print("CO2 sensor is preheating");
        while (co2.isPreHeating()) {
          Serial.print(".");
          delay(5000);
        }
      }
    }

    Serial.printf("\n\rMHZ19 initialized.\n\r");

    return 0;
}


int mhz19_get_co2_reading_analog( void )
{
    unsigned int ppm_pwm = co2.readCO2PWM();

    Serial.printf("CO2 concentration: %d PPM\n\r", ppm_pwm);

    // energy optimization
    /* causes exceptions & high current?
    analogRead(CO2_IN);
    analogRead(MHZ19x_TX);
    analogRead(MHZ19x_RX);
    */

    return ppm_pwm;
}