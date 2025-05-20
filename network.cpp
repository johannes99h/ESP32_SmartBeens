/*
#include "network.hpp"
#include <WiFi.h>
#include <WiFiClientSecure.h>           // for TLS connection
#include <PubSubClient.h>
// #include "time.h"                       // for NTP connection & timestamp
#include "definitions.h"


// TODO: make & pass struct for all WiFi & MQTT credentials 
const char*         wifi_ssid = WIFI_SSID;
const char*         wifi_password = WIFI_PASSKEY;
const char*         mqtt_broker = MQTT_BROKER;
int                 mqtt_port = MQTT_PORT;
const char*         mqtt_username = MQTT_CREDENTIALS_USR;
const char*         mqtt_password = MQTT_CREDENTIALS_PASSWORD;
static const char*  hivemq_server_ca PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw
TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh
cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4
WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu
ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY
MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc
h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+
0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U
A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW
T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH
B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC
B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv
KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn
OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn
jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw
qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI
rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV
HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq
hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL
ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ
3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK
NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5
ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur
TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC
jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc
oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq
4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA
mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d
emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=
-----END CERTIFICATE-----)EOF";

// // for timestamp
// const char*         ntpServer = "pool.ntp.org";
// const long          gmtOffset_sec = 3600;
// const int           daylightOffset_sec = 3600;


// WiFiClient esp32_client;                                 // without TLS
WiFiClientSecure esp32_client;                              // with TLS
PubSubClient client(mqtt_broker, mqtt_port, esp32_client);


int wifi_init( void )
{
    // set to station mode (ESP connects to access point)
    // WiFi.mode(WIFI_STA);

    WiFi.begin(wifi_ssid, wifi_password); 
    Serial.printf("Connecting to WiFi: %s\n\r", wifi_ssid);   

    while (WL_CONNECTED != WiFi.status()) { ;;; }
    Serial.printf("Successfully connected to WiFi.\n\r");   

    // set HiveMQ certificate
    esp32_client.setCACert(hivemq_server_ca);

    // print IP address & signal strength
    if (_DEBUG) { 
        Serial.printf("Local IP address: %d\n\r", WiFi.localIP()); 
        Serial.printf("Wifi signal strength (RSSI): %6lddBm\n\r", WiFi.RSSI());
    }

    return 0;
}


int wifi_deinit( void )
{
  if (WiFi.status() == WL_CONNECTED) {
    WiFi.disconnect();
    if (_DEBUG) { Serial.printf("Disconnecting from Wifi"); }
  }

  while (WiFi.status() == WL_CONNECTED) {
    if (_DEBUG) { Serial.print("."); }
    delay(50);
  }

  if (_DEBUG) { Serial.printf("\n\rSuccessfully disconnected from Wifi.\n\r"); }
}


int mqtt_init( void )
{
    client.setCallback(mqtt_callback);  
    client.setSocketTimeout(60);
    client.setKeepAlive(60);

    mqtt_connect();

    return 0;
}


int mqtt_deinit( void )
{
  if (client.connected()) {
    client.disconnect();
    if (_DEBUG) { Serial.printf("Disconnecting from MQTT broker"); }
  }

  // TODO: eigene Flag definieren
  while (client.connected()) {
    if (_DEBUG) { Serial.print("."); }
    delay(50);
  }

  if (_DEBUG) { Serial.printf("\n\rSuccessfully disconnected from MQTT broker.\n\r"); }
}


void mqtt_callback(char* topic, byte* message, unsigned int length) 
{
    Serial.printf("Received message on topic: %s\n\r", topic);
    
    String str;
    
    // unpack received message
    for (unsigned int i = 0; i < length; i++) {
        str += (char)message[i];
    }

    Serial.printf("Received message: %s\n\r", str);

    // enter reconfiguration function
    char c_str[length];
    str.toCharArray(c_str,length);
    if (strcmp(topic, (char*)c_str)) mqtt_reconfiguration(topic);
}


void mqtt_reconfiguration( char* topic )
{
    if (_DEBUG) { Serial.printf("Reconfiguration entered.\n\r"); }

    if (strcmp(topic, "work/esp32/configuration/samplinginterval"))  { 
        if (_DEBUG) { Serial.printf("Reconfiguration of sampling interval.\n\r"); }
    }
}


void mqtt_connect( void ) 
{
    // connects to defined broker with passed string as ID
    if (!client.connect(ESP_MQTT_CLIENT_ID, mqtt_username, mqtt_password)) {
        Serial.printf("Error connecting to MQTT broker.\n\r");
        return;
    } else {
        Serial.printf("Successfully connected to MQTT broker.\n\r");
    }

    // mqtt_subscribe_topics();

    client.publish("work/esp32/status", "Initial MQTT connection established.");
}


// void mqtt_loop( void ) 
// {
//     // check connection
//     if (!client.connected()) { mqtt_reconnect(); }

//     // avoid being disconnected by broker (sends keep-alive)
//     client.loop();

//     // reset status flag
//     // MQTT_KEEP_ALIVE = false;
// }



// int mqtt_reconnect( void )
// {
//     unsigned int retry_counter = 0;

//     Serial.printf("Attempting MQTT connection...\n\r");
    
//     while (!client.connected() && 10 != retry_counter) {
//         if (client.connect("ESP32Client-1", mqtt_username, mqtt_password)) {               
//             Serial.printf("Successfully reconnected to MQTT client.\n\r");
//             break;
//         } else {
//             Serial.printf("Error %d while reconnecting to MQTT client.\n\r", client.state());
//         }
//         delay(1000);
//         retry_counter++;
//     }

//     mqtt_subscribe_topics();

//     return 0;
// }


int mqtt_subscribe_topics( void )
{
    if (!client.connected()) { 
        return -1; 
    } else {
        // configuration topics
        client.subscribe("work/esp32/configuration/sampling_interval");
        client.subscribe("work/esp32/configuration/time_between_measurements");
        client.subscribe("work/esp32/configuration/hx711_sample_count");
    }

    return 0;
}


int mqtt_publish_example( void )
{
    int test_float = 12.34; 
    char data_str[8];
    dtostrf(test_float, 1, 2, data_str);

    if (_DEBUG) { Serial.printf("MQTT package to be published on private/test: %s\n\r", data_str); }
    
    if (!client.publish("private/test", data_str)) { 
        if (_DEBUG) { Serial.printf("Error publishing on topic private/test.\n\r"); }
        return -1; 
    }

    return 0;
}


// int mqtt_publish_data( void ) 
// {   
//     if (0 != mqtt_publish_temperature()) { return -1; }
//     if (0 != mqtt_publish_humidity()) { return -1; }
//     if (0 != mqtt_publish_soil_moisture()) { return -1; }
//     if (0 != mqtt_publish_light_intensity()) { return -1; }
//     if (0 != mqtt_publish_weight()) { return -1; }

//     return 0;
// }


// int mqtt_publish_temperature( void )
// {
//     char temp_str[8];
//     dtostrf(digital_data.temperature, 1, 2, temp_str);

//     if (_DEBUG) { Serial.printf("MQTT package to be published on work/esp32/temperature: %s\n\r", temp_str); }
    
//     if (!client.publish("work/esp32/temperature", temp_str)) { 
//         if (_DEBUG) { Serial.printf("Error publishing on topic work/esp32/temperature.\n\r"); }
//         return -1; 
//     }

//     return 0;
// }


// int mqtt_publish_humidity( void )
// {
//     char hum_str[8];
//     dtostrf(digital_data.humidity, 1, 2, hum_str);

//     if (_DEBUG) { Serial.printf("MQTT package to be published on work/esp32/humidity: %s\n\r", hum_str); }

//     if (!client.publish("work/esp32/humidity", hum_str)) { 
//         if (_DEBUG) { Serial.printf("Error publishing on topic work/esp32/humidity.\n\r"); }
//         return -1; 
//     }

//     return 0;
// }


// int mqtt_publish_soil_moisture( void )
// {
//     char soil_moisture_str[8];
//     dtostrf(analog_data.soil_moisture, 1, 2, soil_moisture_str);

//     if (_DEBUG) { Serial.printf("MQTT package to be published on work/esp32/soil_moisture: %s\n\r", soil_moisture_str); }

//     if (!client.publish("work/esp32/soil_moisture", soil_moisture_str)) { 
//         if (_DEBUG) { Serial.printf("Error publishing on topic work/esp32/soil_mositure.\n\r"); }
//         return -1; 
//     }

//     return 0;
// }


// int mqtt_publish_light_intensity( void )
// {
//     char light_intensity_str[8];
//     dtostrf(analog_data.light_intensity, 1, 2, light_intensity_str);

//     if (_DEBUG) { Serial.printf("MQTT package to be published on work/esp32/light_intensity: %s\n\r", light_intensity_str); }

//     if (!client.publish("work/esp32/light_intensity", light_intensity_str)) { 
//         if (_DEBUG) { Serial.printf("Error publishing on topic work/esp32/light_intensity.\n\r"); }
//         return -1; 
//     }

//     return 0;
// }


// int mqtt_publish_weight( void )
// {
//     char weight_str[8];
//     dtostrf(digital_data.weight, 1, 2, weight_str);

//     if (_DEBUG) { Serial.printf("MQTT package to be published on work/esp32/weight: %s\n\r", weight_str); }

//     if (!client.publish("work/esp32/weight", weight_str)) { 
//         if (_DEBUG) { Serial.printf("Error publishing on topic work/esp32/weight.\n\r"); }
//         return -1; 
//     }

//     return 0;
// }


// void mqtt_status_logging(const char* str)
// {
//     // currently doesn't work as expected
//     // const char* timestamp = ntp_clock_get_local_time();
//     // Serial.printf("Returned string: %s", ntp_clock_get_local_time());

//     // add timestamp to string
//     // strcat(new_str, str);

//     client.publish("work/esp32/status", timestamp);     // string goes missing...

//     client.publish("work/esp32/status", str);
// }
*/