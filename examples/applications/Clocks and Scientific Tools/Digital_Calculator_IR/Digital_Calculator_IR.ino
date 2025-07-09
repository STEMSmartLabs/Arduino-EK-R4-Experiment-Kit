#include <IRremote.hpp>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "SMLimage.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define IR_PIN 6

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Operation menu
const char* operations[] = { "ADD (+)", "SUB (-)", "MUL (*)", "DIV (/)" };
const char symbols[] = { '+', '-', '*', '/' };
int selectedOpIndex = 0;
bool selectingOperation = true;
bool enteringNumber = false;

String input = "";
char operation = 0;

// IR button codes
const unsigned long OK_BUTTON    = 0xBF40FF00;
const unsigned long UP_BUTTON    = 0xB946FF00;
const unsigned long DOWN_BUTTON  = 0xEA15FF00;
const unsigned long STAR_BUTTON  = 0xBD42FF00; // '*'
const unsigned long CLEAR_BUTTON = 0xB54AFF00; // '#'

// Digit codes
const struct {
  unsigned long code;
  char digit;
} digitButtons[] = {
  {0xE916FF00, '1'}, {0xE619FF00, '2'}, {0xF20DFF00, '3'},
  {0xF30CFF00, '4'}, {0xE718FF00, '5'}, {0xA15EFF00, '6'},
  {0xF708FF00, '7'}, {0xE31CFF00, '8'}, {0xA55AFF00, '9'},
  {0xAD52FF00, '0'}
};

void showSMLLogo() {
  display.clearDisplay();
  display.drawBitmap(0, 0, SMLimage, 128, 64, 1);
  display.display();
  delay(2000);
  display.clearDisplay();
}

void showReadyMessage() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(30, 25);
  display.println("Ready");
  display.display();
  delay(2000);
}

void showOperationMenu() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  for (int i = 0; i < 4; i++) {
    display.setCursor(10, 10 + i * 12);
    display.print((i == selectedOpIndex) ? "> " : "  ");
    display.println(operations[i]);
  }
  display.display();
}

void showInputDisplay() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Enter:");
  display.setTextSize(2);
  display.setCursor(0, 25);
  display.println(input);
  display.display();
}

void showResult(long result) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 20);
  display.print("= ");
  display.println(result);
  display.display();
  delay(3000); // Pause to show result
}

void resetCalculator() {
  selectedOpIndex = 0;
  selectingOperation = true;
  enteringNumber = false;
  input = "";
  operation = 0;
  showOperationMenu();
}

void setup() {
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  IrReceiver.begin(IR_PIN, ENABLE_LED_FEEDBACK);
  showSMLLogo();
  showReadyMessage();
  showOperationMenu();
}

void loop() {
  if (IrReceiver.decode()) {
    unsigned long code = IrReceiver.decodedIRData.decodedRawData;

    // Clear/reset
    if (code == CLEAR_BUTTON) {
      resetCalculator();
      IrReceiver.resume();
      return;
    }

    if (selectingOperation) {
      if (code == UP_BUTTON) {
        selectedOpIndex = (selectedOpIndex + 3) % 4;
        showOperationMenu();
      } else if (code == DOWN_BUTTON) {
        selectedOpIndex = (selectedOpIndex + 1) % 4;
        showOperationMenu();
      } else if (code == OK_BUTTON) {
        operation = symbols[selectedOpIndex];
        selectingOperation = false;
        enteringNumber = true;
        input = "";
        showInputDisplay();
      }
    } else if (enteringNumber) {
      // Handle digits
      for (int i = 0; i < 10; i++) {
        if (code == digitButtons[i].code) {
          input += digitButtons[i].digit;
          showInputDisplay();
        }
      }

      // Handle operation insert via '*'
      if (code == STAR_BUTTON && operation != 0 && !input.endsWith(String(operation))) {
        input += operation;
        showInputDisplay();
      }

      // Confirm input and evaluate
      if (code == OK_BUTTON) {
        int opIndex = input.indexOf(operation);
        if (opIndex > 0 && opIndex < input.length() - 1) {
          long a = input.substring(0, opIndex).toInt();
          long b = input.substring(opIndex + 1).toInt();
          long result = 0;
          switch (operation) {
            case '+': result = a + b; break;
            case '-': result = a - b; break;
            case '*': result = a * b; break;
            case '/': result = (b != 0) ? a / b : 0; break;
          }
          showResult(result);
        }
        resetCalculator();  // Go back to menu
      }
    }

    IrReceiver.resume();
  }
}
