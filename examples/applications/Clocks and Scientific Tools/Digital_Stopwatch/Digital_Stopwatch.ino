#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "SMLimage.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

#define BUTTON_PIN 12    // Push-button
#define SWITCH_PIN A3    // Rocker switch (reset)
#define BUZZER_PIN 11

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

bool running = false;
bool paused = false;
unsigned long startTime = 0;
unsigned long elapsedTime = 0;

void showSMLLogo() {
  display.clearDisplay();
  display.drawBitmap(0, 0, SMLimage, 128, 64, 1);
  display.display();
  delay(2000);
  display.clearDisplay();
}

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Button to GND
  pinMode(SWITCH_PIN, INPUT_PULLUP); // Switch to GND
  pinMode(BUZZER_PIN, OUTPUT);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (1); // halt on OLED fail
  }

  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.clearDisplay();
  display.display();

  showSMLLogo();
}

void loop() {
  static bool lastButtonState = HIGH;
  static bool lastSwitchState = HIGH;

  bool buttonState = digitalRead(BUTTON_PIN);
  bool switchState = digitalRead(SWITCH_PIN);

  // Handle button press: start, pause, resume
  if (lastButtonState == HIGH && buttonState == LOW) {
    delay(50);  // debounce
    if (!running) {
      running = true;
      paused = false;
      startTime = millis();
      tone(BUZZER_PIN, 1000, 100);  // start beep
    } else if (!paused) {
      paused = true;
      elapsedTime = millis() - startTime;
      tone(BUZZER_PIN, 500, 100);   // pause beep
    } else {
      paused = false;
      startTime = millis() - elapsedTime;
      tone(BUZZER_PIN, 750, 100);   // resume beep
    }
  }
  lastButtonState = buttonState;

  // Handle switch flick (LOW): reset to Idle
  if (switchState == LOW && lastSwitchState == HIGH) {
    running = false;
    paused = false;
    elapsedTime = 0;
    startTime = 0;
    tone(BUZZER_PIN, 300, 100);  // reset beep
  }
  lastSwitchState = switchState;

  // Calculate time
  unsigned long timeNow = paused ? elapsedTime : (millis() - startTime);

  // Display status
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println(paused ? "Paused" : running ? "Running" : "Idle");

  // Display time (X.Y format)
  display.setTextSize(2);
  display.setCursor(0, 25);
  if (running || paused) {
    display.print(timeNow / 1000);
    display.print(".");
    display.print((timeNow % 1000) / 100);
  } else {
    display.print("0.0");
  }

  display.display();
  delay(100);
}
