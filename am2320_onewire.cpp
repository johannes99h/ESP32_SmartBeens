#include "am2320.hpp"
#include "dhtnew.h"
#include <SoftwareSerial.h>


DHTNEW am2320_1(5); // pin 16

/*
DHTNEW am2320_2(6); // pin 17


int am2320_init( void )
{
    am2320.begin();
    Serial.println("AM2320 initialized.");
    
    return 0;
}


float am2320_get_temperature( void )
{
    float temperature = am2320.readTemperature();
    Serial.printf("Temperature: %.1f °C\n\r", temperature);
    return temperature;
}


float am2320_get_humidity( void )
{
    float humidity = am2320.readHumidity();
    Serial.printf("Humidity: %.1f percent\n\r", humidity);
    return humidity;
}
*/