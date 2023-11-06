#include <Arduino.h>
#include <FastLED.h>

#include <oracle_screen.h>

oracle_screen_context_t oracle_screen_context;

static void oracle_screen_open(oracle_screen_context_t *ctx)
{
    if (ctx->state == ORACLE_SCREEN_STATE_OPEN) {
        return;
    }

    digitalWrite(ctx->enable_rpwm, HIGH);
    digitalWrite(ctx->enable_lpwm, HIGH);
    analogWrite(ctx->rpwm_output, 250);
    ctx->state = ORACLE_SCREEN_STATE_OPENING;
}

void oracle_screen_open(void)
{
    oracle_screen_context_t *ctx = &oracle_screen_context;
    oracle_screen_open(ctx);
}

static void oracle_screen_close(oracle_screen_context_t *ctx)
{
    if (ctx->state == ORACLE_SCREEN_STATE_CLOSED) {
        return;
    }

    digitalWrite(ctx->enable_rpwm, HIGH);
    digitalWrite(ctx->enable_lpwm, HIGH);
    analogWrite(ctx->lpwm_output, 255);
    ctx->state = ORACLE_SCREEN_STATE_CLOSING;
}

void oracle_screen_close(void)
{
    oracle_screen_context_t *ctx = &oracle_screen_context;
    oracle_screen_close(ctx);
}

static void oracle_screen_stop(oracle_screen_context_t *ctx)
{
    analogWrite(ctx->rpwm_output, 0);
    analogWrite(ctx->lpwm_output, 0);
    digitalWrite(ctx->enable_rpwm, LOW);
    digitalWrite(ctx->enable_lpwm, LOW);
}

void oracle_screen_stop(void)
{
    oracle_screen_context_t *ctx = &oracle_screen_context;
    oracle_screen_stop(ctx);
}

void oracle_screen_open(void)
{
    oracle_screen_context_t *ctx = &oracle_screen_context;
    oracle_screen_open(ctx);
}

static bool oracle_screen_is_open(oracle_screen_context_t *ctx)
{
    if (ctx->state == ORACLE_SCREEN_STATE_OPEN) {
        return true;
    }

    if (ctx->state != ORACLE_SCREEN_STATE_OPENING) {
        return false;
    }

    if (!digitalRead(ctx->fin_de_course_open) == LOW) {
        return false;
    }

    ctx->state = ORACLE_SCREEN_STATE_OPEN;
    oracle_screen_stop(ctx);
    return true;
}

static bool oracle_screen_is_open(void)
{
    oracle_screen_context_t *ctx = &oracle_screen_context;
    return oracle_screen_is_open(ctx);
}

static bool oracle_screen_is_closed(oracle_screen_context_t *ctx)
{
    if (ctx->state == ORACLE_SCREEN_STATE_CLOSED) {
        return true;
    }

    if (ctx->state != ORACLE_SCREEN_STATE_CLOSING) {
        return false;
    }

    if (!digitalRead(ctx->fin_de_course_closed) == LOW) {
        return false;
    }

    ctx->state = ORACLE_SCREEN_STATE_CLOSED;
    oracle_screen_stop(ctx);
    return true;
}

static bool oracle_screen_is_closed(void)
{
    oracle_screen_context_t *ctx = &oracle_screen_context;
    return oracle_screen_is_closed(ctx);
}

static uint8_t oracle_screen_get_state(oracle_screen_context_t *ctx)
{
    return ctx->state;
}

static uint8_t oracle_screen_get_state(void)
{
    oracle_screen_context_t *ctx = &oracle_screen_context;
    return oracle_screen_get_state(ctx);
}

static uint8_t oracle_screen_update(oracle_screen_context_t *ctx)
{
    oracle_screen_is_open(ctx);
    oracle_screen_is_closed(ctx);
    return oracle_screen_get_state();
}

static bool oracle_screen_update(void)
{
    oracle_screen_context_t *ctx = &oracle_screen_context;
    return oracle_screen_update(ctx);
}

static uint8_t oracle_screen_loop(oracle_screen_context_t *ctx)
{
    EVERY_N_MILLISECONDS(ORACLE_SCREEN_LOOP_DELAY) {
        return oracle_screen_update(ctx);
    }

    return oracle_screen_get_state(ctx);
}

uint8_t oracle_screen_loop(void)
{
    oracle_screen_context_t *ctx = &oracle_screen_context;
    return oracle_screen_loop(ctx);
}

void oracle_screen_setup(oracle_screen_config_t *config)
{
    oracle_screen_context_t *ctx = &oracle_screen_context;
    
    ctx->rpwm_output = config->rpwm_output;
    ctx->lpwm_output = config->lpwm_output;
    ctx->enable_rpwm = config->enable_rpwm;
    ctx->enable_lpwm = config->enable_lpwm;
    ctx->fin_de_course_open = config->fin_de_course_open;
    ctx->fin_de_course_closed = config->fin_de_course_closed;
    ctx->state = ORACLE_SCREEN_STATE_STOPPED;
        
    pinMode(config->rpwm_output, OUTPUT);
    pinMode(config->lpwm_output, OUTPUT);
    pinMode(config->enable_rpwm, OUTPUT);
    pinMode(config->enable_lpwm, OUTPUT);
    pinMode(config->fin_de_course_open, INPUT_PULLUP);
    pinMode(config->fin_de_course_closed, INPUT_PULLUP);
    
    analogWrite(config->rpwm_output, 0);
    analogWrite(config->lpwm_output, 0);

    digitalWrite(config->enable_rpwm, HIGH);
    digitalWrite(config->enable_lpwm, HIGH);
}

void oracle_screen_setup(void)
{
    oracle_screen_config_t config = ORACLE_SCREEN_CONFIG_DEFAULT();
    oracle_screen_setup(&config);
}