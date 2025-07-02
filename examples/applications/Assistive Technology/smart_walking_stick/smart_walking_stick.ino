// Project: Smart Walking Stick for Visually Challenged People
// Category: Assistive Technology

// Hardware:
// - Arduino EK R4 Minima
// - Ultrasonic Sensor (D4/D7)
// - Neopixel Ring (D5)
// - LDR (A0)
// - Buzzer (D11)

#include <Adafruit_NeoPixel.h>

#ifdef __AVR__
#include <avr/power.h> // Required for old AVR boards for NeoPixel
#endif

// NeoPixel definitions
#define PIXEL_PIN D5      // NeoPixel Ring pin
#define NUMPIXELS 12      // Number of pixels in your ring
Adafruit_NeoPixel pixels(NUMPIXELS, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

const int trigPin = D4;     // Ultrasonic Sensor Trigger pin
const int echoPin = D7;     // Ultrasonic Sensor Echo pin
const int ldrPin = A0;      // LDR connected to Analog Pin A0
const int buzzerPin = D11;  // Buzzer pin

const long obstacleDistance_cm = 30; // Distance to alert for obstacles
const int darkThreshold = 300;       // LDR value for darkness (lower is darker)

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ldrPin, INPUT);
  pinMode(buzzerPin, OUTPUT);

  pixels.begin();
  pixels.show(); // Initialize all pixels to 'off'
}

void loop() {
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
    // Obstacle detected
    digitalWrite(buzzerPin, HIGH); // Buzz
    pixels.fill(pixels.Color(255, 0, 0)); // Red for obstacle
    pixels.show();
    Serial.println("Obstacle detected!");
  } else {
    digitalWrite(buzzerPin, LOW); // No buzz
    // Check LDR for night light
    int ldrValue = analogRead(ldrPin);
    Serial.print("LDR Value: ");
    Serial.println(ldrValue);

    if (ldrValue < darkThreshold) {
      pixels.fill(pixels.Color(0, 0, 255)); // Blue for night light
      pixels.show();
      Serial.println("Darkness detected - Night light ON");
    } else {
      pixels.clear(); // Turn off NeoPixels
      pixels.show();
    }
  }
  delay(100); // Small delay
}