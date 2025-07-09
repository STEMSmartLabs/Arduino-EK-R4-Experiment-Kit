#include <SPI.h>
#include <MFRC522.h>
#include <EEPROM.h>

// RFID connections
#define SS_PIN A2    // SDA
#define RST_PIN A3   // RST

MFRC522 mfrc522(SS_PIN, RST_PIN);

// Array to hold authorized UID from EEPROM
byte authorizedUID[4];

void setup() {
  Serial.begin(9600);
  while (!Serial);  // Wait for Serial Monitor
  SPI.begin();
  mfrc522.PCD_Init();

  // Load saved UID from EEPROM
  for (int i = 0; i < 4; i++) {
    authorizedUID[i] = EEPROM.read(i);
  }

  Serial.println("RFID Access System Ready");
  Serial.print("Stored UID: ");
  for (int i = 0; i < 4; i++) {
    Serial.print("0x");
    if (authorizedUID[i] < 0x10) Serial.print("0");
    Serial.print(authorizedUID[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
}

void loop() {
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) return;

  Serial.print("Scanned UID: ");
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print("0x");
    if (mfrc522.uid.uidByte[i] < 0x10) Serial.print("0");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    Serial.print(" ");
  }

  if (isAuthorized(mfrc522.uid.uidByte)) {
    Serial.println("\nAccess Granted");
  } else {
    Serial.println("\nAccess Denied");
  }

  // Halt card
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();

  delay(2000);
}

bool isAuthorized(byte *uid) {
  for (byte i = 0; i < 4; i++) {
    if (uid[i] != authorizedUID[i]) return false;
  }
  return true;
}
