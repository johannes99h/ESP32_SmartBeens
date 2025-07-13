#include <Arduino.h>
#include "secrets.h"


/* general ESP functions */
#define _DEBUG                      true
#define ONBOARD_LED                 D9           // pin 2, on Firebeetle board D9
#define USE_ONBOARD_LED             true
#define SD_CARD_CS                  D6           // chip-select pin for SD card SPI interface
#define SD_WRITE_BUFFER             512
#define RTC_USED                    false
#define TRANSISTORS_USED            true
#ifdef TRANSISTORS_USED
#define SENSOR_SUPPLY_3V3           D7
#define SENSOR_SUPPLY_5V            D9
#endif


/* energy optimization */
#define uS_TO_S_FACTOR              1000000     // conversion factor from micro seconds to seconds
#define TIME_TO_SLEEP               900          // sleeping time in seconds
#define BATTERY_VOLTAGE_ADC_PIN     34          // collision with HX711 DATA!


/* AM2320 temperature and humidity sensors */ 
#define AM2320_1_DATA_PIN           D10
#define AM2320_1_HUMIDITY_OFFSET    0
#define AM2320_1_TEMPERATURE_OFFSET 0
#define AM2320_2_DATA_PIN           D11
#define AM2320_2_HUMIDITY_OFFSET    0
#define AM2320_2_TEMPERATURE_OFFSET 0
#define AM2320_3_DATA_PIN           D12
#define AM2320_3_HUMIDITY_OFFSET    0
#define AM2320_3_TEMPERATURE_OFFSET 0


/* MHZ19x CO2 Sensor */
#define USED_MHZ19C                 false
#define USED_MHZ19E                 false
#define MHZ19x_RX                   21          // D16
#define MHZ19x_TX                   21          // D17
#define MHZ19C_CO2_IN               21          // Arduino pin for pwm reading
#define MHZ19C_USE_PREHEATING       false


/* HX711 24-bit ADC */
#define HX711_DATA                  21
#define HX711_CLOCK                 22
#define HX711_READ_SAMPLES          11          // should be an odd number!
#define HX711_LOAD_CELL_TYPE        2
#define HX711_OFFSET                39757
#define HX711_SCALE_FACTOR          21.732576

// blink LED count-times
inline void blink_onboard_led(unsigned int count) 
{
  if (USE_ONBOARD_LED) {
    for (int i = 0; i <= count; i++) {
      digitalWrite(ONBOARD_LED, HIGH);
      delay(50);
      digitalWrite(ONBOARD_LED, LOW);
      delay(50);
    }
  }
}