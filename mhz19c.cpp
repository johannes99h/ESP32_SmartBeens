#include "mhz19c.hpp"
#include <MHZ.h>
#include <SoftwareSerial.h>


#define MH_Z19_RX 3  // D7
#define MH_Z19_TX 2  // D6
#define CO2_IN 13 // Arduino pin for pwm reading


MHZ co2(MH_Z19_RX, MH_Z19_TX, CO2_IN, MHZ19C);



int mhz19_init( void ) 
{
    if (co2.isPreHeating()) {
    Serial.println("CO2 sensor is preheating.");
    while (co2.isPreHeating()) {
      Serial.print(".");
      delay(5000);
    }
    }

    return 0;
}


int mhz19_get_co2_reading_analog( void )
{
    int ppm_pwm = co2.readCO2PWM();
    Serial.printf("CO2 concentration: %d PPM\n\r", ppm_pwm);
    return ppm_pwm;
}