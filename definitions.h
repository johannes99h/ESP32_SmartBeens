#include <Arduino.h>


/* general ESP functions */
#define ONBOARD_LED                 2           // pin D2
#define USE_ONBOARD_LED             true
#define SD_CARD_CS                  5           // chip-select pin for SD card SPI interface


/* energy optimization */
#define uS_TO_S_FACTOR              1000000     // conversion factor from micro seconds to seconds
#define TIME_TO_SLEEP               10          // sleeping time in seconds


/* AM2320 temperature and humidity sensors */ 
#define AM2320_1_DATA_PIN           25
#define AM2320_1_HUMIDITY_OFFSET    0
#define AM2320_1_TEMPERATURE_OFFSET 0
#define AM2320_2_DATA_PIN           26
#define AM2320_2_HUMIDITY_OFFSET    0
#define AM2320_2_TEMPERATURE_OFFSET 0
#define AM2320_3_DATA_PIN           27
#define AM2320_3_HUMIDITY_OFFSET    0
#define AM2320_3_TEMPERATURE_OFFSET 0


/* MHZ19x CO2 Sensor */
#define USED_MHZ19C                 false
#define USED_MHZ19E                 true
#define MHZ19x_RX                   16          // D16
#define MHZ19x_TX                   17          // D17
#define MHZ19C_CO2_IN               13          // Arduino pin for pwm reading
// #define MHZ19C_SIZE_MEAN_VAL        1           // even used?
#define MHZ19C_USE_PREHEATING       false



/* HX711 24-bit ADC */
#define HX711_DATA                  21
#define HX711_CLOCK                 22
#define HX711_READ_SAMPLES          11          // should be an odd number!
#define HX711_LOAD_CELL_TYPE        2
#define HX711_OFFSET                339500
#define HX711_SCALE_FACTOR          21.438265


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