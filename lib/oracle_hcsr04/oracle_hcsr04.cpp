
#include <Arduino.h>

#include <oracle_hcsr04.h>
#include <oracle_timer.h>


static oracle_hcsr04_context_t oracle_hcsr04_context;
static oracle_timer_loop_context_t oracle_hcsr04_timer_context;

static uint8_t oracle_hcsr04_get_distance(oracle_hcsr04_context_t *hcsr04_ctx)
{
    float avg, sum;
    int valid_measure;
    static bool once = false;

    sum = 0.0;
    valid_measure = 0;

    for (int i = 0; i < hcsr04_ctx->num_repeats; i++)
    {
        unsigned long start_timer, end_timer;

        start_timer = millis();
        digitalWrite(hcsr04_ctx->gpio_trigger, LOW);
        delayMicroseconds(2);
        digitalWrite(hcsr04_ctx->gpio_trigger, HIGH);
        delayMicroseconds(hcsr04_ctx->pulse_delay);
        digitalWrite(hcsr04_ctx->gpio_trigger, LOW);
        const long ultrason_duration = pulseIn(hcsr04_ctx->gpio_echo , HIGH);
        end_timer = millis();

        if (!once) {
            char buffer[256];
            unsigned long elapsed_timer;
            elapsed_timer = end_timer - start_timer;

            snprintf(buffer, sizeof(buffer), "HCSR04 repead elapsed: %lu ms", elapsed_timer);
            Serial.println(buffer);
            once = true;
        }

        if (ultrason_duration > 0 && ultrason_duration < hcsr04_ctx->max_value)
        {
            sum += ultrason_duration / hcsr04_ctx->speed_sound / 2;
            valid_measure++;
        }
    }

    avg = sum / (float) valid_measure;

    if (avg > 0 && avg < hcsr04_ctx->max_distance) {
        hcsr04_ctx->distance = avg;
    } else {
        hcsr04_ctx->distance = ORACLE_HCSR04_UNDEF_VALUE;
    }

    return hcsr04_ctx->distance;
}

uint8_t oracle_hcsr04_get_distance(void)
{
    oracle_hcsr04_context_t *hcsr04_ctx = &oracle_hcsr04_context;
    return oracle_hcsr04_get_distance(hcsr04_ctx);
}

static uint8_t oracle_hcsr04_loop(oracle_hcsr04_context_t *hcsr04_ctx)
{
    oracle_timer_loop_context_t *timer_loop_ctx = &oracle_hcsr04_timer_context;

    if (!oracle_timer_loop_ready(timer_loop_ctx)) {
        return ORACLE_HCSR04_SKIP_VALUE;
    }

    return oracle_hcsr04_get_distance(hcsr04_ctx);
}

uint8_t oracle_hcsr04_loop(void)
{
    oracle_hcsr04_context_t *hcsr04_ctx = &oracle_hcsr04_context;
    return oracle_hcsr04_loop(hcsr04_ctx);
}

void oracle_hcsr04_setup(oracle_hcsr04_config_t *config)
{
    /* Only one context per esp32 in this version */
    oracle_hcsr04_context_t *hcsr04_ctx = &oracle_hcsr04_context;
    oracle_timer_loop_context_t *timer_loop_ctx = &oracle_hcsr04_timer_context;

    oracle_timer_loop_setup(timer_loop_ctx);

    hcsr04_ctx->gpio_trigger = config->gpio_trigger;
    hcsr04_ctx->gpio_echo = config->gpio_echo;
    hcsr04_ctx->num_repeats = config->num_repeats;
    hcsr04_ctx->pulse_delay = config->pulse_delay;
    hcsr04_ctx->max_value = config->max_value;
    hcsr04_ctx->max_distance = config->max_distance;
    hcsr04_ctx->speed_sound = config->speed_sound;
    hcsr04_ctx->distance = UINT8_MAX;

    /* La broche TRIGGER doit être à LOW au repos */
    pinMode(hcsr04_ctx->gpio_trigger, OUTPUT);
    digitalWrite(hcsr04_ctx->gpio_trigger, LOW);

    pinMode(hcsr04_ctx->gpio_echo, INPUT);
}

void oracle_hcsr04_setup(void)
{
    oracle_hcsr04_config_t config = ORACLE_HCSR04_CONFIG_DEFAULT();
    oracle_hcsr04_setup(&config);
}
