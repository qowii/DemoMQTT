#ifndef __oracle_utils_h
#define __oracle_utils_h

#include <FastLED.h>

/*!
 * \brief Read uint16_t from MQTT payload
 *
 * \param payload MQTT payload
 * \return uint16_t value
*/
uint16_t oracle_utils_read_uint16(const char *payload);

/*!
 * \brief Read uint64_t from MQTT payload
 *
 * \param payload MQTT payload
 * \return uint64_t value
*/
uin64_t oracle_utilsread_uint64(const char *payload)

/*!
 * \brief Read integer from MQTT payload
 *
 * \param payload MQTT payload
 * \param endptr Pointer to the end of the integer
 * \return Integer value
*/
uint8_t oracle_utils_read_integer(const char *payload, char **endptr);
bool oracle_utils_read_bool(const char *payload, unsigned int length);
CRGB oracle_utils_read_color(const char *payload, unsigned int length);


#endif /* #ifndef __oracle_utils_h */