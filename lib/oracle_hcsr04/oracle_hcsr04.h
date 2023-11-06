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
#define ORACLE_HCSR04_SPEED_OF_SOUND    ALICE_HCSR04_CONFIG_SPEED_OF_SOUND

#define ORACLE_HCSR04_CONFIG_DEFAULT() {\
    .gpio_trigger = ORACLE_HCSR04_GPIO_TRIGGER,\
    .gpio_echo = ORACLE_HCSR04_GPIO_ECHO,\
    .num_repeats = ORACLE_HCSR04_NUM_REPEATS,\
    .pulse_delay = TRIG_PULSE_DURATION_US,\
    .max_value = ORACLE_HCSR04_MAX_VALUE,\
    .max_distance = ALICE_HCSR04_CONFIG_MAX_DISTANCE,\
    .speed_sound = ORACLE_HCSR04_SPEED_OF_SOUND\
}

typedef struct oracle_hcsr04_config_s {
    uint8_t gpio_trigger;
    uint8_t gpio_echo;
    uint8_t num_repeats;
    uint8_t pulse_delay;
    uint8_t max_value;
    uint8_t max_distance;
    uint8_t pad[1];
    int speed_sound;
} oracle_hcsr04_config_t;

typedef struct oracle_hcsr04_context_s {
    uint8_t gpio_trigger;
    uint8_t gpio_echo;
    uint8_t num_repeats;
    uint8_t pulse_delay;
    uint8_t max_value;
    uint8_t max_distance;
    uint8_t distance;
    int speed_sound;
} oracle_hcsr04_context_t;

void oracle_hcsr04_setup(void);
void oracle_hcsr04_setup(oracle_hcsr04_config_t *config);

void oracle_hcsr04_get_distance(void);
uint8_t oracle_hcsr04_get_distance(oracle_hcsr04_context_t *ctx);

#endif /* #ifndef __include_oracle_hcsr04_h */