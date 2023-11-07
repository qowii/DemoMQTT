#ifndef __include_oracle_puppets_h
#define __include_oracle_puppets_h

#include <Arduino.h>
#include <Deneyap_Servo.h>

#include <oracle_puppets_config.h>

typedef struct oracle_puppets_config_s {
    uint8_t *gpio_motors;
    bool *status_motors;
    uint8_t num_gpios;
} oracle_puppets_config_t;

typedef struct oracle_puppets_frame_s {
    uint8_t ready;
    uint8_t pad0[3];
    char buffer[ORACLE_PUPPETS_NUM_FRAMES];
} oracle_puppets_frame_t;

typedef struct oracle_puppets_batchs_s {
    uint8_t running_index;
    uint8_t upload_index;
    uint16_t frame_index;
    char *next_angle;
    oracle_puppets_frame_t frames[ORACLE_PUPPETS_NUM_BATCHS];
} oracle_puppets_batchs_t;

typedef struct oracle_puppets_config_servo_s {
    uint16_t angle_min;
    uint16_t angle_ini;
    uint16_t angle_max;
    uint16_t gpio_pin;
} oracle_puppets_config_servo_t;

typedef struct oracle_puppets_servo_s {
    Servo motor;
    uint16_t angle;
    uint8_t enable;
    uint8_t pad0;
} oracle_puppets_servo_t;

typedef struct oracle_puppets_puppet_context_s {
    uint16_t index;
    uint8_t enable;
    uint8_t pad0;
    oracle_puppets_config_servo_t config;
    oracle_puppets_servo_t servos;
    oracle_puppets_batchs_t batchs;
} oracle_puppets_puppet_context_t;

typedef struct oracle_puppets_context_s {
    uint32_t num_puppets;
    oracle_puppets_puppet_context_t puppets[ORACLE_PUPPETS_NUM_PUPPETS];
} oracle_puppets_context_t;

void oracle_puppets_loop(void);
void oracle_puppets_setup(void);

#endif /* #ifndef __include_oracle_puppets_h */