#line 1 "C:\\Github\\ESP32_SmartBeens_v0\\sd_card.cpp"
#include "sd_card.hpp"
#include "definitions.h"
#include <SPI.h>
#include <SD.h>


File myFile;


int sd_card_init( void )
{
  Serial.println("Initializing SD card...");

  if(!SD.begin(SD_CARD_CS))
  { 
    Serial.println("Initialization failed!");
    return -1;
  }
  
  Serial.println("SD card initialized.");
  blink_onboard_led(3); 

  return 0;
}


uint32_t sd_card_create_new_log_file( void )
{  
  uint32_t file_idx = 0;
  char path[24] = "/esp32_log_0.txt";
  
  // check which index is not already in use
  while (true == SD.exists(path)) {
    file_idx++;
    if (999 == file_idx) { 
      Serial.println("Reached maximum index.");
      break; 
    }
    sprintf(path, "/esp32_log_%lu.txt", file_idx);
  }
  
  Serial.printf("Successfully created new log file %s on SD card.\n\r", path);

  // open filestream
  myFile = SD.open(path, FILE_WRITE);

  // print data information as first lines
  if (USED_MHZ19C) { myFile.printf("Time since POR | T1 | RH1 | T2 | RH2 | T3 | RH3 | CO2 | G\n\r"); }
  if (USED_MHZ19E) { myFile.printf("Time since POR | T1 | RH1 | T2 | RH2 | T3 | RH3 | T4 | CO2 | G\n\r"); }
  myFile.println("-----------------------------------------------------------");

  // close filestream
  myFile.close();
  blink_onboard_led(3);

  return file_idx; 
}


void sd_card_create_new_config_file(int file_idx)
{
    char path[24] = "/esp32_config_0.txt";

  sprintf(path, "/esp32_config_%lu.txt", file_idx);

  // open filestream
  myFile = SD.open(path, FILE_WRITE);
  Serial.printf("Successfully created new config file %s on SD card.\n\r", path);

  // general ESP configuration
  myFile.printf("Sleeping interval: TIME_TO_SLEEP \n\r");
  myFile.printf("Blink onboard LED: USE_ONBOARD_LED \n\r"); 
  myFile.printf("-----------------------------------------------------------\n\r");

  // AM2320 configuration
  myFile.printf("AM2320 sensor 1 (pin AM2320_1_DATA_PIN) \n\r");
  myFile.printf(" - Temperature Offset: AM2320_1_TEMPERATURE_OFFSET \n\r");
  myFile.printf(" - Humidity Offset: AM2320_1_HUMIDITY_OFFSET \n\r");  
  myFile.printf("AM2320 sensor 2 (pin AM2320_2_DATA_PIN) \n\r");
  myFile.printf(" - Temperature Offset: AM2320_2_TEMPERATURE_OFFSET \n\r");
  myFile.printf(" - Humidity Offset: AM2320_2_HUMIDITY_OFFSET \n\r");  
  myFile.printf("AM2320 sensor 3 (pin AM2320_3_DATA_PIN) \n\r");
  myFile.printf(" - Temperature Offset: AM2320_3_TEMPERATURE_OFFSET \n\r");
  myFile.printf(" - Humidity Offset: AM2320_3_HUMIDITY_OFFSET \n\r");  

  // HX711 configuration
  myFile.printf("Weight sensor: HX711 \n\r");
  myFile.printf(" - Scale Factor: HX711_SCALE_FACTOR \n\r");
  myFile.printf(" - Offset: HX711_OFFSET \n\r");
  myFile.printf(" - Load Cell Type: HX711_LOAD_CELL_TYPE \n\r");
  myFile.printf("-----------------------------------------------------------\n\r");

  // used CO2 sensor
  if (USED_MHZ19C) { 
    myFile.printf("CO2 sensor: MHZ19C \n\r"); 
    myFile.printf(" - PWM \n\r");
    myFile.printf(" - Preheating used: MHZ19C_PREHEATING \n\r");                      // TODO: if-statement for true and false? depending on test!
  } else if (USED_MHZ19E) { 
    myFile.printf("CO2 sensor: MHZ19E \n\r"); 
    myFile.printf(" - UART \n\r");
  } else { 
    myFile.printf("CO2 sensor: -/- \n\r"); 
  }
  myFile.printf("-----------------------------------------------------------\n\r");

  // close filestream
  myFile.close();
}


int sd_card_append_to_file(const char* path, const char* str)
{
  // open filestream
  myFile = SD.open(path, FILE_APPEND);

  if (!myFile) {
    Serial.println("Error opening file.");
    return -1;
  }

  Serial.printf("Writing to %s... \n\r", path);
  myFile.println(str);
  
  myFile.close();
  Serial.printf("Writing to %s completed.\n\r", path);

  return 0;
}

int sd_card_append_to_log_file(uint32_t file_idx, const char* str)
{
  // open filestream
  char path[24]; 
  sprintf(path, "/esp32_log_%lu.txt", file_idx);
  myFile = SD.open(path, FILE_APPEND);

  if (!myFile) {
    Serial.println("Error opening file.");
    return -1;
  }

  Serial.printf("Appending to %s... \n\r", path);
  myFile.println(str);

  myFile.close();
  Serial.printf("Appending to %s completed.\n\r", path);

  return 0;
}


int sd_card_read_from_file(const char* path)
{
  myFile = SD.open(path);

  if (!myFile) {
    Serial.println("Error opening file.");
    return -1;
  }

  Serial.printf("Reading file from %s \n\r", path);
  
  // read entire file
  while(myFile.available()) 
  { 
    Serial.write(myFile.read()); 
  }

  return 0;
}


int sd_card_deinit( void )
{
  // energy optimization, not tested yet
  SD.end();
  Serial.printf("SD card deinitialized.\n\r");

  return 0;
}

