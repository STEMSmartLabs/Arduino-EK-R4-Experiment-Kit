#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <MPU6050.h>
#include "SMLimage.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define BUZZER_PIN 11

MPU6050 mpu;
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
  pinMode(BUZZER_PIN, OUTPUT);
  Wire.begin();
  mpu.initialize();
  showSMLLogo();
}

void loop() {
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);

  int shake = abs(ax) + abs(ay) + abs(az);
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.print("Seismic Data: ");
  display.println(shake);
  
  if (shake > 50000) {
    display.println("!! Tremor Detected !!");
    tone(BUZZER_PIN, 1000, 500);
  }

  display.drawLine(0, 60 - (shake / 10000), 128, 60 - (shake / 10000), WHITE);
  display.display();
  delay(300);
}
