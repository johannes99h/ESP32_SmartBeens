#include "_ntp.hpp"
#include "time.h"                       // for NTP connection & timestamp
#include <math.h>


const char*         ntpServer = "pool.ntp.org";
const long          gmtOffset_sec = 3600;
const int           daylightOffset_sec = 3600;


void ntp_clock_init( void )
{
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    ntp_clock_print_local_time();
}


void ntp_clock_print_local_time( void )
{
    struct tm timeinfo;

    if (!getLocalTime(&timeinfo)) { ntp_clock_init(); }

    Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}


const char* ntp_clock_get_local_time( void )
{
    struct tm timeinfo;
    
    if (!getLocalTime(&timeinfo)) { ntp_clock_init(); }

    // create & return string with timestamp
    char str[50];
    strftime(str, sizeof(str), "%A, %B %d %Y %H:%M:%S", &timeinfo);
    if (_DEBUG) { Serial.println(str); }

    return str;
}


const char* ntp_clock_return_iso_8601( void )
{
    struct tm timeinfo;
    
    if (!getLocalTime(&timeinfo)) { ntp_clock_init(); }

    static char time_str[50];

    int time_str_len = sprintf(time_str, "%02d-%02d-%02dT%02d:%02d:%02dZ", (1900+timeinfo.tm_year), (timeinfo.tm_mon+1), timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec); 
    
    if (_DEBUG) { Serial.printf("ISO 8601 timestamp: %s \n\r", time_str); }

    return time_str;
}

