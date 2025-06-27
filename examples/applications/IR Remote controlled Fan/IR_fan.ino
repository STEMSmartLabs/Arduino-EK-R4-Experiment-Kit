#include <IRremote.hpp>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED config
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Pin config
const int RECV_PIN = 6;   // IR sensor pin
const int fanPin = 10;    // PWM fan pin

// Fan state
int fanSpeed = 0;
bool fanOn = false;

// IR codes from remote
const unsigned long IR_BUTTON_TOGGLE     = 0xBF40FF00;  // OK button (toggle)
const unsigned long IR_BUTTON_SPEED_UP   = 0xB946FF00;  // UP
const unsigned long IR_BUTTON_SPEED_DOWN = 0xEA15FF00;  // DOWN

void setup() {
  Serial.begin(9600);
  pinMode(fanPin, OUTPUT);
  analogWrite(fanPin, 0); // Start with fan off

  IrReceiver.begin(RECV_PIN, ENABLE_LED_FEEDBACK);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true);
  }

  display.display();
  delay(2000);
  display.clearDisplay();
  updateOLED();
}

void loop() {
  if (IrReceiver.decode()) {
    unsigned long code = IrReceiver.decodedIRData.decodedRawData;
    Serial.print("IR Code: 0x");
    Serial.println(code, HEX);

    if (code == IR_BUTTON_TOGGLE) {
      fanOn = !fanOn; // Toggle fan ON/OFF
      fanSpeed = fanOn ? 100 : 0;
    } 
    else if (code == IR_BUTTON_SPEED_UP && fanOn) {
      fanSpeed += 50;
      if (fanSpeed > 255) fanSpeed = 255;
    } 
    else if (code == IR_BUTTON_SPEED_DOWN && fanOn) {
      fanSpeed -= 50;
      if (fanSpeed < 0) fanSpeed = 0;
    }

    analogWrite(fanPin, fanOn ? fanSpeed : 0);
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
    display.print("Status: ON");
    display.setTextSize(1);
    display.setCursor(0, 48);
    display.print("Speed: ");
    display.print(map(fanSpeed, 0, 255, 0, 100));
    display.println("%");
  } else {
    display.println("Status: OFF");
  }

  display.display();
}
