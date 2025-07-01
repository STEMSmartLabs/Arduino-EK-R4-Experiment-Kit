#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "SMLimage.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  for (int i = 0; i <= 10; i++) {
    display.clearDisplay();
    display.drawBitmap(i, 0, SMLimage, 128, 64, 1);
    display.display();
    delay(100);
  }
  delay(1000);
}

void loop() {}
