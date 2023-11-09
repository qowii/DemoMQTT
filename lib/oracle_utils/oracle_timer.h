#ifndef __include_oracle_timer
#define __include_oracle_timer

#include <AliceDefaultConfig.h>

#define ORACLE_TIMER_LOOP_DELAY ALICE_ESP32_CONFIG_DELAY

#define ORACLE_TIMER_LOOP_CONFIG_DEFAULT() {\
    .period = ORACLE_TIMER_LOOP_DELAY\
}

typedef struct oracle_timer_loop_config_s {
    unsigned long period;
} oracle_timer_loop_config_t;

typedef struct oracle_timer_loop_context_s {
    unsigned long prev_timer;
    unsigned long period;
} oracle_timer_loop_context_t;

void oracle_timer_loop_reset(oracle_timer_loop_context_t *timer);
void oracle_timer_loop_trigger(oracle_timer_loop_context_t *timer);
bool oracle_timer_loop_ready(oracle_timer_loop_context_t *timer);

void oracle_timer_loop_setup(oracle_timer_loop_context_t *ctx,
                             oracle_timer_loop_config_t *config);
void oracle_timer_loop_setup(oracle_timer_loop_context_t *ctx);

#endif /* #ifndef __include_oracle_timer */