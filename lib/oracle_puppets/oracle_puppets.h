#ifndef __include_oracle_puppets_h
#define __include_oracle_puppets_h

#include <Arduino.h>
#include <Deneyap_Servo.h>

#include <AliceDefaultConfig.h>

#define ORACLE_PUPPETS_NUM_PUPPETS ALICE_PAPILLON_CONFIG_SERVO_MAX_NUM_DEFAULT

#define ORACLE_PUPPETS_PUPPET1_INIT_ANGLE ALICE_PAPILLON_CONFIG_SERVO1_INIT_ANGLE
#define ORACLE_PUPPETS_PUPPET1_MIN_ANGLE  ALICE_PAPILLON_CONFIG_SERVO1_MIN_ANGLE
#define ORACLE_PUPPETS_PUPPET1_MAX_ANGLE  ALICE_PAPILLON_CONFIG_SERVO1_MAX_ANGLE
#define ORACLE_PUPPETS_PUPPET1_GPIO_PIN   ALICE_PAPILLON_CONFIG_SERVO1_PIN

#define ORACLE_PUPPETS_PUPPET2_INIT_ANGLE ALICE_PAPILLON_CONFIG_SERVO2_INIT_ANGLE
#define ORACLE_PUPPETS_PUPPET2_MIN_ANGLE  ALICE_PAPILLON_CONFIG_SERVO2_MIN_ANGLE
#define ORACLE_PUPPETS_PUPPET2_MAX_ANGLE  ALICE_PAPILLON_CONFIG_SERVO2_MAX_ANGLE
#define ORACLE_PUPPETS_PUPPET2_GPIO_PIN   ALICE_PAPILLON_CONFIG_SERVO2_PIN

#define ORACLE_PUPPETS_PUPPET3_INIT_ANGLE ALICE_PAPILLON_CONFIG_SERVO3_INIT_ANGLE
#define ORACLE_PUPPETS_PUPPET3_MIN_ANGLE  ALICE_PAPILLON_CONFIG_SERVO3_MIN_ANGLE
#define ORACLE_PUPPETS_PUPPET3_MAX_ANGLE  ALICE_PAPILLON_CONFIG_SERVO3_MAX_ANGLE
#define ORACLE_PUPPETS_PUPPET3_GPIO_PIN   ALICE_PAPILLON_CONFIG_SERVO3_PIN

#define ORACLE_PUPPETS_PUPPET4_INIT_ANGLE ALICE_PAPILLON_CONFIG_SERVO4_INIT_ANGLE
#define ORACLE_PUPPETS_PUPPET4_MIN_ANGLE  ALICE_PAPILLON_CONFIG_SERVO4_MIN_ANGLE
#define ORACLE_PUPPETS_PUPPET4_MAX_ANGLE  ALICE_PAPILLON_CONFIG_SERVO4_MAX_ANGLE
#define ORACLE_PUPPETS_PUPPET4_GPIO_PIN   ALICE_PAPILLON_CONFIG_SERVO4_PIN

#define ORACLE_PUPPETS_PUPPET5_INIT_ANGLE ALICE_PAPILLON_CONFIG_SERVO5_INIT_ANGLE
#define ORACLE_PUPPETS_PUPPET5_MIN_ANGLE  ALICE_PAPILLON_CONFIG_SERVO5_MIN_ANGLE
#define ORACLE_PUPPETS_PUPPET5_MAX_ANGLE  ALICE_PAPILLON_CONFIG_SERVO5_MAX_ANGLE
#define ORACLE_PUPPETS_PUPPET5_GPIO_PIN   ALICE_PAPILLON_CONFIG_SERVO5_PIN

#define ORACLE_PUPPETS_PUPPET6_INIT_ANGLE ALICE_PAPILLON_CONFIG_SERVO6_INIT_ANGLE
#define ORACLE_PUPPETS_PUPPET6_MIN_ANGLE  ALICE_PAPILLON_CONFIG_SERVO6_MIN_ANGLE
#define ORACLE_PUPPETS_PUPPET6_MAX_ANGLE  ALICE_PAPILLON_CONFIG_SERVO6_MAX_ANGLE
#define ORACLE_PUPPETS_PUPPET6_GPIO_PIN   ALICE_PAPILLON_CONFIG_SERVO6_PIN

#define ORACLE_PUPPETS_PUPPET7_INIT_ANGLE ALICE_PAPILLON_CONFIG_SERVO7_INIT_ANGLE
#define ORACLE_PUPPETS_PUPPET7_MIN_ANGLE  ALICE_PAPILLON_CONFIG_SERVO7_MIN_ANGLE
#define ORACLE_PUPPETS_PUPPET7_MAX_ANGLE  ALICE_PAPILLON_CONFIG_SERVO7_MAX_ANGLE
#define ORACLE_PUPPETS_PUPPET7_GPIO_PIN   ALICE_PAPILLON_CONFIG_SERVO7_PIN

#define ORACLE_PUPPETS_PUPPET8_INIT_ANGLE ALICE_PAPILLON_CONFIG_SERVO8_INIT_ANGLE
#define ORACLE_PUPPETS_PUPPET8_MIN_ANGLE  ALICE_PAPILLON_CONFIG_SERVO8_MIN_ANGLE
#define ORACLE_PUPPETS_PUPPET8_MAX_ANGLE  ALICE_PAPILLON_CONFIG_SERVO8_MAX_ANGLE
#define ORACLE_PUPPETS_PUPPET8_GPIO_PIN   ALICE_PAPILLON_CONFIG_SERVO8_PIN

#define ORACLE_PUPPETS_NUM_BATCHS 4
#define ORACLE_PUPPETS_NUM_FRAMES 256

typedef struct oracle_puppets_config_s {
    uint8_t *gpio_motors;
    bool *status_motors;
    uint8_t num_gpios;
} oracle_puppets_config_t;

typedef struct oracle_puppets_frame_s {
    char buffer[ORACLE_PUPPETS_NUM_FRAMES];
    uint8_t index;
    bool in_use;
} oracle_puppets_frame_t;

typedef struct oracle_puppets_batchs_s {
    oracle_puppets_frame_t frames[ORACLE_PUPPETS_NUM_BATCHS];
    uint8_t running_index;
    uint8_t upload_index;
} oracle_puppets_batchs_t;

typedef struct oracle_puppets_config_servo_s {
    uint16_t angle_min;
    uint16_t angle_ini;
    uint16_t angle_max;
    uint16_t gpio_pin;
} struct oracle_puppets_config_servo_t;

typedef struct oracle_puppets_state_servo_s {
    Servo motor;
    uint16_t angle;
    uint8_t is_running;
    uint8_t pad0;
} oracle_puppert_state_servo_t;

typedef struct oracle_puppets_puppet_context_s {
    uint16_t index;
    uint8_t enable;
    uint8_t pad0;
    oracle_puppets_config_servo_t config;
    oracle_puppets_state_servo_t servos;
    oracle_puppets_batchs_t batchs;
} struct oracle_puppets_puppet_context_t;

typedef struct oracle_puppets_context_s {
    uint32_t num_puppets;
    oracle_puppets_puppet_context_t puppets[ORACLE_PUPPETS_NUM_PUPPETS];
} oracle_puppets_context_t;

#endif /* #ifndef __include_oracle_puppets_h */