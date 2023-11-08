#ifndef __include_oracle_hcsr04_h
#define __include_oracle_hcsr04_h

#include <Arduino.h>
#include <AliceDefaultConfig.h>

#define TRIG_PULSE_DURATION_US 10

#define ORACLE_HCSR04_GPIO_TRIGGER      ALICE_HCSR04_CONFIG_TRIGGER_PIN
#define ORACLE_HCSR04_GPIO_ECHO         ALICE_HCSR04_CONFIG_ECHO_PIN
#define ORACLE_HCSR04_NUM_REPEATS       ALICE_HCSR04_CONFIG_REPEAT
#define ORACLE_HCSR04_PULSE_DELAY       TRIG_PULSE_DURATION_US
#define ORACLE_HCSR04_MAX_VALUE         ALICE_HCSR04_CONFIG_MAX_VALUE
#define ORACLE_HCSR04_MAX_DISTANCE      ALICE_HCSR04_CONFIG_MAX_DISTANCE
#define ORACLE_HCSR04_LOOP_DELAY        ALICE_ESP32_CONFIG_DELAY
#define ORACLE_HCSR04_SPEED_OF_SOUND    ALICE_HCSR04_CONFIG_SPEED_OF_SOUND

#define ORACLE_HCSR04_UNDEF_VALUE       UINT8_MAX
#define ORACLE_HCSR04_SKIP_VALUE        (ORACLE_HCSR04_UNDEF_VALUE - 1)

#define ORACLE_HCSR04_CONFIG_DEFAULT() {\
    .gpio_trigger = ORACLE_HCSR04_GPIO_TRIGGER,\
    .gpio_echo = ORACLE_HCSR04_GPIO_ECHO,\
    .num_repeats = ORACLE_HCSR04_NUM_REPEATS,\
    .pulse_delay = TRIG_PULSE_DURATION_US,\
    .max_distance = ALICE_HCSR04_CONFIG_MAX_DISTANCE,\
    .pad0 = 0,\
    .pad1 = 0,\
    .pad2 = 0,\
    .max_value = ORACLE_HCSR04_MAX_VALUE,\
    .delay = ORACLE_HCSR04_LOOP_DELAY,\
    .speed_sound = ORACLE_HCSR04_SPEED_OF_SOUND\
}

typedef struct oracle_hcsr04_config_s {
    uint8_t gpio_trigger;
    uint8_t gpio_echo;
    uint8_t num_repeats;
    uint8_t pulse_delay;
    uint8_t max_distance;
    uint8_t pad0;
    uint8_t pad1;
    uint8_t pad2;
    int max_value;
    unsigned long delay;
    double speed_sound;
} oracle_hcsr04_config_t;

typedef struct oracle_hcsr04_context_s {
    uint8_t gpio_trigger;
    uint8_t gpio_echo;
    uint8_t num_repeats;
    uint8_t pulse_delay;
    uint8_t max_distance;
    uint8_t distance;
    uint8_t pad[2];
    int max_value;
    unsigned long timer;
    unsigned long delay;
    double speed_sound;
} oracle_hcsr04_context_t;

/*!
 * @brief Setup the HCSR04 sensor
 */
void oracle_hcsr04_setup(void);

/*!
 * @brief Setup the HCSR04 sensor
 * @param config The configuration
 */
void oracle_hcsr04_setup(oracle_hcsr04_config_t *config);

/*!
 * @brief Get the distance from the HCSR04 sensor
 * @return The distance in cm
 */
uint8_t oracle_hcsr04_get_distance(void);

uint8_t oracle_hcsr04_loop(void);

#endif /* #ifndef __include_oracle_hcsr04_h */