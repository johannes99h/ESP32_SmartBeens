#include "rtc.hpp"
#include "RTClib.h"
#include "definitions.h"


RTC_DS3231 rtc;


bool RTC_RESET = false;


int rtc_init( void )
{
  if (!rtc.begin()) { 
    Serial.printf("DS3231 RTC Clock not detected.\n\r"); 
    Serial.flush();
  }
  Serial.printf("DS3231 RTC clock initialized.\n\r");

  if (rtc.lostPower()) {
    Serial.println("RTC is NOT running, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    RTC_RESET = true;
  }

  // TODO: add entry in SD card folder (maybe even additional file with info & last known timestamp)
  if (true == RTC_RESET) { Serial.printf("RTC clock has been resetted to boot time!\n\r"); }

  return 0;
}

String rtc_get_timestamp() {
  // get timestamp from clock
  DateTime time = rtc.now();

  // print timestamp on serial console
  char timestamp_format[] = "YYMMDD-hh:mm:ss";
  String timestamp = time.toString(timestamp_format);

  // print converted timestamp
  Serial.println("Current time: " + timestamp);

  return timestamp;
}