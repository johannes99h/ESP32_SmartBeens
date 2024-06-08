# 1 "C:\\Github\\ESP32_SmartBeens_v0\\ESP32_SmartBeens_v0.ino"
# 2 "C:\\Github\\ESP32_SmartBeens_v0\\ESP32_SmartBeens_v0.ino" 2
# 3 "C:\\Github\\ESP32_SmartBeens_v0\\ESP32_SmartBeens_v0.ino" 2
# 4 "C:\\Github\\ESP32_SmartBeens_v0\\ESP32_SmartBeens_v0.ino" 2
# 5 "C:\\Github\\ESP32_SmartBeens_v0\\ESP32_SmartBeens_v0.ino" 2
# 6 "C:\\Github\\ESP32_SmartBeens_v0\\ESP32_SmartBeens_v0.ino" 2
# 7 "C:\\Github\\ESP32_SmartBeens_v0\\ESP32_SmartBeens_v0.ino" 2



# 9 "C:\\Github\\ESP32_SmartBeens_v0\\ESP32_SmartBeens_v0.ino"
__attribute__((section(".rtc.data" "." "28"))) unsigned long current_file_idx = 0; // file index on SD card
__attribute__((section(".rtc.data" "." "29"))) unsigned long long boot_count = 0; // wakeup cycle count
__attribute__((section(".rtc.data" "." "30"))) unsigned long long time_since_start = 0; // time since start of measurement


extern struct data am2320_1_data;
extern struct data am2320_2_data;
extern struct data am2320_3_data;


void setup() {

    wake_up_from_deep_sleep();

    // initialize pinout
    pinMode(2 /* pin D2*/, 0x03);

    // initialize connected sensors
    if (-1 == sd_card_init()) { prepare_deep_sleep(); }
    mhz19_init(); // will take 60s to warm up
    am2320_init();
    hx711_init();

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
  float weight = hx711_get_weight();

  char data[128]; // size could probably be reduced!
  sprintf(data, "%d, %.1f, %.1f, %.1f, %.1f, %.1f, %.1f, %d, %.1f", time_since_start, am2320_1_data.temperature, am2320_1_data.humidity,
          am2320_2_data.temperature, am2320_2_data.humidity, am2320_3_data.temperature, am2320_3_data.humidity, co2_ppm, weight);
  sd_card_append_to_log_file(current_file_idx, data);

  return 0;
}
