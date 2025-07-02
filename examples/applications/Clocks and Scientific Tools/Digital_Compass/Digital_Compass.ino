#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "SMLimage.h"
#include <MPU6050.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

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
  Wire.begin();
  mpu.initialize();
  showSMLLogo();
  display.setTextSize(1);
  display.setTextColor(WHITE);
}

void loop() {
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);

  float angle = atan2(ay, ax) * 180 / PI;
  if (angle < 0) angle += 360;

  String direction;
  if (angle >= 315 || angle < 45) direction = "E";
  else if (angle >= 45 && angle < 135) direction = "N";
  else if (angle >= 135 && angle < 225) direction = "W";
  else direction = "S";

  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(2);
  display.print("Dir: ");
  display.println(direction);
  display.setTextSize(1);
  display.print("Angle: ");
  display.println(angle, 1);
  display.display();

  delay(500);
}
