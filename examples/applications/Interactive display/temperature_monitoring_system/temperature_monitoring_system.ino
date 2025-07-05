#include <Adafruit_NeoPixel.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "SMLimage.h"

#define DHT_PIN A0
#define NEOPIXEL_PIN 5
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_NeoPixel ring(1, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

void showSMLLogo() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.drawBitmap(0, 0, SMLimage, 128, 64, 1);
  display.display();
  delay(2000);
  display.clearDisplay();
}

void setup() {
  ring.begin();
  ring.show();
  showSMLLogo();
}

void loop() {
  int val = analogRead(DHT_PIN);
  float temp = (val / 1023.0) * 100;

  int red = map(temp, 20, 40, 0, 255);
  int blue = 255 - red;
  ring.setPixelColor(0, ring.Color(red, 0, blue));
  ring.show();

  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.print("Temp: ");
  display.print(temp);
  display.println(" C");
  display.display();

  delay(1000);
}
