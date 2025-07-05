#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <IRremote.hpp>
#include "SMLimage.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define IR_PIN 6
#define BUZZER_PIN 11
#define MODE_BUTTON_PIN A3

char grid[3][9] = {
  {'A','B','C','D','E','F','G','H','I'},
  {'J','K','L','M','N','O','P','Q','R'},
  {'S','T','U','V','W','X','Y','Z',' '}
};

int row = 0, col = 0;
String message = "";
bool phraseMode = false;

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
  pinMode(MODE_BUTTON_PIN, INPUT_PULLUP);
  IrReceiver.begin(IR_PIN);
  showSMLLogo();
}

void loop() {
  if (digitalRead(MODE_BUTTON_PIN) == LOW) {
    phraseMode = !phraseMode;
    delay(300);
  }

  if (IrReceiver.decode()) {
    uint32_t code = IrReceiver.decodedIRData.command;
    IrReceiver.resume();

    if (code == 0x18) row = (row + 1) % 3;           // "DOWN"
    else if (code == 0x52) col = (col + 1) % 9;      // "RIGHT"
    else if (code == 0x1C) message += grid[row][col]; // "OK"
    else if (code == 0x10) message = "";             // "0" = Clear
    tone(BUZZER_PIN, 1000, 100);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Mode: ");
  display.println(phraseMode ? "Phrase" : "Letter");

  display.setCursor(0, 12);
  display.print("Select: ");
  display.println(grid[row][col]);

  display.setCursor(0, 30);
  display.print("Msg: ");
  display.println(message);
  display.display();
}
