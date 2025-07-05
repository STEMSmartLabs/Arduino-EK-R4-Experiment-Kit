#include <IRremote.hpp>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "SMLimage.h"

// OLED setup
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Pin definitions
#define IR_PIN 6
#define BUZZER_PIN 11
#define BUTTON_OK 0xBF40FF00

// IR hex codes for buttons 1–8
const unsigned long IR_KEYS[8] = {
  0xE916FF00, // 1
  0xE619FF00, // 2
  0xF20DFF00, // 3
  0xF30CFF00, // 4
  0xE718FF00, // 5
  0xA15EFF00, // 6
  0xF708FF00, // 7
  0xE31CFF00  // 8
};

// Frequencies for notes C D E F G A B C5
const int NOTES[8] = {
  262, 294, 330, 349, 392, 440, 494, 523
};

// Note names
const char* NOTE_NAMES[8] = {
  "C", "D", "E", "F", "G", "A", "B", "C5"
};

bool playing = false;

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  IrReceiver.begin(IR_PIN, ENABLE_LED_FEEDBACK);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (true); // Stop if OLED fails
  }

  // Show SML logo
  display.clearDisplay();
  display.drawBitmap(0, 0, SMLimage, 128, 64, SSD1306_WHITE);
  display.display();
  delay(2000);
  display.clearDisplay();

  // Show prompt to start
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(20, 24);
  display.println("Press OK to play");
  display.display();
}

void loop() {
  if (IrReceiver.decode()) {
    unsigned long code = IrReceiver.decodedIRData.decodedRawData;

    if (!playing && code == BUTTON_OK) {
      playing = true;
      showNotePrompt();
    }

    if (playing) {
      for (int i = 0; i < 8; i++) {
        if (code == IR_KEYS[i]) {
          tone(BUZZER_PIN, NOTES[i], 300);
          showNote(NOTE_NAMES[i]);
          break;
        }
      }
    }

    IrReceiver.resume();
  }
}

void showNotePrompt() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 24);
  display.println("Press 1-8 to play notes");
  display.display();
}

void showNote(const char* note) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 20);
  display.print("Note: ");
  display.println(note);
  display.display();
}
