#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "SMLimage.h"

#define BUZZER_PIN 11
#define POT_PIN A1

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(POT_PIN, INPUT);

  // Initialize OLED
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();

  // Show SML logo
  display.drawBitmap(0, 0, SMLimage, 128, 64, SSD1306_WHITE);
  display.display();
  delay(2000);
  display.clearDisplay();
}

void loop() {
  int potValue = analogRead(POT_PIN);             // Read analog value (0-1023)
  int frequency = map(potValue, 0, 1023, 100, 2000); // Map to pitch (100–2000 Hz)

  tone(BUZZER_PIN, frequency);

  // Display frequency info
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Potentiometer Buzzer");
  display.setCursor(0, 20);
  display.print("Analog: ");
  display.println(potValue);
  display.setCursor(0, 35);
  display.print("Freq: ");
  display.print(frequency);
  display.println(" Hz");
  display.display();

  delay(100);
}
