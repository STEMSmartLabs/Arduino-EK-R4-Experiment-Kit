#include <IRremote.hpp>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <EEPROM.h>
#include "SMLimage.h"  // Startup logo

// OLED config
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Pins
const int RECV_PIN = 6;
const int fanPin = 10;

// EEPROM address
const int EEPROM_FAN_SPEED_ADDR = 0;

int fanSpeed = 0;
bool fanOn = false;

// IR codes
const unsigned long IR_BUTTON_TOGGLE     = 0xBF40FF00;  // OK
const unsigned long IR_BUTTON_SPEED_UP   = 0xB946FF00;  // UP
const unsigned long IR_BUTTON_SPEED_DOWN = 0xEA15FF00;  // DOWN

void setup() {
  Serial.begin(9600);
  pinMode(fanPin, OUTPUT);

  // Load saved speed
  fanSpeed = EEPROM.read(EEPROM_FAN_SPEED_ADDR);
  if (fanSpeed > 0) {
    fanOn = true;
    analogWrite(fanPin, fanSpeed);
  } else {
    fanOn = false;
    fanSpeed = 100; // Default startup speed when OFF
    analogWrite(fanPin, 0);
  }

  IrReceiver.begin(RECV_PIN, ENABLE_LED_FEEDBACK);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true);
  }

  // Show logo
  display.clearDisplay();
  display.drawBitmap(0, 0, SMLimage, 128, 64, SSD1306_WHITE);
  display.display();
  delay(2000);
  display.clearDisplay();
  display.display();

  updateOLED();
}

void loop() {
  if (IrReceiver.decode()) {
    unsigned long code = IrReceiver.decodedIRData.decodedRawData;
    Serial.print("IR Code: 0x");
    Serial.println(code, HEX);

    if (code == IR_BUTTON_TOGGLE) {
      fanOn = !fanOn;
      analogWrite(fanPin, fanOn ? fanSpeed : 0);
    } 
    else if (code == IR_BUTTON_SPEED_UP) {
      fanSpeed += 50;
      if (fanSpeed > 255) fanSpeed = 255;
      if (fanOn) analogWrite(fanPin, fanSpeed);
      saveFanSpeed(fanSpeed);
    } 
    else if (code == IR_BUTTON_SPEED_DOWN) {
      fanSpeed -= 50;
      if (fanSpeed < 0) fanSpeed = 0;
      if (fanOn) analogWrite(fanPin, fanSpeed);
      saveFanSpeed(fanSpeed);
    }

    updateOLED();
    IrReceiver.resume();
  }

  delay(100);
}

void updateOLED() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Fan Control");

  display.setTextSize(2);
  display.setCursor(0, 20);

  if (fanOn) {
    display.print("Status:ON");
    display.setTextSize(1);
    display.setCursor(0, 48);
    display.print("Speed: ");
    int percent = round(fanSpeed * 100.0 / 255);
    display.print(percent);
    display.println("%");
  } else {
    display.println("Status:OFF");
    display.setTextSize(1);
    display.setCursor(0, 48);
    display.print("Saved Speed: ");
    int percent = round(fanSpeed * 100.0 / 255);
    display.print(percent);
    display.println("%");
  }

  display.display();
}

void saveFanSpeed(int speed) {
  if (EEPROM.read(EEPROM_FAN_SPEED_ADDR) != speed) {
    EEPROM.write(EEPROM_FAN_SPEED_ADDR, speed);
  }
}
