#ifndef __oracle_rc522_mqtt_h
#define __oracle_rc522_mqtt_h

#include <oracle_rc522.h>

bool oracle_rc522_mqtt_loop(void);

void oracle_rc522_mqtt_setup(void);
void oracle_rc522_mqtt_setup(oracle_rc522_config_t *config);

#endif /* #ifndef __oracle_rc522_mqtt_h */