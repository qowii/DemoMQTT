#ifndef __include_oracle_rc522_h
#define __include_oracle_rc522_h

#include <stdint.h>

#if 0
#define SDA_PIN       ALICE_RFID_CONFIG_SDA_PIN
#define RST_PIN       ALICE_RFID_CONFIG_RST_PIN
#define MISO_PIN      ALICE_RFID_CONFIG_MISO_PIN
#define MOSI_PIN      ALICE_RFID_CONFIG_MOSI_PIN
#define SCK_PIN       ALICE_RFID_CONFIG_SCK_PIN
#endif

#define SCK_PIN       18
#define MISO_PIN      19
#define MOSI_PIN      23
#define SDA_PIN       5
#define RST_PIN       22

#define ORACLE_RFID_RC522_CONFIG_DEFAULT() {\
    .enable_spi = 1,\
    .sck_pin = SCK_PIN,\
    .miso_pin = MISO_PIN,\
    .mosi_pin = MOSI_PIN,\
    .sda_pin = SDA_PIN,\
    .rst_pin = RST_PIN\
}

typedef struct oracle_rc522_config_s {
    uint8_t enable_spi;
    uint8_t sck_pin;
    uint8_t miso_pin;
    uint8_t mosi_pin;
    uint8_t sda_pin;
    uint8_t rst_pin;
    uint16_t pad;
} oracle_rc522_config_t;

typedef struct oracle_rc522_uuid_s {
    uint8_t uuid[8];
    uint8_t size;
} oracle_rc522_uuid_t;

extern oracle_rc522_uuid_t oracle_rc522_empty_uuid;


void oracle_rc522_init(void);
void oracle_rc522_init(oracle_rc522_config_t *config);

bool oracle_rc522_read(oracle_rc522_uuid_t *uuid);

void oracle_rc522_dump_config(oracle_rc522_config_t *config);
void oracle_rc522_dump_fwconfig(void);
void oracle_rc522_copy_uuid(oracle_rc522_uuid_t *uuid,
                                 char *buffer, uint8_t buffer_size);

#endif /* #ifndef __include_oracle_rc522_h */