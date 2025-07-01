#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "SMLimage.h"

#define TRIG_PIN 4
#define ECHO_PIN 7
#define BUZZER_PIN 11
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void showSMLLogo() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.drawBitmap(0, 0, SMLimage, 128, 64, 1);
  display.display();
  delay(2000);
  display.clearDisplay();
}

long readDistanceCM() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  return pulseIn(ECHO_PIN, HIGH) * 0.034 / 2;
}

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  showSMLLogo();
}

void loop() {
  long distance = readDistanceCM();
  int level = map(distance, 0, 30, 100, 0); // 0 cm = 100% full, 30 cm = 0%

  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.print("Water Level: ");
  display.print(level);
  display.println("%");

  if (level > 95) {
    display.println("Overflow!");
    tone(BUZZER_PIN, 1000, 500);
  }

  display.display();
  delay(1000);
}
