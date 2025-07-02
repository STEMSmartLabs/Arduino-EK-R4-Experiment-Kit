#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 2
#define RST_PIN 4
MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("Scan a card...");
}

void loop() {
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) return;

  Serial.print("UID: ");
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print("0x"); Serial.print(mfrc522.uid.uidByte[i], HEX); Serial.print(" ");
  }
  Serial.println();
  delay(1000);
}
