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
    init_peripherals();
    runtime_routine();
    prepare_deep_sleep();
}


void loop() { ;;; }


int init_peripherals() 
{
  // initialize pinout
  pinMode(ONBOARD_LED, OUTPUT);
  pinMode(BATTERY_VOLTAGE_ADC_PIN, INPUT);

  // initialize connected sensors
  if (-1 == sd_card_init()) { prepare_deep_sleep(); }
  if (USED_MHZ19C) { mhz19_init(); }                         // will take 60s to warm up
  if (USED_MHZ19E) { mhz19e_init(); }
  am2320_init();                                             // TODO: differentiate between different numbers of sensors
  hx711_init();

  // init SD card on first power-on
  if (1 == boot_count) {
    current_file_idx = sd_card_create_new_log_file();
    sd_card_create_new_config_file(current_file_idx);
  } 

  return 0;
}


// TODO: move to sd_card.hpp/cpp without errors
int sd_prepare_data_log(unsigned long long time, struct data am2320_1, struct data am2320_2, struct data am2320_3, int co2_ppm, float co2_temperature, float weight, float batt_voltage)
{
  char data[SD_WRITE_BUFFER];

  // create string to be written
  unsigned int data_length = sprintf(data, "%d, %.1f, %.1f, %.1f, %.1f, %.1f, %.1f, %.1f, %d, %.1f, %.2f", 
                                    time, am2320_1.temperature, am2320_1.humidity, am2320_2.temperature, 
                                    am2320_2.humidity, am2320_3.temperature, am2320_3.humidity, co2_temperature, 
                                    co2_ppm, weight, batt_voltage);

  // check for maximum size violation
  if (data_length > SD_WRITE_BUFFER) { 
    Serial.printf("Data to be written on SD card exceeds buffer size.\n\r"); 
  } else {
    sd_card_append_to_log_file(current_file_idx, data);
  }

  return 0;
}


int runtime_routine( void )
{
  // TODO: move to function & save error codes (summation?) & offsets on SD card as well
  int status = am2320_get_sensor_vals(); 

  int co2_ppm = -999;
  float co2_temperature = -999;
  float batt_voltage = -999;
  float weight = -999;
  
  if (USED_MHZ19C) { 
    co2_ppm = mhz19_get_co2_reading_analog();
  }
  if (USED_MHZ19E) { 
    co2_ppm = mhz19e_get_co2(); 
    co2_temperature = mhz19e_get_temperature();
    mhz19e_deinit();                                    // energy saving measure
  }
  
  weight = hx711_get_weight();

  batt_voltage = get_battery_voltage();

  sd_prepare_data_log(time_since_start, am2320_1_data, am2320_2_data, am2320_3_data, co2_ppm, co2_temperature, weight, batt_voltage);
  sd_card_deinit();

  return 0;
}




