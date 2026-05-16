#include "sim7070.hpp"
#include "definitions.h"

#include "soc/rtc_cntl_reg.h"
#include "soc/rtc.h"
#include "driver/rtc_io.h"

// UART2 on pin 16 & 17 (only usable, if these pins are used as RX/TX)
HardwareSerial sim7070(2);

const char* API_KEY = THINGSPEAK_WRITE_API_KEY;


void sim7070_init() {
  if (_DEBUG) { Serial.printf("Initializing SIM7070G...\n\r"); }

  // open serial connection to modem
  sim7070.begin(9600, SERIAL_8N1, MODEM_RX, MODEM_TX);


  if (!sim7070_is_alive()) {
    sim7070_pwr_up();

    // wait for modem to become ready after boot
    int retry_count = 0;
    while (!sim7070_is_alive() && retry_count < 10) {
      delay(500);
      retry_count++;
    } 

    if (10 >= retry_count) { 
      if (_DEBUG) { Serial.printf("SIM7070G did not respond after pwr pulse!\n\r"); }
    }
  } else {
    if (_DEBUG) { Serial.printf("SIM7070G already alive, skipping pwr pulse.\n\r"); }
  }

  int retry_count = 0;
  while (!sim7070.available() && retry_count < 10) {
    delay(500);
    retry_count++;
  }
}


bool sim7070_is_alive() {
  while (sim7070.available()) { sim7070.read(); }         

  sim7070.println("AT");
  unsigned long t0 = millis();
  while (millis() - t0 < 2000) {
    if (sim7070.available()) {
      while (sim7070.available()) { sim7070.read(); }  
      return true;
    }
  }
  return false;
}

void sim7070_pwr_up() {
  // disable holding pins during deep sleep
  gpio_deep_sleep_hold_dis();
  gpio_hold_dis((gpio_num_t)MODEM_PWR);
  
  // toggle PWR pin
  digitalWrite(MODEM_PWR, HIGH);
  delay(1500);
  digitalWrite(MODEM_PWR, LOW);
  delay(1500);

  // hold pin low during deep sleep
  gpio_hold_en((gpio_num_t)MODEM_PWR);
  gpio_deep_sleep_hold_en();
}


void sim7070_send_AT_cmd(const char* cmd, unsigned long timeout_ms = 2000) {
  const unsigned int BUF_SIZE = 256;
  char r_buf[BUF_SIZE];  // allocate read buffer
  unsigned int idx = 0;
  unsigned long start = millis();

  // send command to modem
  sim7070.println(cmd);
  Serial.print(">> ");
  Serial.println(cmd);

  // clear buffer before reading
  while (sim7070.available()) sim7070.read();

  // read response until timeout
  while ((millis() - start) < timeout_ms) {
    while (sim7070.available()) {
      char c = sim7070.read();
      // store in buffer if space allows
      if (idx < BUF_SIZE - 1) {
        r_buf[idx++] = c;
      }
      // print line when newline received
      if (c == '\n') {
        r_buf[idx] = 0;                     // null terminate
        r_buf[strcspn(r_buf, "\r\n")] = 0;  // remove CR/LF
        if (strlen(r_buf) > 0) {
          Serial.println(r_buf);
        }
        idx = 0;  // reset buffer for next line
      }
    }
  }

  // print remaining buffer content (if existent)
  if (idx > 0) {
    r_buf[idx] = 0;
    r_buf[strcspn(r_buf, "\r\n")] = 0;
    if (strlen(r_buf) > 0) Serial.println(r_buf);
  }
}


void sim7070_modem_check() {
  if (_DEBUG) { Serial.printf("Checking SIM7070G modem...\n\r"); }
  // tests serial connection, should return "OK"
  // if (1 == boot_count) {
  sim7070_send_AT_cmd("AT");
  // } else {
  //   sim7070_test_wakeup_status("AT");
  // }

  // enable verbose output
  sim7070_send_AT_cmd("AT+CMEE=2");

  // check if SIM card is present
  sim7070_send_AT_cmd("AT+CPIN?");

  // check signal quality
  sim7070_send_AT_cmd("AT+CSQ");
}


int sim7070_wait_for_network_conn(unsigned long timeout_ms = 30000) {  // 30s max
  unsigned long t0 = millis();
  Serial.println(">> AT+CEREG?");
  while (millis() - t0 < timeout_ms) {
    sim7070.println("AT+CEREG?");
    delay(500);  // allow modem to respond

    while (sim7070.available()) {
      String line = sim7070.readStringUntil('\n');
      line.trim();
      if (line.length() > 0) Serial.println(line);

      if (line.indexOf("+CEREG: 0,1") >= 0 || line.indexOf("+CEREG: 0,5") >= 0) {
        Serial.println("Network registered!");
        return 0;
      }
    }
    delay(1000);  // wait a bit before next query
  }
  Serial.println("Timeout waiting for network registration!");

  return -1;
}



void sim7070_network_config() {
  if (_DEBUG) { Serial.printf("Starting SIM7070G modem configuration for connection to O2 NB-IoT network...\n\r"); }

  if (SIM7070_RESTORE_TO_DEFAULT) { sim7070_send_AT_cmd("AT&F"); }

  // configure LTE bands (in case, modem forgets which ones are available -> for O2, band 20 is primarily used)
  sim7070_send_AT_cmd("AT+CBANDCFG=\"NB-IOT\",1,2,3,4,5,8,12,13,14,18,19,20,25,26,27,28,66,85");

  // choose LTE (38) mode over GPRS
  sim7070_send_AT_cmd("AT+CNMP=38");

  // choose NB-IoT (2) over LTE CAT-M (1)
  sim7070_send_AT_cmd("AT+CMNB=2");

  // configure APN for ThingsMobile SIM card
  sim7070_send_AT_cmd("AT+CGDCONT=1,\"IP\",\"TM\"");

  // enable GPRS/LTE registration reporting
  sim7070_send_AT_cmd("AT+CREG=2");

  // enable EPS (LTE CAT-M/NB-IoT) registration reporting
  sim7070_send_AT_cmd("AT+CEREG=2");

  // select O2 network (only one possible for NB-IoT in Germany with ThingsMobile SIM card)
  sim7070_send_AT_cmd("AT+COPS=1,2,\"26203\",9");

  // restart modem
  sim7070_send_AT_cmd("AT+CFUN=1,1");
  delay(10000);

  // wait for recovering network connection (AT+CEREG?)
  sim7070_wait_for_network_conn();

  // check network operator, should return "O2" or "26203"
  sim7070_send_AT_cmd("AT+COPS?");

  // check data readiness, should show "1"
  sim7070_send_AT_cmd("AT+CGATT?");

  if (_DEBUG) { Serial.printf("Finished SIM7070G modem connection to O2 NB-IoT network.\n\r"); }
}


int sim7070_network_check() {
  // check network registration
  if (-1 == sim7070_wait_for_network_conn()) { return -1; }
  sim7070_send_AT_cmd("AT+CPSI?");
  sim7070_send_AT_cmd("AT+CASTATE?");

  return 0;
}


void sim7070_activate_PDP() {
  // activate PDP (only if not already active)
  if (!sim7070_check_for_active_PDP()) {
    Serial.println("Activating PDP...");
    sim7070_send_AT_cmd("AT+CNACT=0,1", 5000);  // wait 5s
  } else {
    Serial.println("PDP already active");
  }
  delay(1000);

  sim7070_send_AT_cmd("AT+CNACT?", 2000);
  delay(1000);
}


bool sim7070_check_for_active_PDP() {
  sim7070.println("AT+CNACT?");
  delay(2000);
  bool active = false;
  while (sim7070.available()) {
    String line = sim7070.readStringUntil('\n');
    line.trim();
    Serial.println(line);
    if (line.indexOf("+CNACT: 0,1") >= 0) {
      active = true;
    }
  }
  return active;
}


void sim7070_http_post_to_thingspeak(const http_request& req) {
  sim7070_activate_PDP();

  // open TCP socket
  Serial.println("Opening TCP socket...");
  sim7070.print("AT+CAOPEN=0,0,\"TCP\",\"api.thingspeak.com\",80\r\n");
  delay(5000);

  bool socketOpen = false;
  while (sim7070.available()) {
    String line = sim7070.readStringUntil('\n');
    line.trim();
    Serial.println(line);
    if (line.indexOf("+CAOPEN: 0,0") >= 0) {
      socketOpen = true;
    }
  }

  if (!socketOpen) {
    Serial.println("Failed to open TCP socket.");
    return;
  }

  sim7070_send_AT_cmd("AT+CASTATE?", 2000);

  // send HTTP GET request
  Serial.print("Sending HTTP GET (");
  Serial.print(req.length);
  Serial.println(" bytes)...");

  sim7070.print("AT+CASEND=0,");
  sim7070.println(req.length);
  delay(500);

  // wait for '>' prompt (signalizes readiness for HTTP input)
  bool promptFound = false;
  unsigned long t0 = millis();
  while (millis() - t0 < 5000) {  // 5s timeout
    if (sim7070.available()) {
      String line = sim7070.readString();
      if (line.indexOf(">") >= 0) {
        promptFound = true;
        break;
      }
    }
  }

  if (promptFound) {
    if (req.length <= 0 || req.length >= sizeof(req.buffer)) {
      Serial.println("Invalid HTTP request length!");
      return;
    }

    sim7070.print(req.buffer);
    sim7070.write(0x1A);  // end sending promt with Ctrl+Z
    delay(5000);          // wait for response

    Serial.println("Response from ThingSpeak:");
    // UNSAFE !!!
    while (sim7070.available()) {
      String line = sim7070.readStringUntil('\n');  // TODO: replace String with char[], since String uses heap!
      line.trim();
      if (line.length() > 0) {
        Serial.println(line);
      }
    }
    /*
    char line[128];
    int idx = 0;

    while (sim7070.available()) {
        char c = sim7070.read();

        if (c == '\r') continue;

        if (c == '\n') {
            if (idx > 0) {
                line[idx] = '\0';
                Serial.println(line);
                idx = 0;
            }
        } else {
            if (idx < sizeof(line) - 1) {
                line[idx++] = c;
            }
        }
    }
    */

  } else {
    Serial.println("No prompt '>' received for CASEND");
  }

  // close socket and deactivate PDP
  sim7070_send_AT_cmd("AT+CACLOSE=0");
  sim7070_send_AT_cmd("AT+CNACT=0,0");

  Serial.println("Done.");
}


void sim7070_prepare_single_sensor_data_for_http_post(int field_num, float data) {
  http_request req;

  // prepare HTTP GET
  req.length = snprintf(req.buffer, sizeof(req.buffer),
                        "GET /update?api_key=%s&field%d=%.2f HTTP/1.0\r\n"
                        "Host: api.thingspeak.com\r\n\r\n",
                        API_KEY, field_num, data);

  if (req.length >= sizeof(req.buffer)) {
    Serial.printf("WARNING: HTTP buffer truncated!\n\r");
    req.length = sizeof(req.buffer) - 1;
  }

  if (_DEBUG) { Serial.printf("HTTP GET request to be send: %s\n\r", req.buffer); }

  // conduct HTTP request
  sim7070_http_post_to_thingspeak(req);

  return;
}


void sim7070_prepare_all_sensor_data_for_http_post(float batt_voltage, float weight, const struct data& am2320_1, const struct data& am2320_2, const struct data& am2320_3) {
  http_request req;

  // prepare HTTP GET
  req.length = snprintf(req.buffer, sizeof(req.buffer),
                        "GET /update?api_key=%s&field1=%.2f&field2=%.2f&field3=%.2f&field4=%.2f&field5=%.2f&field6=%.2f&field7=%.2f&field8=%.2f HTTP/1.0\r\n"
                        "Host: api.thingspeak.com\r\n\r\n",
                        API_KEY, batt_voltage, weight, am2320_1.temperature, am2320_1.humidity, am2320_2.temperature,
                        am2320_2.humidity, am2320_3.temperature, am2320_3.humidity);

  if (req.length >= sizeof(req.buffer)) {
    Serial.printf("WARNING: HTTP buffer truncated!\n\r");
    req.length = sizeof(req.buffer) - 1;
  }

  if (_DEBUG) { Serial.printf("HTTP GET request to be send: %s\n\r", req.buffer); }

  // conduct HTTP request
  sim7070_http_post_to_thingspeak(req);

  return;
}


void sim7070_pwr_down() {
  bool modem_alive = false;

  // // check, if modem is still alive
  // sim7070_send_AT_cmd("AT");
  // unsigned long t0 = millis();
  // while (millis() - t0 < 1000) {
  //   if (sim7070.available()) {
  //     modem_alive = true;
  //     while (sim7070.available()) sim7070.read();  // clear buffer
  //     break;
  //   }
  // }

  // if (modem_alive) {
  if (sim7070_is_alive()) {
    Serial.printf("LTE modem is still alive, shutdown via AT command.\n\r"); 
    // safer way for powering down modem
    sim7070.println("AT+CPOWD=1");
    delay(3000); 
  } else {
    Serial.printf("LTE modem is not alive, shutdown via PWR pulse.\n\r"); 
    
    // disable holding pins during deep sleep
    gpio_hold_dis((gpio_num_t)MODEM_PWR);
    gpio_deep_sleep_hold_dis();
    
    // only fallback: shut module down with power pulse, if it doesn't react on AT pwr down 
    digitalWrite(MODEM_PWR, HIGH);
    delay(1500);
    digitalWrite(MODEM_PWR, LOW);
    delay(3000);

    // hold pin low during deep sleep
    gpio_deep_sleep_hold_en();
    gpio_hold_en((gpio_num_t)MODEM_PWR);
  }

  // close UART for SIM module
  sim7070.end();
}