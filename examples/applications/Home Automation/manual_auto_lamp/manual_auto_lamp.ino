#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "SMLimage.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Pin Definitions
#define LDR_PIN A0
#define MODE_SWITCH A3     // Auto/Manual toggle
#define MANUAL_SWITCH 12   // Controls lamp in Manual Mode
#define LED_PIN 3

bool isAutoMode = true;
bool lastModeState = false;

void showSMLLogo() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.drawBitmap(0, 0, SMLimage, 128, 64, 1);
  display.display();
  delay(2000);
  display.clearDisplay();
}

void setup() {
  pinMode(MODE_SWITCH, INPUT);
  pinMode(MANUAL_SWITCH, INPUT);
  pinMode(LED_PIN, OUTPUT);

  showSMLLogo();
}

void loop() {
  // Read mode
  isAutoMode = digitalRead(MODE_SWITCH);

  int ldrValue = analogRead(LDR_PIN);
  bool ledState = false;

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  if (isAutoMode) {
    display.println("Mode: AUTO");
    if (ldrValue < 500) { // Adjust threshold for darkness
      digitalWrite(LED_PIN, HIGH);
      ledState = true;
    } else {
      digitalWrite(LED_PIN, LOW);
      ledState = false;
    }
  } else {
    display.println("Mode: MANUAL");
    if (digitalRead(MANUAL_SWITCH) == HIGH) {
      digitalWrite(LED_PIN, HIGH);
      ledState = true;
    } else {
      digitalWrite(LED_PIN, LOW);
      ledState = false;
    }
  }

  display.setCursor(0, 20);
  display.print("Lamp: ");
  display.println(ledState ? "ON" : "OFF");

  display.display();
  delay(200);
}
