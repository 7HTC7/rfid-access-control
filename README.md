# Arduino RFID & PIN Access Control

An Arduino-based access control system using an RC522 RFID reader or a 4×4 keypad PIN. 
A 16×2 I²C LCD displays status messages, while green and red LEDs indicate access granted or denied.

## Features
- RFID card/keychain authentication
- Keypad PIN authentication
- LCD status display and LED indicators

## Libraries
- `MFRC522`
- `Keypad`
- `LiquidCrystal_I2C`

## Scanners
- **I²C Scanner:** Detects I²C devices and identifies their addresses, useful for troubleshooting the LCD.
- **RFID Scanner:** Reads RFID tag UIDs for configuring authorized cards and keychains.

## Security Note
This is a demonstration project. RFID UID-based authentication and a hardcoded PIN are not suitable for high-security applications.
