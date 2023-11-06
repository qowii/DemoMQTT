#ifndef __include_oracle_screen_h
#define __include_oracle_screen_h

#include <Arduino.h>

#include <AliceDefaultConfig.h>

#define ORACLE_SCREEN_RPWN_OUTPUT 32
#define ORACLE_SCREEN_LPWN_OUTPUT 33
#define ORACLE_SCREEN_ENABLE_RPWM 25
#define ORACLE_SCREEN_ENABLE_LPWM 26
#define ORACLE_SCREEN_FIN_DE_COURSE_OPEN 14
#define ORACLE_SCREEN_FIN_DE_COURSE_CLOSED 27

#define ORACLE_SCREEN_LOOP_DELAY ALICE_ESP32_CONFIG_DELAY

#define ORACLE_SCREEN_CONFIG_DEFAULT() {\
    .rpwm_output = ORACLE_SCREEN_RPWN_OUTPUT,\
    .lpwm_output = ORACLE_SCREEN_LPWN_OUTPUT,\
    .enable_rpwm = ORACLE_SCREEN_ENABLE_RPWM,\
    .enable_lpwm = ORACLE_SCREEN_ENABLE_LPWM,\
    .fin_de_course_open = ORACLE_SCREEN_FIN_DE_COURSE_OPEN,\
    .fin_de_course_closed = ORACLE_SCREEN_FIN_DE_COURSE_CLOSED\
    };

typedef enum oracle_screen_state_e {
  ORACLE_SCREEN_STATE_OPEN,
  ORACLE_SCREEN_STATE_CLOSED,
  ORACLE_SCREEN_STATE_OPENING,
  ORACLE_SCREEN_STATE_CLOSING,
  ORACLE_SCREEN_STATE_STOPPED
} oracle_screen_state_t;

typedef struct oracle_screen_config_s {
  uint8_t rpwm_output;
  uint8_t lpwm_output;
  uint8_t enable_rpwm;
  uint8_t enable_lpwm;
  uint8_t fin_de_course_open;
  uint8_t fin_de_course_closed;
} oracle_screen_config_t;

typedef struct oracle_screen_context_s {
  uint8_t rpwm_output;
  uint8_t lpwm_output;
  uint8_t enable_rpwm;
  uint8_t enable_lpwm;
  uint8_t fin_de_course_open;
  uint8_t fin_de_course_closed;
  oracle_screen_state_t state;
} oracle_screen_context_t;

void oracle_screen_setup(void);
void oracle_screen_setup(oracle_screen_config_t *config);

void oracle_screen_open(void);
void oracle_screen_close(void);
void oracle_screen_stop(void);

uint8_t oracle_screen_loop(void);

#endif /* #ifndef __include_oracle_screen_h */