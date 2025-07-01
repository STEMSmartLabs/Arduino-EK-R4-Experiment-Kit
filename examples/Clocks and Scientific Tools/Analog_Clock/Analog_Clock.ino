#include <Wire.h>
#include <RTClib.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "SMLimage.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

RTC_DS3231 rtc;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void showSMLLogo() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.drawBitmap(0, 0, SMLimage, 128, 64, 1);
  display.display();
  delay(2000);
  display.clearDisplay();
}

// Draws a clock hand from centerX, centerY at a given angle and length
void drawHand(int centerX, int centerY, float angleRad, int length) {
  int x = centerX + length * cos(angleRad);
  int y = centerY + length * sin(angleRad);
  display.drawLine(centerX, centerY, x, y, WHITE);
}

void setup() {
  Wire.begin();
  rtc.begin();
  showSMLLogo();
  display.setTextColor(WHITE);
}

void loop() {
  DateTime now = rtc.now();

  int centerX = SCREEN_WIDTH / 2;
  int centerY = SCREEN_HEIGHT / 2;

  float secondAngle = now.second() * 6 * PI / 180 - PI / 2;
  float minuteAngle = now.minute() * 6 * PI / 180 - PI / 2;
  float hourAngle = ((now.hour() % 12) + now.minute() / 60.0) * 30 * PI / 180 - PI / 2;

  display.clearDisplay();

  // Draw clock border
  display.drawCircle(centerX, centerY, 30, WHITE);

  // Draw hour, minute, and second hands
  drawHand(centerX, centerY, hourAngle, 12);   // Shorter hour hand
  drawHand(centerX, centerY, minuteAngle, 20); // Medium minute hand
  drawHand(centerX, centerY, secondAngle, 28); // Long second hand

  // Optional: Draw center dot
  display.fillCircle(centerX, centerY, 2, WHITE);

  display.display();
  delay(1000);
}
