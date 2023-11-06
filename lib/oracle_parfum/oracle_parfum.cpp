#include <Arduino.h>
#include <oracle_parfum.h>

void oracle_parfum_init(oracle_parfum_config_t *config)
{
    pinMode(config->gpio_pin, OUTPUT);
    digitalWrite(config->gpio_pin, LOW);
}

void oracle_parfum_init(void)
{
    oracle_parfum_config_t config = ORACLE_PARFUM_CONFIG_DEFAULT();
    oracle_parfum_init(&config);
}