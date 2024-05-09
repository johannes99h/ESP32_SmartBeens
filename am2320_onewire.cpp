#include "am2320_onewire.hpp"
#include "dhtnew.h"
#include <SoftwareSerial.h>


// TODO: add global define for sensor count, so only necessary objects are initialized
DHTNEW am2320_1(AM2320_1_DATA_PIN); // pin D25
DHTNEW am2320_2(AM2320_2_DATA_PIN); // pin D26
DHTNEW am2320_3(AM2320_3_DATA_PIN); // pin D27


struct data am2320_1_data;
struct data am2320_2_data;
struct data am2320_3_data;


int am2320_init( void )
{
    am2320_1.setType(22);
    am2320_2.setType(22);
    am2320_3.setType(22);

    Serial.println("AM2320 initialized.");
    
    return 0;
}


// TODO: outsource every call to a single function and pass structs by reference
int am2320_get_sensor_vals( void )
{
    Serial.println("Reading AM2320 sensors...");

    // check status of first sensor
    am2320_1_data.debug_val = am2320_1.read();

    if (0 == am2320_1_data.debug_val) {                         // equals DHTLIB_OK
        am2320_1_data.temperature = am2320_1.getTemperature(); 
        am2320_1_data.humidity = am2320_1.getHumidity();

        // TODO: wrap in debug macro
        if (true) {
            Serial.println("Data from AM2320 no.1");
            Serial.printf("Temperature: %.1f °C, Humidity: %.1f %%\n\r", am2320_1_data.temperature, am2320_1_data.humidity);
        }
    } else {
        Serial.printf("Error %d while reading AM2320 no.1.\n\r", am2320_1_data.debug_val);
    }

    // check status of second sensor
    am2320_2_data.debug_val = am2320_2.read();

    if (0 == am2320_2_data.debug_val) {                         // equals DHTLIB_OK
        am2320_2_data.temperature = am2320_2.getTemperature(); 
        am2320_2_data.humidity = am2320_2.getHumidity();

        // TODO: wrap in debug macro
        if (true) {
            Serial.println("Data from AM2320 no.2");
            Serial.printf("Temperature: %.1f °C, Humidity: %.1f %%\n\r", am2320_2_data.temperature, am2320_2_data.humidity);
        }
    } else {
        Serial.printf("Error %d while reading AM2320 no.2.\n\r", am2320_2_data.debug_val);
    }

    // check status of third sensor
    am2320_3_data.debug_val = am2320_3.read();

    if (0 == am2320_3_data.debug_val) {                         // equals DHTLIB_OK
        am2320_3_data.temperature = am2320_3.getTemperature(); 
        am2320_3_data.humidity = am2320_3.getHumidity();

        // TODO: wrap in debug macro
        if (true) {
            Serial.println("Data from AM2320 no.3");
            Serial.printf("Temperature: %.1f °C, Humidity: %.1f %%\n\r", am2320_3_data.temperature, am2320_3_data.humidity);
        }
    } else {
        Serial.printf("Error %d while reading AM2320 no.3.\n\r", am2320_3_data.debug_val);
    }

    Serial.println("Finished reading AM2320 sensors.");

    return 0; 
}

