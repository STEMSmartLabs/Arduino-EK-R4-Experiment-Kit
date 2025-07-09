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
#define POT_PIN A1

Servo pillServo;
Adafruit_NeoPixel ring(1, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

unsigned long lastDoseTime = 0;
const unsigned long doseInterval = 30000; // 30 seconds for testing
bool doseDue = false;
bool blinking = false;

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
  ring.begin();
  ring.show();
  showSMLLogo();
  lastDoseTime = millis();
}

void loop() {
  unsigned long currentTime = millis();

  // Time to dispense pill
  if (currentTime - lastDoseTime >= doseInterval && !doseDue) {
    doseDue = true;
    blinking = false;
    rotateServoOpen();          // Servo 90 → 0 (dispense)
    buzzReminder();
    displayDoseReminder();
  }

  // User confirms pill taken
  if (doseDue && digitalRead(BUTTON_PIN) == LOW) {
    doseDue = false;
    blinking = false;
    rotateServoClose();         // Servo back to 90 (close)
    lastDoseTime = currentTime;
    displayConfirmation();
    delay(1000);
  }

  // If user has not pressed button within 10 seconds after due
  if (doseDue && (currentTime - lastDoseTime >= doseInterval + 10000)) {
    blinking = true;
  }

  updateNeoPixel(currentTime - lastDoseTime);
}

// Rotate servo from 90 → 0 (simulate open)
void rotateServoOpen() {
  pillServo.attach(SERVO_PIN);
  pillServo.write(90);
  delay(500);
  pillServo.write(0);
  delay(500);
  pillServo.detach();
}

// Rotate servo back to 90 (simulate close)
void rotateServoClose() {
  pillServo.attach(SERVO_PIN);
  pillServo.write(90);
  delay(500);
  pillServo.detach();
}

// Buzzer buzzes with volume based on potentiometer
void buzzReminder() {
  int volume = analogRead(POT_PIN) / 4; // Map 0–1023 to 0–255
  analogWrite(BUZZER_PIN, volume);
  delay(500);
  analogWrite(BUZZER_PIN, 0);
}

// Show reminder on OLED
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

// Show confirmation on OLED
void displayConfirmation() {
  display.clearDisplay();
  display.setCursor(0, 20);
  display.println("Dose Taken!");
  display.display();
}

// Update NeoPixel based on time
void updateNeoPixel(unsigned long sinceLastDose) {
  if (doseDue && blinking) {
    static unsigned long lastBlink = 0;
    static bool ledOn = false;

    if (millis() - lastBlink > 500) {
      ledOn = !ledOn;
      lastBlink = millis();
    }

    if (ledOn) {
      ring.setPixelColor(0, ring.Color(255, 0, 0)); // Red
    } else {
      ring.setPixelColor(0, 0); // Off
    }

    ring.setBrightness(255);
    ring.show();
    return;
  }

  // Fade from yellow to red
  int red = 255;
  int green = map(sinceLastDose, 0, doseInterval, 255, 0);
  green = constrain(green, 0, 255);

  ring.setPixelColor(0, ring.Color(red, green, 0));
  ring.setBrightness(200);
  ring.show();
}
