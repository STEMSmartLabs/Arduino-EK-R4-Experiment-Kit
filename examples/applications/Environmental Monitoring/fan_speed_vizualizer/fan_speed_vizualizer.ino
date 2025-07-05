#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "SMLimage.h"

#define POT_PIN A1
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
  int speed = analogRead(POT_PIN) / 4;

  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.println("Fan Speed:");

  display.drawRect(0, 20, 100, 10, WHITE);
  display.fillRect(0, 20, map(speed, 0, 255, 0, 100), 10, WHITE);
  display.setCursor(0, 40);
  display.print("Speed: ");
  display.print(map(speed, 0, 255, 0, 100));
  display.println("%");
  display.display();

  delay(200);
}
fa