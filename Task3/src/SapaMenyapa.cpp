#include "main.h"
#include <Arduino.h>
#include <EEPROM.h>

const int mac_index_ku = 9;

void setup(){
    Serial.begin(115200);
    EEPROM.begin(128);

    Serial.println("SAPA MENYAPA ><");
    Serial.printf("Halo, aku %s\n", mac_names[mac_index_ku]);

    if (mulai_esp_now(mac_index_ku) == ESP_OK) {
        Serial.println("inisiasi berhasi :D ");
    }
    else{
        Serial.println("inisiasi gagal :(");
    }
}

void loop() {
    if (Serial.available()) {
        baca_serial(callback_data_serial);
    }
}

//co in : FF FF 00 02 00 01
