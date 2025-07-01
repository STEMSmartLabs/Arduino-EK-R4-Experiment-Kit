#include <Servo.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "SMLimage.h"

#define SERVO_PIN 9
#define BUZZER_PIN 11
#define TEMP_PIN A0
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Servo sprinklerServo;

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
  sprinklerServo.attach(SERVO_PIN);
  showSMLLogo();
}

void loop() {
  int tempValue = analogRead(TEMP_PIN);
  float temp = (tempValue / 1023.0) * 100;

  if (temp > 40) {
    digitalWrite(BUZZER_PIN, HIGH);
    sprinklerServo.write(90);
    display.clearDisplay();
    display.setCursor(0, 20);
    display.println("FIRE DETECTED!");
  } else {
    digitalWrite(BUZZER_PIN, LOW);
    sprinklerServo.write(0);
    display.clearDisplay();
    display.setCursor(0, 20);
    display.println("Temperature Normal");
  }

  display.display();
  delay(1000);
}
