#include "sd_card.hpp"
#include "mhz19c.hpp"
#include "am2320.hpp"
#include "energy_optimization.hpp"
#include <stdio.h>


#define ONBOARD_LED 2           // pin D2
#define uS_TO_S_FACTOR 1000000  // conversion factor from micro seconds to seconds
#define TIME_TO_SLEEP  15       // sleeping time in seconds


RTC_DATA_ATTR int boot_count = 0;             // variable boot_count will be saved before going to sleep
RTC_DATA_ATTR uint32_t current_file_idx = 0;  // file index on SD card


void print_wakeup_reason()
{
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
  }
}


void setup() {
    pinMode(ONBOARD_LED, OUTPUT);
    
    Serial.begin(9600);
    delay(1000);          // waiting for serial console

    // waking up from deep sleep
    ++boot_count;
    Serial.println("Boot number: " + String(boot_count));
    print_wakeup_reason();

    // initialize sensors
    mhz19_init();                                   // !!! will take 60s to warm up
    am2320_init();
    sd_card_init();
    if (1 == boot_count) {
      current_file_idx = sd_card_create_new_file();   // !!! will create a new file after every wake-up
    }

    runtime_routine();

    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
    Serial.println("Setup ESP32 to sleep for every " + String(TIME_TO_SLEEP) + " Seconds");

    // going to deep sleep now
    Serial.println("Going to sleep now...");
    Serial.println("---------------------");
    delay(1000);
    Serial.flush(); 
    esp_deep_sleep_start();
}


void loop() { ; }


int runtime_routine( void )
{
  // int time_since_por = millis() / 1000;    // doesn't work with deep sleep, counter will be restarted with every wakeup!
  int time_since_start = boot_count * TIME_TO_SLEEP; 
  float temperature = am2320_get_temperature();
  float humidity = am2320_get_humidity();
  int co2_ppm = mhz19_get_co2_reading_analog();

  char data[48]; 
  sprintf(data, "%d,%.1f,%.1f,%d", time_since_start, temperature, humidity, co2_ppm);
  
  sd_card_append_to_log_file(current_file_idx, data);

  // heartbeat LED
  blink_onboard_led(1);

  return 0;
}


int blink_onboard_led(uint8_t count) 
{
  for (int i = 0; i <= count; i++) {
    digitalWrite(ONBOARD_LED, HIGH);
    delay(50);                       // replace by non-blocking delay
    digitalWrite(ONBOARD_LED, LOW);
    delay(50);
  }

  return 0;
}