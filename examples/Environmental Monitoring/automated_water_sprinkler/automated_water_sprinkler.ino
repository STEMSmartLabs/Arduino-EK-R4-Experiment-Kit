#include <Servo.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "SMLimage.h"

#define SERVO_PIN 9
#define BUZZER_PIN 11
#define LDR_PIN A0
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Servo sprinkler;

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
  sprinkler.attach(SERVO_PIN);
  showSMLLogo();
}

void loop() {
  int lightVal = analogRead(LDR_PIN);

  if (lightVal > 500) {
    sprinkler.write(90);
    digitalWrite(BUZZER_PIN, LOW);
    display.clearDisplay();
    display.setCursor(0, 20);
    display.println("Sprinkler: ON");
  } else {
    sprinkler.write(0);
    digitalWrite(BUZZER_PIN, HIGH);
    display.clearDisplay();
    display.setCursor(0, 20);
    display.println("Night Mode");
  }

  display.display();
  delay(1000);
}
