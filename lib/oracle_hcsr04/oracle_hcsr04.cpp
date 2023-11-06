
#include <Arduino.h>
#include <oracle_hcsr04.h>

oracle_hcsr04_context_t oracle_hcsr04_context;

uint8_t oracle_hcsr04_get_distance(oracle_hcsr04_context_t *ctx)
{
    float avg, sum;
    int valid_measure;
    static bool once = false;

    sum = 0.0;
    valid_measure = 0;

    for (int i = 0; i < ctx->num_repeats; i++)
    {
        unsigned long start_timer, end_timer;

        start_timer = millis();
        digitalWrite(ctx->gpio_trigger, LOW);
        delayMicroseconds(2);
        digitalWrite(ctx->gpio_trigger, HIGH);
        delayMicroseconds(ctx->pulse_delay);
        digitalWrite(ctx->gpio_trigger, LOW);
        const long ultrason_duration = pulseIn(ctx->gpio_echo , HIGH);
        end_timer = millis();

        if (!once) {
            char buffer[256];
            unsigned long elapsed_timer;
            elapsed_timer = end_timer - start_timer;

            snprintf(buffer, sizeof(buffer), "HCSR04 repead elapsed: %lu ms", elapsed_timer);
            Serial.println(buffer);
            WebSerial.println(buffer);
            once = true;
        }

        if (ultrason_duration > 0 && ultrason_duration < ctx->max_value)
        {
            sum += ultrason_duration / ctx->speed_sound / 2;
            valid_measure++;
        }
    }

    avg = sum / (float) valid_measure;
    ctx->distance = (avg > 0 && avg < ctx->max_distance) ? avg : UINT8_MAX;

    return ctx->distance;
}

uint8_t oracle_hcsr04_get_distance(void)
{
    oracle_hcsr04_context_t *ctx = &oracle_hcsr04_context;
    oracle_hcsr04_get_distance(ctx);
}

void oracle_hcsr04_setup(oracle_hcsr04_config_t *config)
{
    /* Only one context per esp32 in this version */
    oracle_hcsr04_context_t *ctx = &oracle_hcsr04_context;

    ctx->gpio_trigger = config->gpio_trigger;
    ctx->gpio_echo = config->gpio_echo;
    ctx->num_repeats = config->num_repeats;
    ctx->pulse_delay = config->pulse_delay;
    ctx->max_value = config->max_value;
    ctx->max_distance = config->max_distance;
    ctx->distance = config->distance;
    ctx->speed_sound = config->speed_sound;
    ctx->distance = UINT8_MAX;

    /* La broche TRIGGER doit être à LOW au repos */
    pinMode(ctx->gpio_trigger, OUTPUT);
    digitalWrite(ctx->gpio_trigger, LOW);

    pinMode(ctx->gpio_echo, INPUT);
}

void oracle_hcsr04_setup(void)
{
    oracle_hcsr04_config_t config = ORACLE_HCSR04_CONFIG_DEFAULT();
    oracle_hcsr04_setup(&config);
}
