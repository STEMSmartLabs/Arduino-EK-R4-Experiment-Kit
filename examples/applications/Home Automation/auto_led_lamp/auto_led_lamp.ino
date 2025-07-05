#include <Wire.h>           // I2C communication
#include <Adafruit_GFX.h>   // Core graphics library
#include <Adafruit_SSD1306.h> // SSD1306 OLED display library
#include "SMLimage.h"       // Custom startup image

// OLED Display Configuration
#define SCREEN_WIDTH 128    // OLED width
#define SCREEN_HEIGHT 64    // OLED height
#define OLED_RESET -1       // Reset pin
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int ldrPin = A0;      // LDR to Analog Pin A0
const int lightPin = 3;     // LED to Digital Pin 3 (PWM)

const int minLDRValue = 50;  // LDR value for "dark"
const int maxLDRValue = 180; // LDR value for "bright"

void setup() {
  pinMode(lightPin, OUTPUT);
  Serial.begin(9600);

  // OLED Initialization
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed."));
    for (;;);
  }

  // Display startup image
  display.clearDisplay();
  display.drawBitmap(0, 0, SMLimage, 128, 64, SSD1306_WHITE);
  display.display();
  delay(2000); // Display for 2 seconds

  display.clearDisplay();
  display.display();
}

void loop() {
  int ldrValue = analogRead(ldrPin); // Read LDR value (0-1023)

  // Map LDR value to LED brightness (inverted for darkness -> higher brightness)
  int brightness = map(ldrValue, minLDRValue, maxLDRValue, 255, 0);
  brightness = constrain(brightness, 0, 255); // Constrain brightness
  analogWrite(lightPin, brightness); // Set LED brightness

  // Update OLED display with status
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  if (ldrValue <= minLDRValue) {
    display.println("Status: DARK");
  } else if (ldrValue >= maxLDRValue) {
    display.println("Status: BRIGHT");
  } else {
    display.println("Status: DIM");
  }
  display.display();

  // Output to Serial Monitor for debugging
  Serial.print("LDR Value: ");
  Serial.println(ldrValue);
  Serial.print("LED Brightness: ");
  Serial.println(brightness);

  delay(100); // Small delay for stable readings
}
