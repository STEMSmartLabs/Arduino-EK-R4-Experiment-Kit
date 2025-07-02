#include <Servo.h>
#include <EEPROM.h>
#include <IRremote.hpp>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "SMLimage.h"

// OLED setup
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Pins
const int ldrPin = A0;
const int servoPin = D9;
const int irPin = D6;

// Servo setup
Servo myServo;
const int openAngle = 0;
const int partialAngle = 45;
const int closeAngle = 180;
int currentAngle = openAngle;

// Threshold config
int threshold = 300;
const int EEPROM_ADDR = 0;
const int MIN_THRESHOLD = 100;
const int MAX_THRESHOLD = 500;
const int THRESHOLD_STEP = 100;

// IR codes
const unsigned long IR_BUTTON_UP = 0xB946FF00;
const unsigned long IR_BUTTON_DOWN = 0xEA15FF00;

void setup() {
  Serial.begin(9600);
  myServo.attach(servoPin);
  myServo.write(openAngle);
  currentAngle = openAngle;

  // Load threshold from EEPROM
  threshold = EEPROM.read(EEPROM_ADDR);
  if (threshold < MIN_THRESHOLD || threshold > MAX_THRESHOLD) threshold = 300;

  IrReceiver.begin(irPin, ENABLE_LED_FEEDBACK);

  // OLED init
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("OLED init failed"));
    while (true);
  }

  // Show startup image
  display.clearDisplay();
  display.drawBitmap(0, 0, SMLimage, 128, 64, SSD1306_WHITE);
  display.display();
  delay(2000);
  display.clearDisplay();
  display.display();
}

void loop() {
  // Handle IR remote
  if (IrReceiver.decode()) {
    unsigned long code = IrReceiver.decodedIRData.decodedRawData;

    if (code == IR_BUTTON_UP) {
      threshold += THRESHOLD_STEP;
      if (threshold > MAX_THRESHOLD) threshold = MIN_THRESHOLD;
      EEPROM.write(EEPROM_ADDR, threshold);
    } else if (code == IR_BUTTON_DOWN) {
      threshold -= THRESHOLD_STEP;
      if (threshold < MIN_THRESHOLD) threshold = MAX_THRESHOLD;
      EEPROM.write(EEPROM_ADDR, threshold);
    }

    IrReceiver.resume();
  }

  int ldrValue = analogRead(ldrPin);
  int targetAngle = currentAngle;

  if (ldrValue < 100) {
    targetAngle = openAngle;
  } else if (ldrValue >= 100 && ldrValue <= 300) {
    targetAngle = partialAngle;
  } else if (ldrValue > 300 && ldrValue < threshold) {
    targetAngle = map(ldrValue, 301, threshold - 1, partialAngle, closeAngle);
  } else if (ldrValue >= threshold) {
    targetAngle = closeAngle;
  }

  if (targetAngle != currentAngle) {
    moveServoSmoothly(currentAngle, targetAngle);
    currentAngle = targetAngle;
  }

  updateOLED(ldrValue);
  delay(300);
}

void moveServoSmoothly(int fromAngle, int toAngle) {
  if (fromAngle < toAngle) {
    for (int i = fromAngle; i <= toAngle; i++) {
      myServo.write(i);
      delay(10);
    }
  } else {
    for (int i = fromAngle; i >= toAngle; i--) {
      myServo.write(i);
      delay(10);
    }
  }
}

void updateOLED(int ldrVal) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);
  display.println("Smart Curtain");

  display.setCursor(0, 14);
  display.print("LDR: ");
  display.println(ldrVal);

  display.setCursor(0, 26);
  display.print("Threshold: ");
  display.println(threshold);

  drawLevelBlocks(threshold);
  display.display();
}

void drawLevelBlocks(int thresholdVal) {
  int level = map(thresholdVal, MIN_THRESHOLD, MAX_THRESHOLD, 1, 5);
  int blockWidth = 24;
  int blockHeight = 6;
  int y = SCREEN_HEIGHT - blockHeight;

  for (int i = 0; i < level; i++) {
    display.fillRect(i * blockWidth, y, blockWidth - 2, blockHeight, SSD1306_WHITE);
  }

  display.drawRect(0, y, SCREEN_WIDTH, blockHeight, SSD1306_WHITE);
}
