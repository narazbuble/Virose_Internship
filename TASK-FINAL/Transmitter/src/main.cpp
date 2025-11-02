#include "main.h"

const int mac_index_ku = DHANISHARA_ZASCHYA_PUTRI_SYAMSUDIN;  

void setup() {
    Serial.begin(115200);
    mulai_esp_now(mac_index_ku);
}

void loop() {
    if (Serial.available()) {
        baca_serial(callback_data_serial);
    }
}