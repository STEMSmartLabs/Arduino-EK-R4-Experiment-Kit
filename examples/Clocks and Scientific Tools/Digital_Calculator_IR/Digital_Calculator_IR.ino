#include <IRremote.hpp>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "SMLimage.h"

#define IR_PIN 6
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

String input = "";
long result = 0;
char operation = 0;

void showSMLLogo() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.drawBitmap(0, 0, SMLimage, 128, 64, 1);
  display.display();
  delay(2000);
  display.clearDisplay();
}

void updateDisplay() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(2);
  display.println(input);
  display.display();
}

void setup() {
  IrReceiver.begin(IR_PIN);
  showSMLLogo();
  updateDisplay();
}

void loop() {
  if (IrReceiver.decode()) {
    uint8_t code = IrReceiver.decodedIRData.command;
    if (code >= 0x1E && code <= 0x27) {  // Numbers 0-9
      input += String(code - 0x1E);
    } else if (code == 0x0D) { // '+'
      result = input.toInt();
      input = "";
      operation = '+';
    } else if (code == 0x19) { // '-'
      result = input.toInt();
      input = "";
      operation = '-';
    } else if (code == 0x15) { // '*'
      result = input.toInt();
      input = "";
      operation = '*';
    } else if (code == 0x07) { // '/'
      result = input.toInt();
      input = "";
      operation = '/';
    } else if (code == 0x1C) { // '=' (OK)
      long second = input.toInt();
      switch (operation) {
        case '+': result += second; break;
        case '-': result -= second; break;
        case '*': result *= second; break;
        case '/': result = second != 0 ? result / second : 0; break;
      }
      input = String(result);
    } else if (code == 0x43) { // 'C' (power button)
      input = "";
      result = 0;
      operation = 0;
    }
    updateDisplay();
    IrReceiver.resume();
  }
}
