#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_NeoPixel.h>
#include "SMLimage.h"  // Include the correct startup image header

// OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// NeoPixel
#define PIXEL_PIN D5
#define NUMPIXELS 12
Adafruit_NeoPixel pixels(NUMPIXELS, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

// Pins
const int trigPin = D4;
const int echoPin = D7;
const int buzzerPin = D11;
const int parkingModeButton = D12;

// Variables
bool parkingMode = false;
unsigned long lastBeepTime = 0;
bool buzzerState = false;
int beepInterval = 0;
int beepFrequency = 0;
int distance_cm = 0;

void setup() {
  Serial.begin(9600);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(parkingModeButton, INPUT_PULLUP);

  pixels.begin();
  pixels.show();

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true);
  }

  // Show splash image
  display.clearDisplay();
  display.drawBitmap(0, 0, SMLimage, 128, 64, SSD1306_WHITE); // Draw the SMLimage logo
  display.display();
  delay(2000);

  display.clearDisplay();
  updateOLED("Off");
}

void loop() {
  // Toggle parking mode
  if (digitalRead(parkingModeButton) == LOW) {
    delay(50);
    while (digitalRead(parkingModeButton) == LOW);
    parkingMode = !parkingMode;

    if (parkingMode) {
      Serial.println("Parking Mode ON");
      updateOLED("ON");
    } else {
      Serial.println("Parking Mode OFF");
      updateOLED("OFF");
      noTone(buzzerPin);
      pixels.clear();
      pixels.show();
    }
  }

  if (parkingMode) {
    // Measure distance
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    long duration = pulseIn(echoPin, HIGH);
    distance_cm = duration * 0.0343 / 2;

    Serial.print("Distance: ");
    Serial.print(distance_cm);
    Serial.println(" cm");

    updateOLED(String(distance_cm) + " cm");

    // Buzzer and NeoPixel Logic
    if (distance_cm > 1 && distance_cm <= 15) {
      tone(buzzerPin, 349); // Continuous tone
      beepInterval = 0;
      pixels.fill(pixels.Color(255, 0, 0)); // Red
      pixels.show();
    } else if (distance_cm > 15 && distance_cm <= 30) {
      beepInterval = 300; // Fast beeping
      beepFrequency = 1000;
      pixels.fill(pixels.Color(255, 255, 0)); // Yellow
      pixels.show();
    } else if (distance_cm > 30 && distance_cm <= 60) {
      beepInterval = 1000; // Slow beeping
      beepFrequency = 800;
      pixels.fill(pixels.Color(0, 255, 0)); // Green
      pixels.show();
    } else {
      beepInterval = -1;
      noTone(buzzerPin);
      pixels.clear();
      pixels.show();
    }

    // Beep logic for Green/Yellow zones
    if (beepInterval > 0) {
      unsigned long currentTime = millis();
      if (currentTime - lastBeepTime >= beepInterval) {
        buzzerState = !buzzerState;
        if (buzzerState) {
          tone(buzzerPin, beepFrequency);
        } else {
          noTone(buzzerPin);
        }
        lastBeepTime = currentTime;
      }
    }
  }

  delay(50);
}

void updateOLED(String message) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Parking Assist");

  display.setTextSize(2);
  display.setCursor(0, 25);
  display.print("Mode: ");
  display.println(parkingMode ? "ON" : "OFF");

  display.setCursor(0, 45);
  display.print("Dist: ");
  display.println(message);
  display.display();
}
