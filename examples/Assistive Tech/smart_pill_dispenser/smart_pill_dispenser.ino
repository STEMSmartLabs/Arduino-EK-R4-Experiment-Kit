#include <Servo.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_NeoPixel.h>
#include "SMLimage.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define SERVO_PIN 9
#define BUZZER_PIN 11
#define BUTTON_PIN 12
#define NEOPIXEL_PIN 5
#define ACCEL_PIN A2  // Simulated analog tremor detection
#define POT_PIN A1

Servo pillServo;
Adafruit_NeoPixel ring(1, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

unsigned long lastDoseTime = 0;
const unsigned long doseInterval = 30000; // 30 sec for testing
bool doseDue = false;

void showSMLLogo() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.drawBitmap(0, 0, SMLimage, 128, 64, 1);
  display.display();
  delay(2000);
  display.clearDisplay();
}

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pillServo.attach(SERVO_PIN);
  ring.begin();
  ring.show();

  showSMLLogo();
  lastDoseTime = millis();
}

void loop() {
  unsigned long currentTime = millis();

  // Check if it's time for dose
  if (currentTime - lastDoseTime >= doseInterval && !doseDue) {
    doseDue = true;
    pillServo.write(90); // Rotate to next compartment
    buzzReminder();
    displayDoseReminder();
  }

  // Check for button press to confirm dose
  if (doseDue && digitalRead(BUTTON_PIN) == LOW) {
    doseDue = false;
    lastDoseTime = currentTime;
    pillServo.write(0); // Reset servo
    displayConfirmation();
    delay(1000);
  }

  updateNeoPixel(currentTime - lastDoseTime);
}

void buzzReminder() {
  int volume = analogRead(POT_PIN) / 4; // 0–255
  analogWrite(BUZZER_PIN, volume);
  delay(500);
  analogWrite(BUZZER_PIN, 0);
}

void displayDoseReminder() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.println("Time for Pill:");
  display.setCursor(0, 30);
  display.println("Vitamin D");
  display.display();
}

void displayConfirmation() {
  display.clearDisplay();
  display.setCursor(0, 20);
  display.println("Dose Taken!");
  display.display();
}

void updateNeoPixel(unsigned long sinceLastDose) {
  int brightness = map(sinceLastDose, 0, doseInterval * 2, 255, 10);
  int red = map(sinceLastDose, 0, doseInterval * 2, 0, 255);
  int green = 255 - red;

  ring.setPixelColor(0, ring.Color(red, green, 0));
  ring.setBrightness(brightness);
  ring.show();
}
