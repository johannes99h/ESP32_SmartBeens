#if !defined INCLUDED__ESP_32_SD_Test__SD_CARD_HPP
#define INCLUDED__ESP_32_SD_Test__SD_CARD_HPP


/* SPI pins for SD card reader
  - MISO: 19
  - MOSI: 23
  - CLK:  18
  - CS:    5
*/


int sd_card_init( void );
int sd_card_create_new_file( void );
int sd_card_write_to_file(const char* path, const char* str); 
int sd_card_read_from_file(const char* path); 


#endif /* INCLUDED__ESP_32_SD_Test__SD_CARD_HPP */ 