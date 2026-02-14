#if !defined INCLUDED__ESP32_SMART_BEENS_V0__SIM7070_HPP
#define INCLUDED__ESP32_SMART_BEENS_V0__SIM7070_HPP



struct http_request {
  char buffer[256];
  int length;
};


void sim7070_init(); 
void sim7070_pwr_up();
void sim7070_sent_AT_cmd(const char *cmd, unsigned long timeout_ms) ;
void sim7070_modem_check();
void sim7070_wait_for_network_conn(unsigned long timeout_ms);
void sim7070_network_config();
void sim7070_network_check();
void sim7070_activate_PDP(); 
bool sim7070_check_for_active_PDP();
http_request sim7070_prepare_http_get_request(int field_num, float data); 
void sim7070_http_post_to_thingspeak();
void sim7070_deinit();
void sim7070_pwr_down();


#endif /* INCLUDED__ESP32_SMART_BEENS_V0__SIM7070_HPP */