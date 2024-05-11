#include "sd_card.hpp"
#include "mhz19c.hpp"
#include "am2320.hpp"           // probably obsolete now!
#include "am2320_onewire.hpp"
#include "energy_optimization.hpp"
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
    mhz19_init();     // will take 60s to warm up
    am2320_i2c_init();

    if (1 == boot_count) {
      current_file_idx = sd_card_create_new_file();
    }

    runtime_routine();

    prepare_deep_sleep();
}


void loop() { ;;; }


int runtime_routine( void )
{
  // TODO: move to function & save error codes (summation?) & offsets on SD card as well
  int status = am2320_get_sensor_vals(); 
  int co2_ppm = mhz19_get_co2_reading_analog();

  char data[128]; // size could probably be reduced!
  sprintf(data, "%d, %.1f, %.1f, %.1f, %.1f, %.1f, %.1f, %d", time_since_start, am2320_1_data.temperature, am2320_1_data.humidity,
          am2320_2_data.temperature, am2320_2_data.humidity, am2320_3_data.temperature, am2320_3_data.humidity, co2_ppm);
  sd_card_append_to_log_file(current_file_idx, data);

  return 0;
}

