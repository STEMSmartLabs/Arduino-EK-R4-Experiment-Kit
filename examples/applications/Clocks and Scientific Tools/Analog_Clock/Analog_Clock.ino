#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "SMLimage.h"
#include <Wire.h>
#include <math.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int centerX = SCREEN_WIDTH / 2;
const int centerY = SCREEN_HEIGHT / 2;

unsigned long previousMillis = 0;
int hours = 0;     // Start time manually (e.g., 9:10:00 AM)
int minutes = 0;
int seconds = 0;

void showSMLLogo() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.drawBitmap(0, 0, SMLimage, 128, 64, 1);
  display.display();
  delay(2000);
  display.clearDisplay();
}

void drawHand(float angle, int length, int color) {
  int x = centerX + length * cos(angle);
  int y = centerY + length * sin(angle);
  display.drawLine(centerX, centerY, x, y, color);
}

void updateTime() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= 1000) {
    previousMillis = currentMillis;
    seconds++;
    if (seconds >= 60) {
      seconds = 0;
      minutes++;
      if (minutes >= 60) {
        minutes = 0;
        hours++;
        if (hours >= 24) {
          hours = 0;
        }
      }
    }
  }
}

void setup() {
  showSMLLogo();
}

void loop() {
  updateTime();

  float secAngle = seconds * 6 * PI / 180 - PI / 2;
  float minAngle = (minutes + seconds / 60.0) * 6 * PI / 180 - PI / 2;
  float hourAngle = ((hours % 12) + minutes / 60.0) * 30 * PI / 180 - PI / 2;

  display.clearDisplay();

  display.drawCircle(centerX, centerY, 30, WHITE);
  display.fillCircle(centerX, centerY, 2, WHITE);

  drawHand(hourAngle, 12, WHITE);
  drawHand(minAngle, 20, WHITE);
  drawHand(secAngle, 28, WHITE);

  display.display();
  delay(50);  // Fast refresh
}
