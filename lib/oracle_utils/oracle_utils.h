#ifndef __oracle_utils_h
#define __oracle_utils_h

#include <FastLED.h>

typedef struct oracle_utils_rbg_s {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t pad0;
} oracle_utils_rbg_t;

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
uint64_t oracle_utilsread_uint64(const char *payload);

/*!
 * \brief Read integer from MQTT payload
 *
 * \param payload MQTT payload
 * \param endptr Pointer to the end of the integer
 * \return Integer value
*/
uint8_t oracle_utils_read_integer(const char *payload, char **endptr);

bool oracle_utils_read_bool(const char *payload, unsigned int length);

bool oracle_utils_read_color(const char *payload, unsigned int length, oracle_utils_rbg_t *color);

#endif /* #ifndef __oracle_utils_h */