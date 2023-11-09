/*!
 * @file oracle_leds_mqtt.cpp
 * @brief Oracle leds MQTT
 * @version 1.0
 * @date 2021-04-30
 * @note Created by Antoine CAPRA
 */
#include <FastLED.h>

#include <oracle_leds.h>
#include <oracle_mqtt.h>
#include <oracle_utils.h>
#include <oracle_leds_mqtt.h>

/*!
 * @brief Publish leds status
 * @param status Leds status
 */
static void oracle_leds_mqtt_publish_leds_status(bool status)
{
    const char *topic = "/esp32/leds/get/status";
    if (status)
        oracle_mqtt_publish(topic, "true");
    else
        oracle_mqtt_publish(topic, "false");
}

/*!
 * @brief Change status of all leds
 * @param topic MQTT topic
 * @param payload MQTT payload
 * @param length MQTT payload length
*/
static bool oracle_leds_mqtt_set_leds_status(const char *topic, byte *payload, unsigned int length)
{
  bool status;
  const char *expected_topic = "/esp32/leds/set/status";

  if (strncmp(topic, expected_topic, strlen(expected_topic)))
    return false;

  status = oracle_utils_read_bool((char *)payload, length);
  if (status) {
    oracle_leds_turn_leds_on();
  } else {
    oracle_leds_turn_leds_off();
  }

  oracle_leds_mqtt_publish_leds_status(status);

  return true;
}

/*!
 * @brief Publish leds brightness
 * @param brightness Leds brightness
*/
static void oracle_leds_mqtt_publish_leds_brightness(uint8_t brightness)
{
    char payload[4];
    const char *topic = "/esp32/leds/get/brightness";
    sprintf(payload, "%d", brightness);
    oracle_mqtt_publish(topic, payload);
}

/*!
 * @brief Change brightness of all leds
 * @param topic MQTT topic
 * @param payload MQTT payload
 * @param length MQTT payload length
*/
static bool oracle_leds_mqtt_set_leds_brightness(const char *topic, byte *payload, unsigned int length)
{
  char *endptr;
  char buffer[256];
  uint8_t brightness;
  const char *expected_topic = "/esp32/leds/set/brightness";

  if (strncmp(topic, expected_topic, strlen(expected_topic)))
    return false;

  if (length < 1 || length > sizeof(buffer))
    return false;

  snprintf(buffer, length + 1, "%s", payload);
  brightness = oracle_utils_read_integer(buffer, &endptr);
  oracle_leds_set_leds_brightness(brightness);
  oracle_leds_mqtt_publish_leds_brightness(brightness);

  return true;
}

/*!
 * @brief Change color of leds
 * @param topic MQTT topic
 * @param payload MQTT payload
 * @param length MQTT payload length
 */
static bool oracle_leds_mqtt_set_leds_color(const char *topic, byte *payload, unsigned int length)
{
  CRGB color = CRGB::Black;
  oracle_utils_rbg_t raw_color;

  const char *expected_topic = "/esp32/leds/set/color";

  if (strncmp(topic, expected_topic, strlen(expected_topic)))
    return false;

  if (length < 6)
    return false;

  if (oracle_utils_read_color((char *) payload, length, &raw_color)) {
    color = CRGB(raw_color.red, raw_color.green, raw_color.blue);
  }

  oracle_leds_set_leds_color(color);

  return true;
}

/*!
 * @brief Publish leds status
 * @param led_index Led index
 * @param status Leds status
 */
static void oracle_leds_mqtt_publish_led_status(uint8_t led_index, bool status)
{
    char topic[64];
    snprintf(topic, 64, "/esp32/led/%d/get/status", led_index);
    if (status)
        oracle_mqtt_publish(topic, "true");
    else
        oracle_mqtt_publish(topic, "false");
}

/*!
 * \brief Get the led index from the topic
 * \param topic The topic
 * \param payload The payload
 * \param length The payload length
 * \return The led index
 */
static uint8_t oracle_leds_mqtt_get_led_index(const char *topic)
{
  char *token;
  char buffer[256];
  uint8_t token_index, led_index;
  const char *expected_tokens[] = {"esp32", "led", "", "set", "status"};

  if (strlen(topic) > sizeof(buffer) - 1)
    return UINT8_MAX;

  /* strtok modify buffer, we duplicate topic to avoid issues */
  snprintf(buffer, strlen(topic) + 1, "%s", topic);

  token_index = 0;
  led_index = UINT8_MAX;
  token = strtok((char *) buffer, "/");

  while (token != NULL)
  {
    uint8_t length = strlen(expected_tokens[token_index]);
    if (!length || !strcmp(token, expected_tokens[token_index])) {
        if (!length) {
          led_index = oracle_utils_read_integer(token, NULL);
        }
        token = strtok(NULL, "/");
        token_index++;
        continue;
    }

    return UINT8_MAX;
  }

  return led_index;
}

/*!
 * @brief Change status of a led
 * @param topic MQTT topic
 * @param payload MQTT payload
 * @param length MQTT payload length
 * @return true if topic was handled, false otherwise
 */
static bool oracle_leds_mqtt_set_led_status(const char *topic, byte *payload, unsigned int length)
{
    bool status;
    uint8_t led_index;

    led_index = oracle_leds_mqtt_get_led_index(topic);
    if (led_index == UINT8_MAX)
        return false;

    status = oracle_utils_read_bool((char *)payload, length);
    if (status) {
      oracle_leds_turn_led_on(led_index);
    } else {
      oracle_leds_turn_led_off(led_index);
    }

    oracle_leds_mqtt_publish_led_status(led_index, status);
    return true;
}

/*!
 * @brief Apply leds status
 * @param topic MQTT topic
 * @param payload MQTT payload
 * @param length MQTT payload length
 * @return true if topic was handled, false otherwise
 */
static bool oracle_leds_mqtt_set_leds_apply(const char *topic, byte *payload, unsigned int length)
{
  bool status;
  const char *expected_topic = "/esp32/leds/set/apply";

  if (strncmp(topic, expected_topic, strlen(expected_topic))) {
    return false;
  }

  status = oracle_utils_read_bool((char *)payload, length);
  if (status) {
    oracle_leds_apply();
  }

  return true;
}

bool oracle_leds_mqtt_callback(const char *topic, byte *payload, unsigned int length)
{
  if (oracle_leds_mqtt_set_leds_status(topic, payload, length)) {
    return true;
  }

  if (oracle_leds_mqtt_set_leds_brightness(topic, payload, length)) {
    return true;
  }

  if (oracle_leds_mqtt_set_leds_color(topic, payload, length)) {
    return true;
  }

  if (oracle_leds_mqtt_set_led_status(topic, payload, length)) {
    return true;
  }

  if (oracle_leds_mqtt_set_leds_apply(topic, payload, length)) {
    return true;
  }


  return false;
}

void oracle_leds_mqtt_subscribe(void)
{
  char topic[64];
  const uint8_t num_topics = 3;
  const char *topics[] = {"status", "color", "brightness"};
  const uint8_t num_leds = oracle_rfid_leds_get_num_leds();

  if (0 == num_leds)
    return;

  snprintf(topic, 64, "%s", "/esp32/leds/set/apply");
  oracle_mqtt_subscribe(topic);

  for (int i = 0; i < num_topics; i++) {
    snprintf(topic, 64, "%s/%s", "/esp32/leds/set", topics[i]);
    oracle_mqtt_subscribe(topic);
  }

  for (int i = 0; i < num_leds; i++) {
    for (int j = 0; j < num_topics; j++) {
      snprintf(topic, 64, "%s/%d/%s/%s", "/esp32/led", i, "set", topics[j]);
      oracle_mqtt_subscribe(topic);
    }
  }
}

bool oracle_leds_mqtt_loop(void)
{
  EVERY_N_MILLISECONDS(ORACLE_LEDS_MQTT_LOOP_DELAY) {
    return true;
  }

  return true;
}

void oracle_leds_mqtt_setup(void)
{
  oracle_leds_setup();
}