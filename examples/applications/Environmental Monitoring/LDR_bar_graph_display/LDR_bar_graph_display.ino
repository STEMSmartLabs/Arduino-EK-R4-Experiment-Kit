#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "SMLimage.h"

#define LDR_PIN A0
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

void setup() {
  showSMLLogo();
}

void loop() {
  int light = analogRead(LDR_PIN);
  int barLength = map(light, 0, 1023, 0, 100);

  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.println("Ambient Light:");

  display.drawRect(0, 20, 100, 10, WHITE);
  display.fillRect(0, 20, barLength, 10, WHITE);

  display.display();
  delay(200);
}
