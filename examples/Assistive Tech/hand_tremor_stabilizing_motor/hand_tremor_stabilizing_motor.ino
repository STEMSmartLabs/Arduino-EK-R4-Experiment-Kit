#include <Servo.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "SMLimage.h"

#define SERVO_PIN 9
#define LED_PIN 3
#define BUTTON_PIN 12
#define ACCEL_PIN A2
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Servo stabilizerServo;
int mode = 0;

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
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  stabilizerServo.attach(SERVO_PIN);
  showSMLLogo();
}

void loop() {
  if (digitalRead(BUTTON_PIN) == LOW) {
    mode = (mode + 1) % 3;
    delay(300);
  }

  int tremor = analogRead(ACCEL_PIN);
  int correction = map(tremor, 300, 700, 70, 110);
  stabilizerServo.write(constrain(correction, 60, 120));
  digitalWrite(LED_PIN, mode > 0 ? HIGH : LOW);

  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.println("Stabilizer Mode:");
  if (mode == 0) display.println("OFF");
  else if (mode == 1) display.println("Soup");
  else display.println("Rice");
  display.display();

  delay(100);
}
