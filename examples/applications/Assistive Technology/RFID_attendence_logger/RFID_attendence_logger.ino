#include <SPI.h>
#include <MFRC522.h>
#include <EEPROM.h>

#define SS_PIN A2     // RFID SDA
#define RST_PIN A3    // RFID RST

MFRC522 mfrc522(SS_PIN, RST_PIN);

const int UID_SIZE = 4;
const int MAX_UIDS = 25; // You can store up to 25 unique UIDs (4 bytes each)
int storedUIDs = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial); // Wait for Serial Monitor to open

  SPI.begin();
  mfrc522.PCD_Init();
  delay(100);

  Serial.println("RFID Attendance Logger Ready");
  storedUIDs = EEPROM.read(0); // EEPROM[0] holds count
  if (storedUIDs > MAX_UIDS) storedUIDs = 0;

  Serial.print("Stored UID count: ");
  Serial.println(storedUIDs);
}

void loop() {
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) return;

  byte scannedUID[UID_SIZE];
  for (int i = 0; i < UID_SIZE; i++) {
    scannedUID[i] = mfrc522.uid.uidByte[i];
  }

  Serial.print("Scanned UID: ");
  printUID(scannedUID);

  if (isNewUID(scannedUID)) {
    if (storedUIDs < MAX_UIDS) {
      saveUID(scannedUID);
      Serial.println("-> New UID logged.");
    } else {
      Serial.println("-> Memory full. UID not saved.");
    }
  } else {
    Serial.println("-> UID already logged.");
  }

  // Always display current count
  Serial.print("Total Stored UIDs: ");
  Serial.println(storedUIDs);

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
  delay(1000);
}

void printUID(byte *uid) {
  for (int i = 0; i < UID_SIZE; i++) {
    Serial.print("0x");
    if (uid[i] < 0x10) Serial.print("0");
    Serial.print(uid[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
}

bool isNewUID(byte *uid) {
  for (int i = 0; i < storedUIDs; i++) {
    int baseAddr = 1 + (i * UID_SIZE);
    bool match = true;
    for (int j = 0; j < UID_SIZE; j++) {
      if (EEPROM.read(baseAddr + j) != uid[j]) {
        match = false;
        break;
      }
    }
    if (match) return false;
  }
  return true;
}

void saveUID(byte *uid) {
  int baseAddr = 1 + (storedUIDs * UID_SIZE);
  for (int i = 0; i < UID_SIZE; i++) {
    EEPROM.write(baseAddr + i, uid[i]);
  }
  storedUIDs++;
  EEPROM.write(0, storedUIDs); // Save updated count
}
