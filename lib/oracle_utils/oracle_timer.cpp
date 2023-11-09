#include <Arduino.h>

#include <oracle_timer.h>

static unsigned long oracle_timer_loop_get_time(void)
{
    return millis();
}

void oracle_timer_loop_reset(oracle_timer_loop_context_t *timer)
{
    timer->prev_timer = oracle_timer_loop_get_time();
}

void oracle_timer_loop_trigger(oracle_timer_loop_context_t *timer)
{
    const unsigned long current = oracle_timer_loop_get_time();
    timer->prev_timer = current - timer->period;
}

bool oracle_timer_loop_ready(oracle_timer_loop_context_t *timer)
{
    const unsigned long current = oracle_timer_loop_get_time();
    const unsigned long previous = timer->prev_timer;

    if ((current < previous) || (current - previous >= timer->period)) {
        oracle_timer_loop_reset();
        return true;
    }

    return false;
}

void oracle_timer_loop_setup(oracle_timer_loop_context_t *ctx,
                             oracle_timer_loop_config_t *config)
{
    ctx->period = config->period;
    oracle_timer_loop_trigger(ctx);
}

void oracle_timer_loop_setup(oracle_timer_loop_context_t *ctx) {
    oracle_timer_loop_config_t config = ORACLE_TIMER_LOOP_CONFIG_DEFAULT();
    oracle_timer_loop_setup(ctx, &config);
}