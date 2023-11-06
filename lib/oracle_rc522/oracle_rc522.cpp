#include <Arduino.h>
#include <SPI.h>            //https://www.arduino.cc/en/reference/SPI
#include <MFRC522.h>        //https://github.com/miguelbalboa/rfid
#include <ArduinoJson.h>
#include <WebSerialLite.h>

#include <oracle_rc522.h>

static MFRC522 oracle_rc522;

oracle_rc522_uuid_t oracle_rc522_empty_uuid = {
    .uuid = {0, 0, 0, 0}, .size = 4
};

/// @brief 
/// @param uuid 
/// @return 
bool oracle_rc522_read(oracle_rc522_uuid_t *uuid)
{
    // Look for new 1 cards
    if (!oracle_rc522.PICC_IsNewCardPresent()) {
        return false;
    }

    // Verify if the NUID has been readed
    if (!oracle_rc522.PICC_ReadCardSerial()) {
        Serial.println("RFID Card Read Error");
        WebSerial.println("RFID Card Read Error");
        return false;
    }

    if (oracle_rc522.uid.size != 4) {
        Serial.println("RFID Card Size Error");
        WebSerial.println("RFID Card Size Error");
        return false;
    }

    for (int i = 0; i < oracle_rc522.uid.size; i++) {
        uuid->uuid[i] = oracle_rc522.uid.uidByte[i];
    }
    uuid->size = oracle_rc522.uid.size;

    // Halt PICC
    oracle_rc522.PICC_HaltA();

    // Stop encryption on PCD
    oracle_rc522.PCD_StopCrypto1();

    return true;
}

void oracle_rc522_dump_config(oracle_rc522_config_t *config)
{
    char buffer[256];

    /* Setup RFID Reader */
    snprintf(buffer, sizeof(buffer), "RFID MC522 SS PIN#%d", config->sda_pin);
    Serial.println(buffer);
    WebSerial.println(buffer);

    snprintf(buffer, sizeof(buffer), "RFID MC522 RST PIN#%d", config->rst_pin);
    Serial.println(buffer);
    WebSerial.println(buffer);

    snprintf(buffer, sizeof(buffer), "RFID MC522 SCK PIN#%d", config->sck_pin);
    Serial.println(buffer);
    WebSerial.println(buffer);

    snprintf(buffer, sizeof(buffer), "RFID MC522 MOSI PIN#%d", config->mosi_pin);
    Serial.println(buffer);
    WebSerial.println(buffer);

    snprintf(buffer, sizeof(buffer), "RFID MC522 MISO PIN#%d", config->miso_pin);
    Serial.println(buffer);
    WebSerial.println(buffer);
}

void oracle_rc522_copy_uuid(oracle_rc522_uuid_t *uuid,
                                 char *buffer, uint8_t buffer_size)
{
    snprintf(buffer, buffer_size, "%02X:%02X:%02X:%02X", uuid->uuid[0],
             uuid->uuid[1], uuid->uuid[2], uuid->uuid[3]);
}

void oracle_rc522_dumpinfo(void)
{
    oracle_rc522.PCD_DumpVersionToSerial();
}

void oracle_rc522_setup(oracle_rc522_config_t *config)
{
    Serial.println("Setup RFID BEGIN");
    WebSerial.println("Setup RFID BEGIN");

    SPI.begin(config->sck_pin, config->miso_pin, config->mosi_pin);
    oracle_rc522 = MFRC522(config->sda_pin, config->rst_pin);
    oracle_rc522.PCD_Init();

    Serial.println("Setup RFID DONE");
    WebSerial.println("Setup RFID DONE");
}

void oracle_rc522_setup(void)
{
  oracle_rc522_config_t config = ORACLE_RFID_RC522_CONFIG_DEFAULT();
  oracle_rc522_setup(&config);
}