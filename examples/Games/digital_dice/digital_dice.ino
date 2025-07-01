#include <IRremote.hpp>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "SMLimage.h"

#define OLED_RESET -1
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define IR_PIN 6
#define BUZZER_PIN 11
#define BUTTON_ROLL 0xBF40FF00

bool waitingForInput = true;

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  Serial.begin(9600);
  IrReceiver.begin(IR_PIN, ENABLE_LED_FEEDBACK);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (true); // stop on failure
  }

  randomSeed(analogRead(A0));

  // Show SML logo
  display.clearDisplay();
  display.drawBitmap(0, 0, SMLimage, 128, 64, SSD1306_WHITE);
  display.display();
  delay(2000);

  showPrompt();
}

void loop() {
  if (IrReceiver.decode()) {
    unsigned long code = IrReceiver.decodedIRData.decodedRawData;
    if (code == BUTTON_ROLL && waitingForInput) {
      waitingForInput = false;
      rollDice();
      delay(1500);
      showPrompt();
      waitingForInput = true;
    }
    IrReceiver.resume();
  }
}

void rollDice() {
  // Animation: flick through faces
  for (int i = 0; i < 8; i++) {
    int face = random(1, 7);
    drawDiceDots(face);
    delay(100);
  }

  // Final result
  int result = random(1, 7);
  drawDiceDots(result);
  displayNumber(result);
  tone(BUZZER_PIN, 800, 150);
}

void drawDiceDots(int value) {
  display.clearDisplay();

  int centerX = SCREEN_WIDTH / 2;
  int centerY = SCREEN_HEIGHT / 2;

  int r = 4; // radius of each dot
  int spacing = 10;

  // Coordinates for 3x3 dice positions
  int x1 = centerX - spacing;
  int x2 = centerX;
  int x3 = centerX + spacing;
  int y1 = centerY - spacing;
  int y2 = centerY;
  int y3 = centerY + spacing;

  // Draw dots based on value
  switch (value) {
    case 1:
      display.fillCircle(x2, y2, r, SSD1306_WHITE);
      break;
    case 2:
      display.fillCircle(x1, y1, r, SSD1306_WHITE);
      display.fillCircle(x3, y3, r, SSD1306_WHITE);
      break;
    case 3:
      display.fillCircle(x1, y1, r, SSD1306_WHITE);
      display.fillCircle(x2, y2, r, SSD1306_WHITE);
      display.fillCircle(x3, y3, r, SSD1306_WHITE);
      break;
    case 4:
      display.fillCircle(x1, y1, r, SSD1306_WHITE);
      display.fillCircle(x3, y1, r, SSD1306_WHITE);
      display.fillCircle(x1, y3, r, SSD1306_WHITE);
      display.fillCircle(x3, y3, r, SSD1306_WHITE);
      break;
    case 5:
      display.fillCircle(x1, y1, r, SSD1306_WHITE);
      display.fillCircle(x3, y1, r, SSD1306_WHITE);
      display.fillCircle(x2, y2, r, SSD1306_WHITE);
      display.fillCircle(x1, y3, r, SSD1306_WHITE);
      display.fillCircle(x3, y3, r, SSD1306_WHITE);
      break;
    case 6:
      display.fillCircle(x1, y1, r, SSD1306_WHITE);
      display.fillCircle(x3, y1, r, SSD1306_WHITE);
      display.fillCircle(x1, y2, r, SSD1306_WHITE);
      display.fillCircle(x3, y2, r, SSD1306_WHITE);
      display.fillCircle(x1, y3, r, SSD1306_WHITE);
      display.fillCircle(x3, y3, r, SSD1306_WHITE);
      break;
  }

  display.display();
}

void displayNumber(int number) {
  // Draw number below dots
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor((SCREEN_WIDTH - 84) / 2, 5);
  display.println("You Rolled:");

  display.setTextSize(2);
  int x = (SCREEN_WIDTH - (12 * String(number).length())) / 2;
  display.setCursor(x, 48);
  display.print(number);

  display.display();
}

void showPrompt() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor((SCREEN_WIDTH - 96) / 2, 20);
  display.println("Press OK on Remote");
  display.setCursor((SCREEN_WIDTH - 66) / 2, 36);
  display.println("to roll the dice");
  display.display();
}
