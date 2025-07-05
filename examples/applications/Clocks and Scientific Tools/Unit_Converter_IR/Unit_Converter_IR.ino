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
int mode = 0; // 0: cm to inches, 1: kg to lb, 2: °C to °F

void showSMLLogo() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.drawBitmap(0, 0, SMLimage, 128, 64, 1);
  display.display();
  delay(2000);
  display.clearDisplay();
}

void updateDisplay(float result) {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  if (mode == 0) display.print("cm -> inches");
  else if (mode == 1) display.print("kg -> lb");
  else display.print("C -> F");
  display.setCursor(0, 20);
  display.setTextSize(2);
  display.print(input);
  display.print(" = ");
  display.println(result, 1);
  display.display();
}

void setup() {
  IrReceiver.begin(IR_PIN);
  showSMLLogo();
}

void loop() {
  if (IrReceiver.decode()) {
    uint8_t code = IrReceiver.decodedIRData.command;
    
    if (code >= 0x1E && code <= 0x27) {
      input += String(code - 0x1E); // digits
    } else if (code == 0x1C) { // OK = Convert
      float value = input.toFloat();
      float result = 0;
      if (mode == 0) result = value * 0.393701;
      else if (mode == 1) result = value * 2.20462;
      else result = value * 1.8 + 32;
      updateDisplay(result);
      input = "";
    } else if (code == 0x43) { // Power = clear
      input = "";
      display.clearDisplay();
      display.display();
    } else if (code == 0x40) { // Menu = switch mode
      mode = (mode + 1) % 3;
    }

    IrReceiver.resume();
  }
}
