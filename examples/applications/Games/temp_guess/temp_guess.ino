#include <IRremote.hpp>
#include <DHT.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "SMLimage.h"

#define DHTPIN 8
#define DHTTYPE DHT11
#define IR_PIN 6

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Corrected IR codes
const unsigned long IR_0 = 0xAD52FF00UL;
const unsigned long IR_1 = 0xE916FF00UL;
const unsigned long IR_2 = 0xE619FF00UL;
const unsigned long IR_3 = 0xF20DFF00UL;
const unsigned long IR_4 = 0xF30CFF00UL;
const unsigned long IR_5 = 0xE718FF00UL;
const unsigned long IR_6 = 0xA15EFF00UL;
const unsigned long IR_7 = 0xF708FF00UL;
const unsigned long IR_8 = 0xE31CFF00UL;
const unsigned long IR_9 = 0xA55AFF00UL;
const unsigned long IR_OK = 0xBF40FF00UL;
const unsigned long IR_dot = 0xBD42FF00UL;  // * button = decimal point

String guess = "";
float fixedTemperature = 0;

void setup() {
  Serial.begin(9600);
  IrReceiver.begin(IR_PIN, ENABLE_LED_FEEDBACK);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) while (true);

  display.clearDisplay();
  display.drawBitmap(0, 0, SMLimage, 128, 64, SSD1306_WHITE);
  display.display();
  delay(2000);

  showReadingMessage();
  delay(1000);

  DHT dht(DHTPIN, DHTTYPE);
  dht.begin();
  fixedTemperature = dht.readTemperature();

  showPrompt();
}

void loop() {
  if (IrReceiver.decode()) {
    unsigned long code = IrReceiver.decodedIRData.decodedRawData;
    IrReceiver.resume();

    int digit = mapIRToDigit(code);
    if (digit != -1) {
      guess += String(digit);
      showGuess();
    } else if (code == IR_dot && guess.indexOf('.') == -1) {  // Fix: use indexOf instead of contains
      guess += ".";
      showGuess();
    } else if (code == IR_OK && guess.length() > 0) {
      showChecking();
      showResult(fixedTemperature);
      delay(3000);
      guess = "";
      showPrompt();
    }
  }
}

int mapIRToDigit(unsigned long code) {
  if (code == IR_0) return 0;
  if (code == IR_1) return 1;
  if (code == IR_2) return 2;
  if (code == IR_3) return 3;
  if (code == IR_4) return 4;
  if (code == IR_5) return 5;
  if (code == IR_6) return 6;
  if (code == IR_7) return 7;
  if (code == IR_8) return 8;
  if (code == IR_9) return 9;
  return -1;
}

void showPrompt() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Guess the temperature");
  display.setCursor(0, 20);
  display.println("Use remote to enter");
  display.setCursor(0, 40);
  display.println("Press OK to submit");
  display.display();
}

void showGuess() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Your Guess:");
  display.setTextSize(2);
  display.setCursor(30, 24);
  display.print(guess);
  display.print(" C");
  display.display();
}

void showResult(float actual) {
  float guessedValue = guess.toFloat();

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Your Guess:");
  display.setCursor(0, 12);
  display.print(guessedValue);
  display.println(" C");
  display.setCursor(0, 30);
  display.println("Actual Temp:");
  display.setCursor(0, 42);
  display.print(actual);
  display.println(" C");
  display.display();
}

void showChecking() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 24);
  display.println("Checking...");
  display.display();
}

void showReadingMessage() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 24);
  display.println("Reading temperature...");
  display.display();
}
