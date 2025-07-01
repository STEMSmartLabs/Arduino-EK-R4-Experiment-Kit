#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "SMLimage.h"

#define BUTTON_PIN 12
#define LED_PIN 3
#define BUZZER_PIN 11

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Timing thresholds
const unsigned long dotThreshold = 300;  // Press < 300ms → dot
const unsigned long debounceDelay = 50;

bool lastButtonState = HIGH;
unsigned long pressStartTime = 0;
String morseMessage = "";

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.drawBitmap(0, 0, SMLimage, 128, 64, SSD1306_WHITE);
  display.display();
  delay(2000);
  display.clearDisplay();
  display.display();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Morse Code Blinker");
  display.display();
}

void loop() {
  bool currentState = digitalRead(BUTTON_PIN);

  // Button pressed
  if (lastButtonState == HIGH && currentState == LOW) {
    pressStartTime = millis();
  }

  // Button released
  if (lastButtonState == LOW && currentState == HIGH) {
    unsigned long pressDuration = millis() - pressStartTime;

    if (pressDuration > debounceDelay) {
      if (pressDuration < dotThreshold) {
        blinkSymbol('.');
      } else {
        blinkSymbol('-');
      }
    }
  }

  lastButtonState = currentState;
}

void blinkSymbol(char symbol) {
  digitalWrite(LED_PIN, HIGH);
  tone(BUZZER_PIN, 1000);
  delay(symbol == '.' ? 200 : 600);
  digitalWrite(LED_PIN, LOW);
  noTone(BUZZER_PIN);
  delay(100);

  morseMessage += symbol;

  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.println("Morse Code Blinker");
  display.setCursor(0, 20);
  display.print("Message: ");
  display.println(morseMessage);
  display.display();
}
