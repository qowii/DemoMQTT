#include <Arduino.h>
#include <FastLED.h>

#include <oracle_parfum.h>

oracle_parfum_context_t oracle_parfum_context;

void oracle_parfum_setup(oracle_parfum_config_t *config)
{
    oracle_parfum_context_t *ctx = &oracle_parfum_context;

    pinMode(config->gpio_pin, OUTPUT);
    digitalWrite(config->gpio_pin, LOW);

    ctx->max_duration = config->max_duration;
    ctx->start_time = 0;
    ctx->end_time = 0;
    ctx->is_running = false;
}

void oracle_parfum_setup(void)
{
    oracle_parfum_config_t config = ORACLE_PARFUM_CONFIG_DEFAULT();
    oracle_parfum_setup(&config);
}

void oracle_parfum_enable(void)
{
    oracle_parfum_context_t *ctx = &oracle_parfum_context;

    ctx->start_time = millis();
    ctx->end_time = ctx->start_time + ctx->max_duration;
    ctx->is_running = true;

    digitalWrite(ORACLE_PARFUM_PIN, HIGH);
}

void oracle_parfum_disable(void)
{
    oracle_parfum_context_t *ctx = &oracle_parfum_context;

    ctx->start_time = 0;
    ctx->end_time = 0;
    ctx->is_running = false;

    digitalWrite(ORACLE_PARFUM_PIN, LOW);
}

void oracle_parfum_set_duration(oracle_parfum_context_t *ctx,
                                const int duration)
{
    ctx->max_duration = duration;
}

void oracle_parfum_set_duration(const int duration)
{
    oracle_parfum_context_t *ctx = &oracle_parfum_context;
    oracle_parfum_set_duration(ctx, duration);
}

static bool oracle_parfum_get_state(oracle_parfum_context_t *ctx)
{
    return ctx->is_running;
}

static bool oracle_parfum_get_state(void)
{
    oracle_parfum_context_t *ctx = &oracle_parfum_context;
    return oracle_parfum_get_state(ctx);
}

static bool oracle_parfum_check_timer(void)
{
    /* Manage overflow */
    if (oracle_parfum_context.is_running) {
        unsigned long now = millis();
        if (now < oracle_parfum_context.end_time) {
            return true; /* Nothing to do */
        }
    }

    /* Force disable every time */
    oracle_parfum_disable();
    return false;
}

bool oracle_parfum_loop(void)
{
    EVERY_N_MILLISECONDS(ORACLE_PARFUM_LOOP_DELAY) {
        return oracle_parfum_check_timer();
    }

    return oracle_parfum_get_state();
}