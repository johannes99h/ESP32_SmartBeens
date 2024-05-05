#include "sd_card.hpp"
#include "mhz19c.hpp"
#include "am2320.hpp"
#include <stdio.h>


#define ONBOARD_LED 2   // pin D2


volatile bool CALL_RUNTIME_ROUTINE = false;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
hw_timer_t * timer = NULL;

uint32_t current_file_idx = 0;

void setup() {
    pinMode(ONBOARD_LED, OUTPUT);
    
    Serial.begin(9600);
    while (!Serial) { ; }

    mhz19_init();

    am2320_init();

    sd_card_init();
    current_file_idx = sd_card_create_new_file();
}


void loop() {
  // replacement for timer since it isn't working just now 
  /*
  uint64_t last_millis_count = millis();
  if (millis() > last_millis_count + 1000) {
    CALL_RUNTIME_ROUTINE = true;
  }

  if (true == CALL_RUNTIME_ROUTINE) {
    runtime_routine();
  }
  */

  runtime_routine();
  delay(1000);
}


/* configure timer for interrupt every second
int timer_interrupt_init( void )
{
  timer = timerBegin(0, 240, true);              // divider
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 1000000, true);        // prescaler
  timerAlarmEnable(timer);
  
  return 0; 
}
*/


/* set flag for runtime routine on timer interrupt  
void IRAM_ATTR onTimer( void )
{
  portENTER_CRITICAL_ISR(&timerMux);
  CALL_RUNTIME_ROUTINE = true;
  portEXIT_CRITICAL_ISR(&timerMUX);
}
*/


int runtime_routine( void )
{
  // reset flag
  CALL_RUNTIME_ROUTINE = false;

  int time_since_por = millis() / 1000;
  float temperature = am2320_get_temperature();
  float humidity = am2320_get_humidity();
  int co2_ppm = mhz19_get_co2_reading_analog();

  char data[48]; 
  sprintf(data, "%d,%f,%f,%d", time_since_por, temperature, humidity, co2_ppm);
  
  sd_card_append_to_log_file(current_file_idx, data);

  // heartbeat LED
  blink_onboard_led(1);

  // esp_deep_sleep_start();
  
  return 0;
}


int blink_onboard_led(uint8_t count) 
{
  for (int i = 0; i <= count; i++) {
    digitalWrite(ONBOARD_LED, HIGH);
    delay(50);                       // replace by non-blocking delay
    digitalWrite(ONBOARD_LED, LOW);
    delay(50);
  }

  return 0;
}