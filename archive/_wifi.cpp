#include "_wifi.hpp"
// #include "time.h"                       // for NTP connection & timestamp
// #include <WiFi.h>
#include <WiFiClientSecure.h>
// #include <HTTPClient.h>
// #include <exception>
// #include <stdexcept>
// #include "_ntp.hpp"


// WiFiClient esp32_client;  
WiFiClientSecure esp32_client;                                
// HTTPClient http;
// String json_buf;


const char*         wifi_ssid = WIFI_SSID;
const char*         wifi_password = WIFI_PASSKEY;
// const char*         isrg_root_x1 = \
//                                   "-----BEGIN CERTIFICATE-----\n" \
//                                   "MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw\n" \
//                                   "TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh\n" \
//                                   "cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4\n" \
//                                   "WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu\n" \
//                                   "ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY\n" \
//                                   "MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc\n" \
//                                   "h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+\n" \
//                                   "0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U\n" \
//                                   "A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW\n" \
//                                   "T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH\n" \
//                                   "B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC\n" \
//                                   "B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv\n" \
//                                   "KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn\n" \
//                                   "OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn\n" \
//                                   "jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw\n" \
//                                   "qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI\n" \
//                                   "rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV\n" \
//                                   "HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq\n" \
//                                   "hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL\n" \
//                                   "ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ\n" \
//                                   "3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK\n" \
//                                   "NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5\n" \
//                                   "ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur\n" \
//                                   "TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC\n" \
//                                   "jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc\n" \
//                                   "oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq\n" \
//                                   "4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA\n" \
//                                   "mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d\n" \
//                                   "emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=\n" \
//                                   "-----END CERTIFICATE-----\n"; 


int wifi_init( void )
{
    // set to station mode (ESP connects to access point)
    // WiFi.mode(WIFI_STA);

    WiFi.begin(wifi_ssid, wifi_password); 
    Serial.printf("Connecting to WiFi: %s\n\r", wifi_ssid);   

    while (WL_CONNECTED != WiFi.status()) { ;;; }
    if (_DEBUG) { Serial.printf("Successfully connected to WiFi.\n\r"); }

    // // set certificate for HTTPS
    // esp32_client.setCACert(isrg_root_x1);

    // print IP address & signal strength
    if (_DEBUG) { 
        Serial.printf("Local IP address: %d\n\r", WiFi.localIP()); 
        Serial.printf("Wifi signal strength (RSSI): %6lddBm\n\r", WiFi.RSSI());
    }

    return 0;
}


// void http_prepare_data_to_post( void )
// {
//     // only first http post (either one) works for some reason!
//     http_post_sensor_data_to_questdb(QUESTDB_TABLE_NAME,digital_data.temperature, digital_data.humidity, analog_data.soil_moisture, analog_data.light_intensity, digital_data.weight);
//     // http_post_data_to_powerbi(digital_data.temperature, digital_data.humidity, analog_data.soil_moisture, analog_data.light_intensity, digital_data.weight);
// }


// void http_prepare_dwd_data_to_post( void )
// {
//     http_post_dwd_data_to_questdb("InnoCube_Weather_Test", weather_data.moon_phase, weather_data.condition, weather_data.cloud_cover, weather_data.sunshine, weather_data.visibility, weather_data.temperature, weather_data.relative_humidity);
// }

// void http_post_data_to_powerbi(float t, float rh, float sm, float li, float w)
// {
//     if (_DEBUG) { Serial.printf("HTTP post.\ n\r"); }
    
//     // try-catch is just a temporary measure since post sometimes crashes uC
//     try {
//         // define goal location & data type
//         http.begin(POWERBI_ADRESS);
//         http.addHeader("Content-Type", "application/json");

//         // allocate memory
//         char* pbi_buf = (char*)malloc(DB_BUF_SIZE * sizeof(char));

//         // construct .JSON from data
//         unsigned int buf_size = sprintf(pbi_buf, "{\"temperature\":\"%.1f\",\"humidity\":\"%.1f\",\"light_intensity\":\"%.1f\",\"soil_humidity\":\"%.1f\",\"weight\":\"%.1f\"}", t, rh, sm, li, w);
//         if (buf_size > DB_BUF_SIZE) { if (_DEBUG) { Serial.printf("JSON buffer is too small!"); } }
//         if (_DEBUG) { Serial.printf("JSON: %s\n\r", pbi_buf); } 

//         // post & check http response
//         int http_response = http.POST(pbi_buf);
//         if (_DEBUG) { if (200 == http_response) { Serial.printf("HTTP post successful.\n\r"); }     // 200: OK
//         else { Serial.printf("HTTPS response code: %d\n\r", http_response); } }

//         // close connection & free memory
//         free(pbi_buf);
//         if (_DEBUG) { Serial.printf("Successfully freed memory.\n\r"); } 
//         http.end();
//         if (_DEBUG) { Serial.printf("Successfully ended HTTP connection.\n\r"); } 
//     } catch (const std::exception &e) { Serial.printf("Exception reason: %s", e.what()); }
}


// void http_post_sensor_data_to_questdb(const char* db, float t, float rh, float sm, float li, float w)
// {
//     if (_DEBUG) { Serial.printf("Preparing HTTP post to database.\n\r"); }
    
//     // define goal location & data type
//     bool http_status = http.begin(QUESTDB_ADRESS);
//     http.addHeader("Content-Type", "text-plain");
//     if (_DEBUG) { 
//         if (http_status) { Serial.printf("HTTP connection to database established.\n\r"); }
//         else { Serial.printf("No HTTP connection to database.\n\r"); }
//     }

//     // allocate memory
//     char* qdb_buf = (char*)malloc(DB_BUF_SIZE * sizeof(char));

//     // construct ILP-string from data
//     unsigned int buf_size = sprintf(qdb_buf, "%s,temp=%.1f,humidity=%.1f,moisture=%.1f,light=%.1f,weight=%.1f", db, t, rh, sm, li, w);
//     if (buf_size > DB_BUF_SIZE) { if (_DEBUG) { Serial.printf("ILP buffer is too small!"); } }
//     if (_DEBUG) { Serial.printf("ILP: %s (char*)\n\r", qdb_buf); } 

//     // post & check http response
//     int http_response = http.POST(qdb_buf);
//     if (_DEBUG) { if (200 == http_response) { Serial.printf("HTTP post successful.\n\r"); }     // 200: OK
//     else { Serial.printf("HTTP response code: %d\n\r", http_response); } }

//     // close connection & free memory
//     http.end();
//     free(qdb_buf);
// }


// void http_post_dwd_data_to_questdb(const char* db, const char* moon, const char* condition, float cloud, float sunshine, float visibility, float t_out, float rh_out) 
// {
//     if (_DEBUG) { Serial.printf("Preparing HTTP post to database.\n\r"); }
    
//     // define goal location & data type
//     bool http_status = http.begin(QUESTDB_ADRESS);
//     http.addHeader("Content-Type", "text-plain");
//     if (_DEBUG) { 
//         if (http_status) { Serial.printf("HTTP connection to database established.\n\r"); }
//         else { Serial.printf("No HTTP connection to database.\n\r"); }
//     }

//     // allocate memory
//     char* qdb_buf = (char*)malloc(DB_BUF_SIZE * sizeof(char));

//     // construct ILP-string from data
//     unsigned int buf_size = sprintf(qdb_buf, "%s,moon_phase=%s,condition=%s,cloud_cover=%.1f,sunshine=%.1f,visibility=%.1f,temp_outside=%.1f,hum_outside=%.1f", db, moon, condition, cloud, sunshine, visibility, t_out, rh_out);
//     if (buf_size > DB_BUF_SIZE) { if (_DEBUG) { Serial.printf("ILP buffer is too small!"); } }
//     if (_DEBUG) { Serial.printf("ILP: %s (char*)\n\r", qdb_buf); } 

//     // post & check http response
//     int http_response = http.POST(qdb_buf);
//     if (_DEBUG) { if (200 == http_response) { Serial.printf("HTTP post successful.\n\r"); }     // 200: OK
//     else { Serial.printf("HTTP response code: %d\n\r", http_response); } }

//     // close connection & free memory
//     http.end();
//     free(qdb_buf);
// }


// void http_get_weather_data( void )
// {
//   // build requesting string
//   const char* curr_time = ntp_clock_return_iso_8601();
//   char* req_str = (char*)malloc(DB_BUF_SIZE * sizeof(char));

//   Serial.printf("Still alive with %s.\n\r", curr_time); 
  
//   // split timestamp for current weather
//   int req_str_len = sprintf(req_str, "%sweather?date=%s&dwd_station_id=%s&tz=%s", WEATHER_ADRESS, curr_time, WEATHER_DWD_STATION_ID, WEATHER_TZ);
//   if (_DEBUG) { Serial.printf("Requesting weather data on: %s\n\r", req_str); }
    
//   http.begin(esp32_client, req_str); 
  
//   int http_response = http.GET();
//   if (0 < http_response) {
//     Serial.printf("HTTP response code for GET-request: %d\n\r", http_response);
    
//     // get data
//     json_buf = http.getString();
//     Serial.print(json_buf);
//     Serial.println();

//     JSONVar weather_obj = JSON.parse(json_buf);

//     if (_DEBUG) {
//       Serial.print("Sunshine: ");
//       Serial.println(weather_obj["weather"][0]["sunshine"]);
//       Serial.print("Temperature: ");
//       Serial.println(weather_obj["weather"][0]["temperature"]);
//     }
//   } else {
//     Serial.printf("HTTP GET request failed with error code: %d\n\r", http_response);
//   }

//   http.end();

//   free(req_str);
// }


// void http_store_weather_data_in_struct(JSONVar w_obj)
// {
//     weather_data.cloud_cover = (JSON.stringify(w_obj["weather"][0]["cloud_cover"])).toFloat();                              // [%]
//     weather_data.dew_point = (JSON.stringify(w_obj["weather"][0]["dew_point"])).toFloat();                                  // [°C]
//     weather_data.precipitation = (JSON.stringify(w_obj["weather"][0]["precipitation"])).toFloat();                          // [mm]
//     weather_data.precipitation_probability = (JSON.stringify(w_obj["weather"][0]["precipitation_probability"])).toFloat();  // [%]
//     weather_data.pressure = (JSON.stringify(w_obj["weather"][0]["pressure_msl"])).toFloat();                                // [hPa]
//     weather_data.relative_humidity = (JSON.stringify(w_obj["weather"][0]["relative_humidity"])).toFloat();                  // [%]
//     weather_data.solar_irradiation = (JSON.stringify(w_obj["weather"][0]["solar"])).toFloat();                              // [kWh/m^2]
//     weather_data.sunshine = (JSON.stringify(w_obj["weather"][0]["sunshine"])).toFloat();                                    // min
//     weather_data.temperature = (JSON.stringify(w_obj["weather"][0]["temperature"])).toFloat();                              // [°C]
//     weather_data.visibility = (JSON.stringify(w_obj["weather"][0]["visibility"])).toFloat();                                // m
//     weather_data.wind_direction = (JSON.stringify(w_obj["weather"][0]["wind_direction"])).toFloat();                        // [°]
//     weather_data.wind_speed = (JSON.stringify(w_obj["weather"][0]["wind_speed"])).toFloat();                                // [km/h]
//     weather_data.moon_phase = ntp_calc_moon_phase();
//     weather_data.condition = w_obj["weather"][0]["condition"];
//     weather_data.last_api_call_timestamp = ntp_clock_return_iso_8601();                                                     // [ISO 8601 timestamp]
//     weather_data.last_dwd_update = w_obj["weather"][0]["timestamp"];                                                        // [some timestamp]

//     if (_DEBUG) { 
//       Serial.printf("DWD API call on: %s with last update on: %s\n\r", weather_data.last_api_call_timestamp, weather_data.last_dwd_update);
//       Serial.printf(" - Cloud cover: %.1f\n\r", weather_data.cloud_cover); 
//       Serial.printf(" - Dew point: %.1f\n\r", weather_data.dew_point); 
//       Serial.printf(" - Precipitation: %.1f\n\r", weather_data.precipitation); 
//       Serial.printf(" - Precipitation probability: %.1f\n\r", weather_data.precipitation_probability); 
//       Serial.printf(" - Pressure: %.1f\n\r", weather_data.pressure); 
//       Serial.printf(" - Relative humidity (outside): %.1f\n\r", weather_data.relative_humidity); 
//       Serial.printf(" - Solar irradiation: %.1f\n\r", weather_data.solar_irradiation); 
//       Serial.printf(" - Sunshine: %.1f\n\r", weather_data.sunshine); 
//       Serial.printf(" - Temperature (outside): %.1f\n\r", weather_data.temperature); 
//       Serial.printf(" - Visibility: %.1f\n\r", weather_data.visibility); 
//       Serial.printf(" - Wind direction: %.1f\n\r", weather_data.wind_direction); 
//       Serial.printf(" - Wind speed: %.1f\n\r", weather_data.wind_speed); 
//       Serial.printf(" - Moon phase: %s\n\r", weather_data.moon_phase); 
//       Serial.printf(" - Condition: %s\n\r", weather_data.condition); 
//     }
// }

