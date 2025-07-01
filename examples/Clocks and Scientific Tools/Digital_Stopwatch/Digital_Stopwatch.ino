#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "SMLimage.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

#define BUTTON_PIN 12     // Connect one side of the button to GND
#define BUZZER_PIN 11

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

bool running = false;
bool paused = false;
unsigned long startTime = 0;
unsigned long elapsedTime = 0;

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
  pinMode(BUZZER_PIN, OUTPUT);
  showSMLLogo();
}

void loop() {
  static bool lastButtonState = HIGH;
  bool buttonState = digitalRead(BUTTON_PIN);

  if (lastButtonState == HIGH && buttonState == LOW) {
    delay(50); // debounce
    if (!running) {
      running = true;
      paused = false;
      startTime = millis();
      tone(BUZZER_PIN, 1000, 100);
    } else if (!paused) {
      paused = true;
      elapsedTime = millis() - startTime;
      tone(BUZZER_PIN, 500, 100);
    } else {
      paused = false;
      startTime = millis() - elapsedTime;
      tone(BUZZER_PIN, 750, 100);
    }
  }
  lastButtonState = buttonState;

  unsigned long timeNow = paused ? elapsedTime : (millis() - startTime);

  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 20);
  display.print(timeNow / 1000);
  display.print(".");
  display.print((timeNow % 1000) / 100);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print(paused ? "Paused" : running ? "Running" : "Idle");
  display.display();

  delay(100);
}
