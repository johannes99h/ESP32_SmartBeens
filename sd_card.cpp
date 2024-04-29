#include "sd_card.hpp"
#include <stdio.h>
#include <SPI.h>
#include <SD.h>


File myFile;
const int CS = 5;


int sd_card_init( void )
{
  Serial.println("Initializing SD card...");
  if(!SD.begin(CS))
  { 
    Serial.println("Initialization failed!");
    return -1;
  }
  Serial.println("Initialization done.");

  for(int i = 0; i < 3; i++) {
    digitalWrite(2, HIGH);
    delay(50); 
    digitalWrite(2, LOW);
    delay(50);
  }

  return 0;
}


int sd_card_create_new_file( void )
{  
  uint32_t file_idx = 0;
  char path[20] = "/esp32_log_0.txt";
  
  // check which index is not already in use
  while (true == SD.exists(path)) {
    file_idx++;
    sprintf(path, "/esp32_log_%lu.txt", file_idx);
    if (999 == file_idx) { break; }
  }

// print data information as first lines
  myFile = SD.open(path, FILE_WRITE);
  myFile.println("Time since POR | Temperature | Humidity | CO2-Concentration");
  myFile.println("-----------------------------------------------------------");
  myFile.close();

  for(int i = 0; i < 3; i++) {
    digitalWrite(2, HIGH);
    delay(50); 
    digitalWrite(2, LOW);
    delay(50);
  }

  return 0; 
}


int sd_card_write_to_file(const char* path, const char* str)
{
  myFile = SD.open(path, FILE_APPEND);

  if (!myFile) {
    Serial.println("Error opening file.");
    return -1;
  }

  Serial.printf("Writing to %s...", path);
  myFile.println(str);
  myFile.close();
  Serial.println("Writing completed.");

  return 0;
}


int sd_card_read_from_file(const char* path)
{
  myFile = SD.open(path);

  if (!myFile) {
    Serial.println("Error opening file.");
    return -1;
  }

  Serial.printf("Reading file from %s \n", path);
  
  // read entire file
  while(myFile.available()) 
  { 
    Serial.write(myFile.read()); 
  }

  return 0;
}
