#if !defined INCLUDED__ESP32_SMART_BEENS_V0__SIM7070_HPP
#define INCLUDED__ESP32_SMART_BEENS_V0__SIM7070_HPP


#include "am2320.hpp"


extern unsigned long long boot_count;

struct http_request {
  char buffer[256];
  int length;
};


void sim7070_init(); 
bool sim7070_is_alive(); 
void sim7070_pwr_up();
void sim7070_sent_AT_cmd(const char *cmd, unsigned long timeout_ms) ;
void sim7070_modem_check();
int  sim7070_wait_for_network_conn(unsigned long timeout_ms);
void sim7070_network_config();
int  sim7070_network_check();
void sim7070_activate_PDP(); 
bool sim7070_check_for_active_PDP();
void sim7070_http_post_to_thingspeak(const http_request& req);
void sim7070_prepare_single_sensor_data_for_http_post(int field_num, float data); 
// void sim7070_prepare_all_sensor_data_for_http_post(float batt_voltage, float weight, const struct data& am2320_1, const struct data& am2320_2, const struct data& am2320_3); 
void sim7070_prepare_all_sensor_data_for_http_post(float batt_voltage, float weight, const struct data& am2320_1, const struct data& am2320_2); 
void sim7070_pwr_down();


#endif /* INCLUDED__ESP32_SMART_BEENS_V0__SIM7070_HPP */