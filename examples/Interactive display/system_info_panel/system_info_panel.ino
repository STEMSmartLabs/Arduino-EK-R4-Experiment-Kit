#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "SMLimage.h"

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
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("STEM Device v1.0");
  display.println("Status: ONLINE");
  display.println("Temp: 28.5 C");
  display.display();
  delay(2000);
}
