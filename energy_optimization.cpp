#include "energy_optimization.hpp"
#include "esp_sleep.h"
#include <SoftwareSerial.h>
#include "am2320.hpp"
#include "sd_card.hpp"
#include "definitions.h"


int wake_up_from_deep_sleep() 
{
    // update "alive counter"
    if (0 != boot_count) {
      time_since_start += TIME_TO_SLEEP; 
    }

    // heartbeat LED
    // blink_onboard_led(1);

    // activate serial console
    Serial.begin(9600);
    delay(1000);
    Serial.printf("\n\r\n\r\n\r---------------------");
    
    // waking up from deep sleep
    Serial.println("Wakeup cycle: " + String(boot_count));
    Serial.printf("Time since start of measurement: %d s\n\r", time_since_start);

    ++boot_count;

    print_wakeup_reason();

    return 0;
}


int print_wakeup_reason()
{
    esp_sleep_wakeup_cause_t wakeup_reason;

    wakeup_reason = esp_sleep_get_wakeup_cause();

    switch(wakeup_reason)
    {
      case ESP_SLEEP_WAKEUP_EXT0:     Serial.println("Wakeup caused by external signal using RTC_IO"); break;
      case ESP_SLEEP_WAKEUP_EXT1:     Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
      case ESP_SLEEP_WAKEUP_TIMER:    Serial.println("Wakeup caused by timer"); break;
      case ESP_SLEEP_WAKEUP_TOUCHPAD: Serial.println("Wakeup caused by touchpad"); break;
      case ESP_SLEEP_WAKEUP_ULP:      Serial.println("Wakeup caused by ULP program"); break;
      default :                       Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
    }

    return 0;
}


float get_battery_voltage()
{
  int raw_val = analogRead(BATTERY_VOLTAGE_ADC_PIN);

  float shunt_voltage = (float)raw_val * 3.3 / 4096;
  if (_DEBUG) { Serial.printf("Current shunt voltage: %.2fV\n\r", shunt_voltage); }

  // measurement over lower resistor
  unsigned int R_1 = 5000;      // upper resistor
  unsigned int R_2 = 10000;     // lower resistor

  float batt_voltage = shunt_voltage * ((float)R_1 + (float)R_2) / (float)R_2; 
  if (_DEBUG) { Serial.printf("Current battery voltage: %.2fV\n\r", batt_voltage); }

  return batt_voltage;
}


int prepare_deep_sleep()
{
    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);

    am2320_deinit();
    sd_card_deinit();

    // heartbeat LED
    // blink_onboard_led(3);

    Serial.println("Going to sleep now, will wake in " + String(TIME_TO_SLEEP) + " seconds.");
    Serial.println("---------------------");
    delay(1000);
    Serial.flush(); 

    // update "alive counter" with time passed since wake-up
    time_since_start += millis() / 1000;

    esp_deep_sleep_start();

    return 0;
}

