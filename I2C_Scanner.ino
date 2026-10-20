#include <Wire.h>

void setup() {
    Wire.begin();
    Serial.begin(9600);
    Serial.println("I2C scan...");
}

void loop() {
    byte count = 0;
    for (byte addr = 1; addr < 127; addr++) {
        Wire.beginTransmission(addr);
        if (Wire.endTransmission() == 0) {
            Serial.print("Found: 0x");
            if (addr < 16) Serial.print("0");
            Serial.println(addr, HEX);
            count++;
            delay(5);
        }
    }
    Serial.print("Done. Devices: ");
    Serial.println(count);
    delay(5000);
}
