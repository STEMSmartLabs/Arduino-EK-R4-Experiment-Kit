#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "SMLimage.h"

#define SS_PIN 2
#define RST_PIN 4
#define SERVO_PIN 9
#define BUZZER_PIN 11

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

MFRC522 mfrc522(SS_PIN, RST_PIN);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Servo lockServo;

// Change this UID to your card's UID
byte authorizedUID[4] = {0xDE, 0xAD, 0xBE, 0xEF}; // Replace with your card/tag UID

void showSMLLogo() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.drawBitmap(0, 0, SMLimage, 128, 64, 1);
  display.display();
  delay(2000);
  display.clearDisplay();
}

void setup() {
  SPI.begin();
  mfrc522.PCD_Init();
  pinMode(BUZZER_PIN, OUTPUT);
  lockServo.attach(SERVO_PIN);

  showSMLLogo();
  lockServo.write(0); // Locked
}

void loop() {
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Scan RFID Card...");
  display.display();

  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) return;

  if (isAuthorized(mfrc522.uid.uidByte)) {
    unlockDoor();
    displayMessage("Access Granted", true);
  } else {
    displayMessage("Access Denied", false);
  }

  delay(3000);
  lockServo.write(0); // Lock again
}

bool isAuthorized(byte *uid) {
  for (byte i = 0; i < 4; i++) {
    if (uid[i] != authorizedUID[i]) return false;
  }
  return true;
}

void unlockDoor() {
  lockServo.write(90); // Unlock
  digitalWrite(BUZZER_PIN, HIGH);
  delay(100);
  digitalWrite(BUZZER_PIN, LOW);
}

void displayMessage(String message, bool success) {
  display.clearDisplay();
  display.setCursor(0, 20);
  display.println(message);
  display.display();

  if (!success) {
    for (int i = 0; i < 3; i++) {
      digitalWrite(BUZZER_PIN, HIGH);
      delay(150);
      digitalWrite(BUZZER_PIN, LOW);
      delay(150);
    }
  }
}
