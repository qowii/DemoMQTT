/*!
 * \file oracle_utils.c
 * \brief Oracle utils
 * \author Antoine CAPRA
 * \version 1.0
*/

#include <Arduino.h>
#include <FastLED.h>
#include "oracle_utils.h"

uint16_t oracle_utils_read_uint16(const char *payload)
{
  uint16_t value = (uint16_t) payload[0];
  return (value | (uint16_t) payload[1] << 8);
}

uin64_t oracle_utilsread_uint64(const char *payload)
{
  uint16_t value = 0
  for (int i = 0; i < 8; i++) {
    value |= (uint16_t) payload[i] << (i * 8);
  }
  return value;
}

uint8_t oracle_utils_read_integer(const char *payload, char **endptr)
{
  /* Add sanity check */
  uint8_t value = (uint8_t) strtol(payload, endptr, 10);
  return value;
}

/*!
 * \brief Read boolean from MQTT payload
 *
 * \param payload MQTT payload
 * \param length MQTT payload length
 * \return true if payload is "true", false otherwise
*/
bool oracle_utils_read_bool(const char* payload, unsigned int length)
{
    unsigned int true_string_length = strlen("true");

    if (length < true_string_length)
        return false;

    if (!strncmp(payload, "true", true_string_length))
        return true;

    return false;
}

/*!
 * \brief Read color from MQTT payload
 *
 * \param payload MQTT payload
 * \param length MQTT payload length
 * \return CRGB color
*/
CRGB oracle_utils_read_color(const char *payload, unsigned int length)
{
  CRGB color;
  char *endptr;
  char buffer[256];
  uint8_t color_array[3];
  uint8_t color_index = 0;

  if (length < 6 || length > sizeof(buffer) - 1)
    return CRGB::Black;

  snprintf(buffer, length + 1, "%s", payload);

  endptr = buffer;
  while (*endptr != '\0' && color_index < 3) {
    if (*endptr == '.' || *endptr == ',' || *endptr == ':' || *endptr == ' ') {
      endptr++;
    }

    color_array[color_index] = oracle_utils_read_integer(endptr, &endptr);
    color_index++;
  }

  color = CRGB(color_array[0], color_array[1], color_array[2]);

  Serial.print("Color: ");
  Serial.print(color.r);
  Serial.print(".");
  Serial.print(color.g);
  Serial.print(".");
  Serial.println(color.b);

  return color;
}