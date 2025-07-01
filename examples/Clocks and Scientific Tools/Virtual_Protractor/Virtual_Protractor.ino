#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <MPU6050.h>
#include "SMLimage.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define LED_PIN 3

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
MPU6050 mpu;

void showSMLLogo() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.drawBitmap(0, 0, SMLimage, 128, 64, 1);
  display.display();
  delay(2000);
  display.clearDisplay();
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Wire.begin();
  mpu.initialize();
  showSMLLogo();
  display.setTextSize(2);
  display.setTextColor(WHITE);
}

void loop() {
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);

  float angle = atan2(ay, az) * 180 / PI;
  if (angle < 0) angle += 180;

  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Angle:");
  display.setTextSize(2);
  display.setCursor(0, 20);
  display.print((int)angle);
  display.print(" deg");
  display.display();

  if (angle >= 88 && angle <= 92) digitalWrite(LED_PIN, HIGH);
  else digitalWrite(LED_PIN, LOW);

  delay(300);
}
