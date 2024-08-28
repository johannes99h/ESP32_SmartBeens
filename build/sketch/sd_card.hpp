#line 1 "C:\\Github\\ESP32_SmartBeens_v0\\sd_card.hpp"
#if !defined INCLUDED__ESP32_SMART_BEENS_V0__SD_CARD_HPP
#define INCLUDED__ESP32_SMART_BEENS_V0__SD_CARD_HPP


#include <stdio.h>


/* SPI pins for SD card reader
  - MISO: 19
  - MOSI: 23
  - CLK:  18
  - CS:    5
*/


int sd_card_init( void );
uint32_t sd_card_create_new_log_file( void );
void sd_card_create_new_config_file(int file_idx);
int sd_card_write_to_file(const char* path, const char* str); 
int sd_card_append_to_log_file(uint32_t file_idx, const char* str); 
int sd_card_read_from_file(const char* path); 
int sd_card_deinit( void );


#endif /* INCLUDED__ESP32_SMART_BEENS_V0__SD_CARD_HPP */ 