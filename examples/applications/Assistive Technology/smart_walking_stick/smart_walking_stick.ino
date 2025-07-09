// Project: Smart Walking Stick for Visually Challenged People
// Category: Assistive Technology

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_NeoPixel.h>
#include "SMLimage.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define PIXEL_PIN D5
#define NUMPIXELS 12
Adafruit_NeoPixel pixels(NUMPIXELS, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

#define trigPin D4
#define echoPin D7
#define ldrPin A0
#define buzzerPin D11
#define PIXEL_SWITCH_PIN A3  // Switch to enable/disable NeoPixel

const long obstacleDistance_cm = 30;
const int darkThreshold = 300;

bool pixelEnabled = true;

void showSMLLogo() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.drawBitmap(0, 0, SMLimage, 128, 64, 1);
  display.display();
  delay(2000);
  display.clearDisplay();
}

void setup() {
  Serial.begin(9600);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ldrPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(PIXEL_SWITCH_PIN, INPUT_PULLUP); // Switch with pull-up

  pixels.begin();
  pixels.show();

  showSMLLogo();
}

void loop() {
  // Read switch status (LOW = ON, HIGH = OFF)
  pixelEnabled = (digitalRead(PIXEL_SWITCH_PIN) == LOW);

  // Ultrasonic obstacle detection
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  long distance_cm = duration * 0.0343 / 2;

  Serial.print("Distance: ");
  Serial.print(distance_cm);
  Serial.println(" cm");

  if (distance_cm > 1 && distance_cm <= obstacleDistance_cm) {
    // Buzzer beep 3 times
    for (int i = 0; i < 3; i++) {
      tone(buzzerPin, 1000);   // 1000 Hz beep
      delay(150);
      noTone(buzzerPin);
      delay(100);
    }

    if (pixelEnabled) {
      pixels.fill(pixels.Color(255, 0, 0)); // Red
      pixels.show();
    } else {
      pixels.clear();
      pixels.show();
    }

    Serial.println("Obstacle detected!");
  } else {
    noTone(buzzerPin);  // Ensure buzzer is off

    // Read LDR
    int ldrValue = analogRead(ldrPin);
    Serial.print("LDR Value: ");
    Serial.println(ldrValue);

    if (pixelEnabled && ldrValue < darkThreshold) {
      pixels.fill(pixels.Color(0, 0, 255)); // Blue
      pixels.show();
      Serial.println("Darkness detected - Night light ON");
    } else {
      pixels.clear();
      pixels.show();
    }
  }

  delay(100);
}
