#ifndef __include_oracle_rfid_leds_h
#define __include_oracle_rfid_leds_h

#include <FastLED.h>

/*! 
 * \brief Initialize the leds
 */
void oracle_leds_setup(void);

/*!
 * \brief Apply the leds changes
*/
void oracle_leds_apply(void);

/*!
 * \brief Get the number of leds
 * \return The number of leds
 */
uint8_t oracle_rfid_leds_get_num_leds(void);

/*!
 * \brief Turn on the leds
 */
void oracle_leds_turn_leds_on(void);

/*!
 * \brief Turn off the leds
 */
void oracle_leds_turn_leds_off(void);

/*!
 * \brief Turn on a led by its index
 * \param led_index The led index
 */
void oracle_leds_turn_led_on(uint8_t led_index);

/*!
 * \brief Turn off a led by its index
 * \param led_index The led index
 */
void oracle_leds_turn_led_off(uint8_t led_index);

/*!
 * \brief Set the leds brightness
 * \param brightness The new brightness
*/
void oracle_leds_set_leds_brightness(uint8_t brightness);

/*!
 * \brief Set the leds color
 * \param new_color The new color
*/
void oracle_leds_set_leds_color(const CRGB& new_color);

#endif /* #ifndef __include_oracle_rfid_leds_h */