#include <IRremote.hpp>
#include <EEPROM.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "SMLimage.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// IR receiver
#define IR_PIN 6

#define EEPROM_ADDR_PASSWORD 0
const byte ENCRYPTION_KEY = 0x5A;

// IR Codes
const unsigned long OK_BUTTON = 0xBF40FF00;
const unsigned long NUM_0     = 0xAD52FF00;
const unsigned long NUM_1     = 0xE916FF00;
const unsigned long NUM_2     = 0xE619FF00;
const unsigned long NUM_3     = 0xF20DFF00;
const unsigned long NUM_4     = 0xF30CFF00;
const unsigned long NUM_5     = 0xE718FF00;
const unsigned long NUM_6     = 0xA15EFF00;
const unsigned long NUM_7     = 0xF708FF00;
const unsigned long NUM_8     = 0xE31CFF00;
const unsigned long NUM_9     = 0xA55AFF00;

String newPassword = "";

void setup() {
  Wire.begin();                    // ✅ Ensure I2C starts before display
  IrReceiver.begin(IR_PIN, ENABLE_LED_FEEDBACK);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) while (true);

  display.clearDisplay();
  display.drawBitmap(0, 0, SMLimage, 128, 64, 1);
  display.display();
  delay(2000);
  display.clearDisplay();

  display.setTextColor(SSD1306_WHITE);  // ✅ Needed after clear
  showPasswordScreen();
}

void loop() {
  if (IrReceiver.decode()) {
    unsigned long code = IrReceiver.decodedIRData.decodedRawData;
    char digit = mapIRToDigit(code);

    if (digit != '-') {
      if (newPassword.length() < 4) {
        newPassword += digit;
        showPasswordScreen();
      }
    }

    if (code == OK_BUTTON && newPassword.length() == 4) {
      storeEncryptedPassword(newPassword);
      showSuccessMessage();
      while (true);  // Done
    }

    IrReceiver.resume();
  }
}

// ----------- Helper Functions -----------

char mapIRToDigit(unsigned long code) {
  if (code == NUM_0) return '0';
  if (code == NUM_1) return '1';
  if (code == NUM_2) return '2';
  if (code == NUM_3) return '3';
  if (code == NUM_4) return '4';
  if (code == NUM_5) return '5';
  if (code == NUM_6) return '6';
  if (code == NUM_7) return '7';
  if (code == NUM_8) return '8';
  if (code == NUM_9) return '9';
  return '-';
}

void showPasswordScreen() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE); // ✅ Ensure color is set
  display.setTextSize(1);
  display.setCursor(10, 0);
  display.println("Enter New Password:");

  display.setTextSize(2);
  display.setCursor(34, 30);
  for (int i = 0; i < 4; i++) {
    if (i < newPassword.length()) {
      display.print("*");
    } else if (i == newPassword.length()) {
      display.print("_");
    } else {
      display.print(" ");
    }
  }

  display.display();
}

void storeEncryptedPassword(String pwd) {
  for (int i = 0; i < 4; i++) {
    char encryptedChar = pwd[i] ^ ENCRYPTION_KEY;
    EEPROM.update(EEPROM_ADDR_PASSWORD + i, encryptedChar);
  }
}

void showSuccessMessage() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(2);
  display.setCursor(30, 20);
  display.println("Saved!");
  display.display();
}
