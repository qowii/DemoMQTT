#ifndef __oracle_utils_h
#define __oracle_utils_h

#include <FastLED.h>

uint8_t oracle_utils_read_integer(const char *payload, char **endptr);
bool oracle_utils_read_bool(const char *payload, unsigned int length);
CRGB oracle_utils_read_color(const char *payload, unsigned int length);

#endif /* #ifndef __oracle_utils_h */