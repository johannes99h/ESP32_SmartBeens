#line 1 "C:\\Github\\ESP32_SmartBeens_v0\\hx711.cpp"
#include "hx711.hpp"
#include <HX711.h>
#include "definitions.h"


HX711 scale;


void hx711_init( void ) 
{
  scale.begin(HX711_DATA, HX711_CLOCK);

  scale.set_offset(HX711_OFFSET);
  scale.set_scale(HX711_SCALE_FACTOR);

  Serial.println("HX711 initialized.");
}


void hx711_calibrate( void ) 
{
  Serial.printf("Starting calibration phase for HX711.\n\r");
  Serial.printf("Remove all weight from the load cell");

  //  TODO: remove if not necessary!
  while (Serial.available()) { Serial.read(); }

  Serial.printf(" and press enter.\n\r");
  while (Serial.available() == 0);

  // takes average of 20 zero weight measurements for offset-determination
  Serial.printf("Determining zero weight offset... ");
  scale.tare(20);
  uint32_t offset = scale.get_offset();
  Serial.printf("Offset: %d\n\r", offset);


  Serial.printf("Place a known weight on the load  cell.\n\r");

  //  TODO: remove if not necessary!
  while (Serial.available()) { Serial.read(); }

  Serial.printf("Enter the weight in (whole) grams and press enter.\n\r");
  uint32_t weight = 0;
  while (Serial.peek() != '\n')
  {
    if (Serial.available())
    {
      char ch = Serial.read();
      if (isdigit(ch))
      {
        weight *= 10;
        weight = weight + (ch - '0');
      }
    }
  }
  Serial.printf("Calibration weight: %d", weight);
  scale.calibrate_scale(weight, 20);
  float scale_val = scale.get_scale();

  Serial.printf("Scale: %f\n\r", scale_val);

  Serial.print("Use scale.set_offset(%d) and scale.set_scale(%f).\n\r");
}


float hx711_get_weight( void )
{
  float weight_unit = scale.get_units(HX711_READ_SAMPLES);
  Serial.printf("Weight unit: %f\n\r", weight_unit);

  // scale.power_down();

  return weight_unit;
}

