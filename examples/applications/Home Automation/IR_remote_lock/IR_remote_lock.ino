#include <Wire.h>
#include <IRremote.hpp>
#include <Servo.h>
#include <EEPROM.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_NeoPixel.h>
#include "SMLimage.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// NeoPixel
#define NEOPIXEL_PIN 5
#define NUM_PIXELS 16
Adafruit_NeoPixel pixels(NUM_PIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

// Pins
const int RECV_PIN = 6;
const int servoPin = 9;
const int buzzerPin = 11;

Servo myServo;

// State variables
String enteredPassword = "";
bool enteringPassword = false;
bool waitingToLock = false;
int attempts = 0;
const int maxAttempts = 3;
bool lockedOut = false;
unsigned long lockoutStartTime = 0;
const int lockoutDuration = 10000;

#define EEPROM_ADDR_ATTEMPTS 10
#define EEPROM_ADDR_PASSWORD 0
const byte ENCRYPTION_KEY = 0x5A;

const int lockedAngle = 0;
const int unlockedAngle = 90;

// IR Codes
const unsigned long OK_BUTTON = 0xBF40FF00;
const unsigned long NUM_0     = 0xAD52FF00;
const unsigned long NUM_1     = 0xE916FF00;
const unsigned long NUM_2     = 0xE619FF00;
const unsigned long NUM_3     = 0xF20DFF00;
const unsigned long NUM_4     = 0xF30CFF00;
const unsigned long NUM_5     = 0xE718FF00;
const unsigned long NUM_6     = 0xA15EFF00;
const unsigned long NUM_7     = 0xF708FF00;
const unsigned long NUM_8     = 0xE31CFF00;
const unsigned long NUM_9     = 0xA55AFF00;

void setup() {
  Wire.begin();  // ✅ Required for OLED

  pinMode(buzzerPin, OUTPUT);
  IrReceiver.begin(RECV_PIN, ENABLE_LED_FEEDBACK);
  myServo.attach(servoPin);
  myServo.write(lockedAngle);

  pixels.begin();
  pixels.clear();
  pixels.show();

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) while (true);
  display.setTextColor(SSD1306_WHITE);
  display.setTextWrap(true);

  display.clearDisplay();
  display.drawBitmap(0, 0, SMLimage, 128, 64, WHITE);
  display.display();
  delay(2000);

  Serial.begin(9600);
  attempts = EEPROM.read(EEPROM_ADDR_ATTEMPTS);
  if (attempts >= maxAttempts) {
    lockedOut = true;
    lockoutStartTime = millis();
  }

  displayStatus("LOCKED");
}

void loop() {
  if (lockedOut) {
    unsigned long elapsed = millis() - lockoutStartTime;
    while (elapsed < lockoutDuration) {
      int secondsLeft = (lockoutDuration - elapsed) / 1000 + 1;

      display.clearDisplay();
      display.setTextColor(SSD1306_WHITE);
      display.setTextSize(2);
      display.setCursor(7, 10);
      display.println("LOCKED OUT");
      display.setTextSize(3);
      display.setCursor(45, 35);
      display.print(secondsLeft);
      display.display();

      delay(250);
      elapsed = millis() - lockoutStartTime;
    }

    lockedOut = false;
    attempts = 0;
    EEPROM.update(EEPROM_ADDR_ATTEMPTS, attempts);
    displayStatus("LOCKED");
    delay(1000);
    return;
  }

  if (IrReceiver.decode()) {
    unsigned long code = IrReceiver.decodedIRData.decodedRawData;

    if (waitingToLock && code == OK_BUTTON) {
      myServo.write(lockedAngle);
      pixels.clear();
      pixels.show();
      displayStatus("LOCKED");
      waitingToLock = false;
      IrReceiver.resume();
      delay(200);
      return;
    }

    if (code == OK_BUTTON && !enteringPassword && !waitingToLock) {
      enteringPassword = true;
      enteredPassword = "";
      displayPasswordScreen();
    } else if (enteringPassword) {
      char digit = mapIRToDigit(code);
      if (digit != '-') {
        enteredPassword += digit;
        displayPasswordScreen();

        if (enteredPassword.length() == 4) {
          if (enteredPassword == readDecryptedPassword()) {
            myServo.write(unlockedAngle);
            displayStatus("UNLOCKED");
            showColor(0, 255, 0);
            waitingToLock = true;
            attempts = 0;
            EEPROM.update(EEPROM_ADDR_ATTEMPTS, attempts);
          } else {
            attempts++;
            EEPROM.update(EEPROM_ADDR_ATTEMPTS, attempts);
            displayStatus("WRONG PASS");
            playFNote();
            showColor(255, 0, 0);
            delay(3000);
            pixels.clear();
            pixels.show();

            if (attempts >= maxAttempts) {
              lockedOut = true;
              lockoutStartTime = millis();
            } else {
              displayStatus("LOCKED");
            }
          }
          enteringPassword = false;
        }
      }
    }

    IrReceiver.resume();
  }

  delay(100);
}

// -------- Helper Functions --------

String readDecryptedPassword() {
  String decrypted = "";
  for (int i = 0; i < 4; i++) {
    char encryptedChar = EEPROM.read(EEPROM_ADDR_PASSWORD + i);
    decrypted += char(encryptedChar ^ ENCRYPTION_KEY);
  }
  return decrypted;
}

void displayStatus(String message) {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(2);
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(message, 0, 0, &x1, &y1, &w, &h);
  display.setCursor((SCREEN_WIDTH - w) / 2, (SCREEN_HEIGHT - h) / 2);
  display.println(message);
  display.display();
}

void displayPasswordScreen() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Enter Password:");
  display.setTextSize(2);
  display.setCursor(0, 30);
  for (int i = 0; i < 4; i++) {
    if (i < enteredPassword.length()) {
      display.print("*");
    } else if (i == enteredPassword.length()) {
      display.print("_");
    } else {
      display.print(" ");
    }
  }
  display.display();
}

char mapIRToDigit(unsigned long code) {
  if (code == NUM_0) return '0';
  if (code == NUM_1) return '1';
  if (code == NUM_2) return '2';
  if (code == NUM_3) return '3';
  if (code == NUM_4) return '4';
  if (code == NUM_5) return '5';
  if (code == NUM_6) return '6';
  if (code == NUM_7) return '7';
  if (code == NUM_8) return '8';
  if (code == NUM_9) return '9';
  return '-';
}

void playFNote() {
  tone(buzzerPin, 349);
  delay(400);
  noTone(buzzerPin);
}

void showColor(uint8_t r, uint8_t g, uint8_t b) {
  for (int i = 0; i < NUM_PIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(r, g, b));
  }
  pixels.show();
}
