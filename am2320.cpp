#include "am2320.hpp"
#include <Adafruit_AM2320.h>


Adafruit_AM2320 am2320 = Adafruit_AM2320();


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