#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "SMLimage.h"

#define TEMP_PIN A0
#define LDR_PIN A1
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
  int temp = analogRead(TEMP_PIN) / 10;
  int light = analogRead(LDR_PIN);

  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.print("Temp: ");
  display.print(temp);
  display.print("C");

  display.setCursor(0, 20);
  display.print("Light: ");
  display.print(light);

  display.setCursor(0, 40);
  if (temp > 35) display.println("Status: HOT!");
  else if (light < 300) display.println("Status: DARK");
  else display.println("Status: NORMAL");

  display.display();
  delay(1000);
}
