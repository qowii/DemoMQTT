#ifndef __include_oracle_rfid_leds_h
#define __include_oracle_rfid_leds_h

#include <FastLED.h>

void oracle_leds_init(void);
void oracle_leds_apply(void);

uint8_t oracle_rfid_leds_get_num_leds(void);

void oracle_leds_turn_leds_on(void);
void oracle_leds_turn_leds_off(void);

void oracle_leds_turn_led_on(uint8_t led_index);
void oracle_leds_turn_led_off(uint8_t led_index);

void oracle_leds_set_leds_brightness(uint8_t brightness);
void oracle_leds_set_leds_color(const CRGB& new_color);

#endif /* #ifndef __include_oracle_rfid_leds_h */