#include <Arduino.h>
#include <FastLED.h>

/* Alice Libraries */
#include <oracle_leds.h>
#include <AliceDefaultConfig.h>

#define LED_TYPE      ALICE_LEDS_CONFIG_LED_TYPE
#define COLOR_ORDER   ALICE_LEDS_CONFIG_COLOR_ORDER
//#define DATA_PIN    ALICE_LEDS_CONFIG_LED_PIN
#define NUM_LEDS      ALICE_LEDS_CONFIG_NUM_LEDS
#define DATA_PIN      21

static bool oracle_leds_status[ALICE_LEDS_CONFIG_NUM_LEDS];
static CRGB oracle_leds_colors[ALICE_LEDS_CONFIG_NUM_LEDS];
static uint8_t oracle_leds_bright[ALICE_LEDS_CONFIG_NUM_LEDS];

static CRGB oracle_leds_colors_previous[ALICE_LEDS_CONFIG_NUM_LEDS];
static uint8_t oracle_leds_bright_previous[ALICE_LEDS_CONFIG_NUM_LEDS];

void oracle_leds_apply(void)
{
    Serial.println("Apply Alice FastLED");
    FastLED.show();
}

void oracle_leds_turn_leds_off(void)
{
    for (int i = 0; i < ALICE_LEDS_CONFIG_NUM_LEDS; i++) {
        oracle_leds_bright_previous[i] = oracle_leds_bright[i];
        oracle_leds_colors_previous[i] = oracle_leds_colors[i];
        oracle_leds_status[i] = false;
    }

    FastLED.clear();
}

void oracle_leds_turn_leds_on(void)
{
    for (int i = 0; i < ALICE_LEDS_CONFIG_NUM_LEDS; i++) {
        oracle_leds_colors[i] = oracle_leds_colors_previous[i];
        oracle_leds_bright[i] = oracle_leds_bright_previous[i];
        oracle_leds_status[i] = true;
    }
    FastLED.show();
}

void oracle_leds_set_leds_color(const CRGB& new_color)
{
    for (int i = 0; i < ALICE_LEDS_CONFIG_NUM_LEDS; i++) {
        oracle_leds_colors[i] = new_color;
        oracle_leds_colors_previous[i] = new_color;
        oracle_leds_status[i] = true;
    }
}

void oracle_leds_set_leds_brightness(uint8_t brightness)
{
    FastLED.setBrightness(brightness);
}

uint8_t oracle_rfid_leds_get_num_leds(void)
{
    return ALICE_LEDS_CONFIG_NUM_LEDS;
}

void oracle_leds_turn_led_on(uint8_t led_index)
{
    oracle_leds_colors[led_index] = oracle_leds_colors_previous[led_index];
    oracle_leds_bright[led_index] = oracle_leds_bright_previous[led_index];
    oracle_leds_status[led_index] = true;
}

void oracle_leds_turn_led_off(uint8_t led_index)
{
    oracle_leds_colors_previous[led_index] = oracle_leds_colors[led_index];
    oracle_leds_bright_previous[led_index] = oracle_leds_bright[led_index];
    oracle_leds_status[led_index] = false;

    /* Dirty Hack */
    FastLED.clear();
    for (int i = 0; i < oracle_rfid_leds_get_num_leds(); i++) {
        if (!oracle_leds_status[i])
            continue;   
        oracle_leds_colors[i] = oracle_leds_colors_previous[i];
    }
}

void oracle_leds_setup(void)
{
    Serial.println("Initialize Alice FastLED");
    FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(oracle_leds_colors, ALICE_LEDS_CONFIG_NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(ALICE_LEDS_CONFIG_MAX_BRIGHTNESS);
    delay(100);
}