#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <SPI.h>
#include <Keypad.h>
#include <MFRC522.h>

//RC522
#define RFID_RST 9
#define RFID_SS 8

//2 leds
#define GREEN_LED A3
#define RED_LED A2

//button matrix 4x4
#define R4 7
#define R3 6
#define R2 5
#define R1 4
#define C1 3
#define C2 2
#define C3 A0
#define C4 A1

LiquidCrystal_I2C lcd(0x27, 16, 2);

MFRC522 rfid(RFID_SS, RFID_RST);

const byte ROWS=4;
const byte COLS=4;

char keys[ROWS][COLS]={
    {'1','2','3','A'},
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','#','D'}
};

byte rowPins[ROWS]={R1, R2, R3, R4};
byte colPins[COLS]={C1, C2, C3, C4};

Keypad keypad=Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

const String UID_CARD="82 3A CF 5C";
const String UID_KEYCHAIN="02 68 A6 05";
const String PIN_CODE="293A0";

enum Mode { IDLE_MODE, ENTER_PIN };
Mode mode=IDLE_MODE;
String pinBuffer="";

void ledsOff() {
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, LOW);
}

void show2(const char* line1, const char* line2="") {
    lcd.clear();
    lcd.setCursor(0,0); lcd.print(line1);
    lcd.setCursor(0,1); lcd.print(line2);
}

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

bool isAllowedUID(const String& uid) {
  return (uid==UID_CARD) || (uid==UID_KEYCHAIN);
}

void grantAccess() {
    ledsOff();
    digitalWrite(GREEN_LED, HIGH);
    show2("ACCESS GRANTED", "Welcome");
    delay(1500);
    ledsOff();
    pinBuffer="";
    mode=IDLE_MODE;
    show2("Scan RFID", "or enter PIN");
}

void denyAccess() {
    ledsOff();
    digitalWrite(RED_LED, HIGH);
    show2("ACCESS DENIED", "Try again");
    delay(1500);
    ledsOff();
    pinBuffer="";
    mode=IDLE_MODE;
    show2("Scan RFID", "or enter PIN");
}

void handleRFID() {
    if (!rfid.PICC_IsNewCardPresent()) return;
    if (!rfid.PICC_ReadCardSerial()) return;

    String uid=uidToString(&rfid.uid);
    Serial.print("UID: "); Serial.println(uid);

    if (isAllowedUID(uid)) {
        grantAccess();
    } else {
        mode=ENTER_PIN;
        pinBuffer="";
        show2("Unknown tag", "Enter PIN:");
    }

    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
}

void handleKeypad() {
    char k=keypad.getKey();
    if (!k) return;

    Serial.print("Key: "); Serial.println(k);

    //allow * to clear
    if (k=='*') {
        pinBuffer="";
        mode=ENTER_PIN;
        show2("Enter PIN:", "");
        return;
    }

    //if not in pin mode, ignore digits until user starts entering
    if (mode!=ENTER_PIN) {
        mode=ENTER_PIN;
        pinBuffer="";
        show2("Enter PIN:", "");
    }

    if (k=='#') {
        if (pinBuffer==PIN_CODE) grantAccess();
        else denyAccess();
        return;
    }

    if ((k>='0' && k<='9') || (k>='A' && k<='D')) {
        if (pinBuffer.length()<8) {
          pinBuffer+=k;
          //show masked stars on 2nd line
          lcd.setCursor(0,1);
          for (unsigned int i=0; i<pinBuffer.length(); i++) lcd.print('*');
        }
    }
}

void setup() {
    Serial.begin(9600);

    pinMode(GREEN_LED, OUTPUT);
    pinMode(RED_LED, OUTPUT);
    ledsOff();

    lcd.init();
    lcd.backlight();
    show2("Starting...", "");

    SPI.begin();
    rfid.PCD_Init();

    delay(300);
    show2("Scan RFID", "or enter PIN");
}

void loop() {
    handleRFID();
    handleKeypad();
}

