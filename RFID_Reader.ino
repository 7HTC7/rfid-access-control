#include <SPI.h>
#include <MFRC522.h>

#define RFID_RST 9
#define RFID_SS 8

MFRC522 rfid(RFID_SS, RFID_RST);

String uidToString(MFRC522::Uid *uid) {
    String s="";
    for (byte i=0; i<uid->size; i++) {
      if (uid->uidByte[i]<0x10) s+="0";
      s += String(uid->uidByte[i], HEX);
      if (i!=uid->size-1) s+=" ";
    }
    s.toUpperCase();
    return s;
}

void setup() {
    Serial.begin(9600);
    SPI.begin();
    rfid.PCD_Init();
    Serial.println("RC522 ready. Scan a tag...");
}

void loop() {
    if (!rfid.PICC_IsNewCardPresent()) return; 
    if (!rfid.PICC_ReadCardSerial()) return;

    Serial.print("UID: ");
    Serial.println(uidToString(&rfid.uid));

    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
}
