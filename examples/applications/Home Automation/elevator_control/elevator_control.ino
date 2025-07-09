#include <IRremote.hpp>
#include <Servo.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_NeoPixel.h>
#include "SMLimage.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Pin definitions
#define IR_PIN 6
#define SERVO_PIN 9
#define NEOPIXEL_PIN 5
#define BUZZER_PIN 11
#define FAN_PIN 10
#define LDR_PIN A0
#define LIGHT_PIN 3

Servo doorServo;
Adafruit_NeoPixel pixels(1, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

int currentFloor = 0;
bool doorOpen = false;

// SML Logo Display
void showSMLLogo() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.drawBitmap(0, 0, SMLimage, 128, 64, 1);
  display.display();
  delay(2000);
  display.clearDisplay();
}

void setup() {
  IrReceiver.begin(IR_PIN, ENABLE_LED_FEEDBACK);
  doorServo.attach(SERVO_PIN);
  pixels.begin();
  pixels.clear();

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(FAN_PIN, OUTPUT);
  pinMode(LIGHT_PIN, OUTPUT);

  showSMLLogo();
}

void loop() {
  // Auto Fan & Light Control via LDR
  int ldrValue = analogRead(LDR_PIN);
  digitalWrite(FAN_PIN, ldrValue > 600 ? HIGH : LOW);
  digitalWrite(LIGHT_PIN, ldrValue < 400 ? HIGH : LOW);

  if (IrReceiver.decode()) {
    unsigned long code = IrReceiver.decodedIRData.command;
    IrReceiver.resume();

    switch (code) {
      case 0x45: setFloor(1); break; // Button "1"
      case 0x46: setFloor(2); break; // Button "2"
      case 0x47: setFloor(3); break; // Button "3"
      case 0x44: openDoor(); break;  // Button "OK"
      case 0x40: closeDoor(); break; // Button "LEFT"
      case 0x43: triggerAlarm(); break; // Button "HASH"
    }
  }

  updateDisplay();
}

void setFloor(int floor) {
  currentFloor = floor;
  pixels.setPixelColor(0, pixels.Color(0, 0, 255)); // Blue
  pixels.show();
}

void openDoor() {
  doorServo.write(90); // Open
  doorOpen = true;
}

void closeDoor() {
  doorServo.write(0); // Closed
  doorOpen = false;
}

void triggerAlarm() {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(500);
  digitalWrite(BUZZER_PIN, LOW);
}

void updateDisplay() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(0, 0);
  display.print("Floor: ");
  display.println(currentFloor);

  display.setCursor(0, 15);
  display.print("Door: ");
  display.println(doorOpen ? "OPEN" : "CLOSED");

  display.setCursor(0, 30);
  display.print("LDR: ");
  display.println(analogRead(LDR_PIN));

  display.display();
}
