#include <IRremote.hpp>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "SMLimage.h"

#define IR_PIN 6
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int score = 0;
int fouls = 0;

void showSMLLogo() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.drawBitmap(0, 0, SMLimage, 128, 64, 1);
  display.display();
  delay(2000);
  display.clearDisplay();
}

void updateDisplay() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(2);
  display.print("Score: ");
  display.println(score);
  display.setTextSize(1);
  display.print("Fouls: ");
  display.println(fouls);
  display.display();
}

void setup() {
  IrReceiver.begin(IR_PIN);
  showSMLLogo();
  updateDisplay();
}

void loop() {
  if (IrReceiver.decode()) {
    uint8_t code = IrReceiver.decodedIRData.command;

    if (code == 0x1C) score++;        // OK
    else if (code == 0x18) score--;   // UP
    else if (code == 0x52) fouls++;   // DOWN
    else if (code == 0x08) fouls--;   // LEFT
    else if (code == 0x5A) { score = 0; fouls = 0; } // RIGHT = reset

    updateDisplay();
    IrReceiver.resume();
  }
}
