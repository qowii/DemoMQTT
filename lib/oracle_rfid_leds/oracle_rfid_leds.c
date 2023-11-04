#include <Arduino.h>
#include <FastLED.h>

#include "oracle_rfid_leds.h"

/* Alice Libraries */
#include <AliceDefaultConfig.h>

#define LED_TYPE      ALICE_LEDS_CONFIG_LED_TYPE
#define COLOR_ORDER   ALICE_LEDS_CONFIG_COLOR_ORDER
//#define DATA_PIN    ALICE_LEDS_CONFIG_LED_PIN
#define NUM_LEDS      ALICE_LEDS_CONFIG_NUM_LEDS
#define DATA_PIN      21

static CRGB AliceSketchLeds[ALICE_LEDS_CONFIG_NUM_LEDS];

void oracle_rfid_leds_turnoff(void)
{
  FastLED.clear();
  FastLED.show();
  delay(100);
}

void oracle_rfid_leds_change_color(const CRGB& new_color)
{
    for (int i = 0; i < ALICE_LEDS_CONFIG_NUM_LEDS; i++) {
        AliceSketchLeds[i] = new_color;
    }
    FastLED.show();
    delay(100);
}

void oracle_rfid_leds_init(void)
{
    Serial.println("Initialize Alice FastLED");
    FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(AliceSketchLeds, ALICE_LEDS_CONFIG_NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(ALICE_LEDS_CONFIG_MAX_BRIGHTNESS);
    delay(100);
}
