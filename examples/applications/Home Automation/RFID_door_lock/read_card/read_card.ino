#include <SPI.h>
#include <MFRC522.h>
#include <EEPROM.h>

// New pin connections
#define SS_PIN A2    // RFID SDA connected to A2
#define RST_PIN A3   // RFID RST connected to A3

MFRC522 mfrc522(SS_PIN, RST_PIN);
bool uidSaved = false;

void setup() {
  Serial.begin(9600);
  while (!Serial);  // Wait for Serial Monitor to connect
  SPI.begin();
  delay(100);
  mfrc522.PCD_Init();

  Serial.println("Scan a card to save UID...");
}

void loop() {
  Serial.println("\nWaiting for card...");

  while (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    delay(100);
  }

  Serial.print("UID: ");
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    byte val = mfrc522.uid.uidByte[i];
    Serial.print("0x");
    if (val < 0x10) Serial.print("0");
    Serial.print(val, HEX);
    Serial.print(" ");

    if (!uidSaved) EEPROM.write(i, val);  // Save only first UID
  }

  if (!uidSaved) {
    uidSaved = true;
    Serial.println("\nUID saved to EEPROM.");
  } else {
    Serial.println("\nCard scanned.");
  }

  // Halt and reset
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();

  delay(2000);
}
