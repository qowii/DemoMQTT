#ifndef __include_oracle_rfid_leds_h
#define __include_oracle_rfid_leds_h

#include <FastLED.h>

void oracle_rfid_leds_turnoff(void);
void oracle_rfid_leds_change_color(const CRGB& new_color);
void oracle_rfid_leds_init(void);

#endif /* #ifndef __include_oracle_rfid_leds_h */