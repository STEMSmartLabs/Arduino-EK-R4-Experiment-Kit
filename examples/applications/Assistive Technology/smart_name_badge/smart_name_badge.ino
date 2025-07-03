#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <IRremote.hpp>
#include "SMLimage.h"

#define IR_PIN 6
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
String messages[] = {"STEM Smart Labs", "Hello! I'm Vishal", "Arduino UNO R4"};
int index = 0;

void showSMLLogo() {
  display.clearDisplay();
  display.drawBitmap(0, 0, SMLimage, 128, 64, 1);
  display.display();
  delay(2000);
  display.clearDisplay();
}

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // Moved here
  IrReceiver.begin(IR_PIN);
  showSMLLogo();
}

void loop() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);  // Ensure text is visible
  display.setCursor(0, 20);
  display.println(messages[index]);
  display.display();
  delay(200);

  if (IrReceiver.decode()) {
    IrReceiver.resume();
    index = (index + 1) % 3;
  }
}
