#include <Arduino.h>
#line 1 "C:\\Github\\ESP32_SmartBeens_v0\\ESP32_SmartBeens_v0.ino"
#include "sd_card.hpp"
#include "mhz19c.hpp"
#include "mhz19e.hpp"
#include "am2320.hpp"
#include "hx711.hpp"
#include "energy_optimization.hpp"
#include "definitions.h"
#include <stdio.h>


RTC_DATA_ATTR unsigned long current_file_idx = 0;             // file index on SD card
RTC_DATA_ATTR unsigned long long boot_count = 0;              // wakeup cycle count
RTC_DATA_ATTR unsigned long long time_since_start = 0;        // time since start of measurement


extern struct data am2320_1_data;
extern struct data am2320_2_data;
extern struct data am2320_3_data;


void setup() {
    wake_up_from_deep_sleep();

    // initialize pinout
    pinMode(ONBOARD_LED, OUTPUT);

    // initialize connected sensors
    if (-1 == sd_card_init()) { prepare_deep_sleep(); }
    if (USED_MHZ19C) { mhz19_init(); }                         // will take 60s to warm up
    if (USED_MHZ19E) { mhz19e_init(); }
    am2320_init();
    hx711_init(); 

    if (1 == boot_count) {
      current_file_idx = sd_card_create_new_log_file();
      sd_card_create_new_config_file(current_file_idx);
    }

    runtime_routine();

    prepare_deep_sleep();
}


void loop() { ;;; }


int runtime_routine( void )
{
  // TODO: move to function & save error codes (summation?) & offsets on SD card as well
  int status = am2320_get_sensor_vals(); 
  float weight = hx711_get_weight();

  char data[128]; // size could probably be reduced!

  if (USED_MHZ19C) {
    int co2_ppm = mhz19_get_co2_reading_analog();
    sprintf(data, "%d, %.1f, %.1f, %.1f, %.1f, %.1f, %.1f, %d, %.1f", time_since_start, am2320_1_data.temperature, am2320_1_data.humidity,
            am2320_2_data.temperature, am2320_2_data.humidity, am2320_3_data.temperature, am2320_3_data.humidity, co2_ppm, weight);
    sd_card_append_to_log_file(current_file_idx, data); 
  }

  if (USED_MHZ19E) {
    int co2_ppm = mhz19e_get_co2();
    float co2_temperature = mhz19e_get_temperature();
    sprintf(data, "%d, %.1f, %.1f, %.1f, %.1f, %.1f, %.1f, %.1f, %d, %.1f", time_since_start, am2320_1_data.temperature, am2320_1_data.humidity,
            am2320_2_data.temperature, am2320_2_data.humidity, am2320_3_data.temperature, am2320_3_data.humidity, co2_temperature, co2_ppm, weight);
    sd_card_append_to_log_file(current_file_idx, data); 
  }

  if (USED_MHZ19E) { mhz19e_deinit(); }
  sd_card_deinit();

  return 0;
}


