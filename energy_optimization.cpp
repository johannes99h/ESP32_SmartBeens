#include "energy_optimization.hpp"
#include "esp_sleep.h"
#include "esp_adc_cal.h"
#include <SoftwareSerial.h>
#include "am2320.hpp"
#include "sd_card.hpp"
#include "definitions.h"


#define LOW_BATTERY_VOLTAGE 3.20
#define VERY_LOW_BATTERY_VOLTAGE 3.10
#define CRITICALLY_LOW_BATTERY_VOLTAGE 3.00



int wake_up_from_deep_sleep() {
  // update "alive counter"
  if (0 != boot_count) {
    time_since_start += TIME_TO_SLEEP;
  }

  // heartbeat LED
  if (_DEBUG) { blink_onboard_led(1); }

  // activate serial console
  Serial.begin(115200);
  delay(5000);
  Serial.printf("\n\r\n\r\n\r---------------------");

  // waking up from deep sleep
  Serial.println("Wakeup cycle: " + String(boot_count));
  Serial.printf("Time since start of measurement: %d s\n\r", time_since_start);

  ++boot_count;

  print_wakeup_reason();

  return 0;
}


int print_wakeup_reason() {
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch (wakeup_reason) {
    case ESP_SLEEP_WAKEUP_EXT0: Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1: Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER: Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD: Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP: Serial.println("Wakeup caused by ULP program"); break;
    default: Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason); break;
  }

  return 0;
}


int gpio_init() {
  // set PWR GPIO 
  pinMode(MODEM_PWR, OUTPUT); 
  digitalWrite(MODEM_PWR, LOW); 
  
  // hold pin low during deep sleep
  // gpio_deep_sleep_hold_en();
  // gpio_hold_en((gpio_num_t) MODEM_PWR);  
  
  pinMode(ONBOARD_LED, OUTPUT);
  pinMode(BATTERY_VOLTAGE_ADC_PIN, INPUT);

  if (TRANSISTORS_USED) {
    pinMode(SENSOR_SUPPLY_3V3, OUTPUT);
    pinMode(SENSOR_SUPPLY_5V, OUTPUT);
    digitalWrite(SENSOR_SUPPLY_3V3, HIGH);
    digitalWrite(SENSOR_SUPPLY_5V, HIGH);
  }

  delay(500);

  return 0;
}


int gpio_deinit() {
  if (TRANSISTORS_USED) {
    digitalWrite(SENSOR_SUPPLY_5V, LOW);
    digitalWrite(SENSOR_SUPPLY_3V3, LOW);
  }

  delay(100);

  return 0;
}


float get_battery_voltage() {
  uint32_t value = 0;
  int rounds = 11;
  esp_adc_cal_characteristics_t adc_chars;

  // battery voltage divided by 2 can be measured at GPIO34, which equals ADC1_CHANNEL6
  switch (esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &adc_chars)) {
    case ESP_ADC_CAL_VAL_EFUSE_TP:
      Serial.println("Characterized using Two Point Value");
      break;
    case ESP_ADC_CAL_VAL_EFUSE_VREF:
      Serial.printf("Characterized using eFuse Vref (%d mV)\r\n", adc_chars.vref);
      break;
    default:
      Serial.printf("Characterized using Default Vref (%d mV)\r\n", 1100);
  }

  // to avoid noise, sample the pin several times and average the result
  for (int i = 1; i <= rounds; i++) {
    value += analogRead(BATTERY_VOLTAGE_ADC_PIN);
  }
  value /= (uint32_t)rounds;

  // due to the voltage divider (1M+1M) values must be multiplied by 2 and convert mV to V
  return (esp_adc_cal_raw_to_voltage(value, &adc_chars) * 2.0 / 1000.0);
}


int prepare_deep_sleep() {
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);

  // am2320_deinit();      // currently doesn't do anything, but crashes uC!
  sd_card_deinit();
  gpio_deinit();

  // // heartbeat LED
  // if (_DEBUG) {
  //   blink_onboard_led(3);
  //   digitalWrite(ONBOARD_LED, LOW);
  // }

  Serial.printf("Going to sleep now, will wake in %d seconds.\n\r", TIME_TO_SLEEP);
  Serial.println("---------------------");
  delay(1000);
  Serial.flush();

  // update "alive counter" with time passed since wake-up
  time_since_start += millis() / 1000;

  esp_deep_sleep_start();

  return 0;   // will never be reached!
}
