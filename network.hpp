/*
#if !defined ESP32_SMART_BEENS_V0__NETWORK_IMPLEMENTATION_H
#define ESP32_SMART_BEENS_V0__NETWORK_IMPLEMENTATION_H


typedef unsigned char byte; 


// extern struct data_1 digital_data;
// extern struct data_2 analog_data;


// extern volatile bool MQTT_KEEP_ALIVE;


int wifi_init( void );
int wifi_deinit( void );

int mqtt_init( void );
int mqtt_deinit( void );
void mqtt_callback(char* topic, byte* message, unsigned int length);
void mqtt_reconfiguration(char* topic);
// void mqtt_loop( void );
void mqtt_connect( void );
// int mqtt_reconnect( void );
int mqtt_subscribe_topics( void );
int mqtt_publish_data( void );
// void mqtt_status_logging(const char* str);

// helper functions for better visibility 
// TODO: move to single function with measurements as parameters
int mqtt_publish_example( void );
// int mqtt_publish_temperature( void );
// int mqtt_publish_humidity( void );
// int mqtt_publish_soil_moisture( void );
// int mqtt_publish_light_intensity( void );
// int mqtt_publish_weight( void );

*/
#endif /* ESP32_SMART_BEENS_V0__NETWORK_IMPLEMENTATION_H */
