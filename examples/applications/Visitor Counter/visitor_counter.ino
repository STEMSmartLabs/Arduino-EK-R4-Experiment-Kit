#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED Definitions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Ultrasonic Sensor Pins
const int trigPin = 4; // D4
const int echoPin = 7; // D7

// Button Pin
const int buttonPin = 12; // D12

// Logic Variables
const long detectionDistance_cm = 50;
long lastDetectionTime = 0;
const long debounceTime = 2000; // 2 seconds

int visitorCount = 0;
bool waitingForButton = false;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buttonPin, INPUT_PULLUP); // Use internal pull-up resistor
  Serial.begin(9600);

  // OLED Init
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (true); // Loop forever if OLED init fails
  }

  display.display();
  delay(2000);
  display.clearDisplay();
  updateOLED("Ready");
}

void loop() {
  // If waiting for button press, check button
  if (waitingForButton) {
    if (digitalRead(buttonPin) == LOW) { // Button pressed
      visitorCount++;
      waitingForButton = false;
      updateOLED("Counted");
      delay(500); // debounce
    }
    return; // Skip ultrasonic check while waiting
  }

  // Trigger the ultrasonic sensor
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

  // If person detected and debounce time passed
  if (distance_cm > 1 && distance_cm <= detectionDistance_cm &&
      (millis() - lastDetectionTime > debounceTime)) {
    lastDetectionTime = millis();
    waitingForButton = true;
    updateOLED("Press button");
  }

  delay(100);
}

void updateOLED(String message) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Visitor Counter");

  // Show live visitor count (add "..." if waiting)
  display.setTextSize(1);
  display.setCursor(0, 20);
  display.print("Count: ");
  display.print(visitorCount);
  if (waitingForButton) display.print("...");
  
  // Show message (e.g., "Press button")
  display.setTextSize(1);
  display.setCursor(0, 50);
  display.println(message);
  display.display();
}
