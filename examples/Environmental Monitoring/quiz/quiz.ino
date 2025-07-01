#include <IRremote.hpp>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_NeoPixel.h>
#include "SMLimage.h"

#define IR_PIN 6
#define BUZZER_PIN 11
#define NEOPIXEL_PIN 5
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_NeoPixel pixel(1, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

int correctAnswer = 2; // IR button code (example)
bool answered = false;

void showSMLLogo() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.drawBitmap(0, 0, SMLimage, 128, 64, 1);
  display.display();
  delay(2000);
  display.clearDisplay();
}

void setup() {
  pixel.begin();
  pinMode(BUZZER_PIN, OUTPUT);
  IrReceiver.begin(IR_PIN);
  showSMLLogo();
}

void loop() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.println("Q: 2 + 2 = ?");
  display.println("1) 3   2) 4   3) 5");
  display.display();

  if (IrReceiver.decode()) {
    int code = IrReceiver.decodedIRData.command;
    IrReceiver.resume();

    if (code == 0x1C) { // Assume 'OK' is correct for 2
      correctFeedback();
    } else {
      wrongFeedback();
    }
  }
}

void correctFeedback() {
  pixel.setPixelColor(0, pixel.Color(0, 255, 0));
  pixel.show();
  tone(BUZZER_PIN, 1000, 200);
}

void wrongFeedback() {
  pixel.setPixelColor(0, pixel.Color(255, 0, 0));
  pixel.show();
  tone(BUZZER_PIN, 200, 500);
}
