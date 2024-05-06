#include "sd_card.hpp"
#include "mhz19c.hpp"
#include "am2320.hpp"
#include "energy_optimization.hpp"
#include <stdio.h>


RTC_DATA_ATTR unsigned long current_file_idx = 0;             // file index on SD card
RTC_DATA_ATTR unsigned long long boot_count = 0;              // wakeup cycle count
RTC_DATA_ATTR unsigned long long time_since_start = 0;        // time since start of measurement


void setup() {

    wake_up_from_deep_sleep();

    // initialize pinout
    pinMode(ONBOARD_LED, OUTPUT);

    // initialize connected sensors
    if (-1 == sd_card_init()) { prepare_deep_sleep(); }
    mhz19_init();     // will take 60s to warm up
    am2320_init();

    if (1 == boot_count) {
      current_file_idx = sd_card_create_new_file();
    }

    runtime_routine();

    prepare_deep_sleep();
}


void loop() { ;;; }


int runtime_routine( void )
{
  float temperature = am2320_get_temperature();
  float humidity = am2320_get_humidity();
  int co2_ppm = mhz19_get_co2_reading_analog();

  char data[48]; // size could probably be reduced!
  sprintf(data, "%d,%.1f,%.1f,%d", time_since_start, temperature, humidity, co2_ppm);
  
  sd_card_append_to_log_file(current_file_idx, data);

  return 0;
}
