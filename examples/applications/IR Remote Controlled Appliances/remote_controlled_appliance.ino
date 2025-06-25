#include <IRremote.hpp>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED setup
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// IR Receiver
const int RECV_PIN = 6;

// Appliances
const int ledPin = 3;
const int fanPin = 10;

// States
bool ledState = false;
bool fanState = false;
int selectedDevice = 0; // 0 = LED, 1 = FAN

// IR codes
const unsigned long OK_BUTTON    = 0xBF40FF00;
const unsigned long UP_BUTTON  = 0xB946FF00;
const unsigned long DOWN_BUTTON = 0xEA15FF00;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(fanPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  digitalWrite(fanPin, LOW);

  IrReceiver.begin(RECV_PIN, ENABLE_LED_FEEDBACK);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (true); // Stop if OLED init fails
  }

  display.display();
  delay(1000);
  display.clearDisplay();
  updateDisplay();
}

void loop() {
  if (IrReceiver.decode()) {
    unsigned long code = IrReceiver.decodedIRData.decodedRawData;

    if (code == UP_BUTTON) {
      selectedDevice = 0; // Select LED
    } else if (code == DOWN_BUTTON) {
      selectedDevice = 1; // Select FAN
    } else if (code == OK_BUTTON) {
      if (selectedDevice == 0) {
        ledState = !ledState;
        digitalWrite(ledPin, ledState ? HIGH : LOW);
      } else {
        fanState = !fanState;
        digitalWrite(fanPin, fanState ? HIGH : LOW);
      }
    }

    updateDisplay();
    IrReceiver.resume();
  }

  delay(100);
}

// ------------ OLED Display Update ------------
void updateDisplay() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Show LED row
  display.setCursor(0, 10);
  display.print((selectedDevice == 0) ? "> LED: " : "  LED: ");
  display.println(ledState ? "ON" : "OFF");

  // Show FAN row
  display.setCursor(0, 30);
  display.print((selectedDevice == 1) ? "> FAN: " : "  FAN: ");
  display.println(fanState ? "ON" : "OFF");

  display.display();
}
