#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "SMLimage.h"

#define BUTTON_PIN 12
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
String messages[] = {"Meeting at 4PM", "Lunch Break: 1-2PM", "Project Deadline Today"};
int index = 0;

void showSMLLogo() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.drawBitmap(0, 0, SMLimage, 128, 64, 1);
  display.display();
  delay(2000);
  display.clearDisplay();
}

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  showSMLLogo();
}

void loop() {
  display.clearDisplay();
  display.setCursor(0, 20);
  display.setTextSize(1);
  display.println(messages[index]);
  display.display();

  if (digitalRead(BUTTON_PIN) == LOW) {
    index = (index + 1) % 3;
    delay(300);
  }
}
