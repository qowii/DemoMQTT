#ifndef __include_oracle_parfum_h
#define __include_oracle_parfum_h

#include <AliceDefaultConfig.h>

#define ORACLE_PARFUM_PIN           ALICE_PARFUM_CONFIG_PIN
#define ORACLE_PARFUM_MAX_TIME      ALICE_PARFUM_CONFIG_MAX_TIME
#define ORACLE_PARFUM_LOOP_DELAY    ALICE_ESP32_CONFIG_DELAY

#define ORACLE_PARFUM_CONFIG_DEFAULT() {\
    .gpio_pin = ORACLE_PARFUM_PIN,\
    .max_duration = ORACLE_PARFUM_MAX_TIME\
}

typedef struct oracle_parfum_config_s {
    int gpio_pin;
    int max_duration;
} oracle_parfum_config_t;

typedef struct oracle_parfum_context_s {
    int max_duration;
    int duration;
    unsigned long start_time;
    unsigned long end_time;
    bool is_running;
} oracle_parfum_context_t;

void oracle_parfum_setup(void);
void oracle_parfum_setup(oracle_parfum_config_t *config);

bool oracle_parfum_loop(void);

void oracle_parfum_set_duration(oracle_parfum_context_t *ctx,
                                const int duration);
void oracle_parfum_set_duration(const int duration);

void oracle_parfum_enable(void);
void oracle_parfum_disable(void);

#endif /* #ifndef __include_oracle_parfum_h */