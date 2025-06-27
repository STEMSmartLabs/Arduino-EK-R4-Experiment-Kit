// Project: Automated Door Opening with Ultrasonic Sensor & Servo
// Category: Home Automation

#include <Servo.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Servo setup
Servo myServo;
const int trigPin = D4;
const int echoPin = D7;
const int servoPin = D9;

const long detectionDistance_cm = 50;
const int openAngle = 90;
const int closeAngle = 0;

bool doorOpen = false;

// OLED setup
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Function declarations
void moveServoSmoothly(int fromAngle, int toAngle, int stepDelay = 10);
void updateOLED(String status, long distance);
long getDistance();

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  myServo.attach(servoPin);
  myServo.write(closeAngle);

  Serial.begin(9600);
  Serial.println("Automated Door System Ready.");

  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true);
  }

  display.display();
  delay(2000);
  display.clearDisplay();
  updateOLED("Door Closed", 0);
}

void loop() {
  long distance_cm = getDistance();

  Serial.print("Distance: ");
  Serial.print(distance_cm);
  Serial.println(" cm");

  if (distance_cm > 0 && distance_cm <= detectionDistance_cm) {
    if (!doorOpen) {
      moveServoSmoothly(closeAngle, openAngle);
      doorOpen = true;
      Serial.println("Door Opened");
      updateOLED("Door Open!", distance_cm);
    } else {
      updateOLED("Door Open!", distance_cm); // Still open
    }
  } else {
    if (doorOpen) {
      Serial.println("No object. Attempting to close...");
      updateOLED("Closing...", distance_cm);

      // Start slow close with interruption check
      for (int angle = openAngle; angle >= closeAngle; angle--) {
        myServo.write(angle);
        delay(10);

        // Check again if object appears mid-close
        long recheck = getDistance();
        if (recheck > 0 && recheck <= detectionDistance_cm) {
          Serial.println("Object detected again! Reopening...");
          moveServoSmoothly(angle, openAngle);
          updateOLED("Door Open!", recheck);
          return; // Exit early
        }
      }

      // Fully closed
      doorOpen = false;
      Serial.println("Door Closed");
      updateOLED("Door Closed", distance_cm);
    } else {
      updateOLED("Door Closed", distance_cm); // Already closed
    }
  }

  delay(100);
}

// Distance reader function
long getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 20000); // timeout at 20ms
  if (duration == 0) return 999; // no object
  return duration * 0.0343 / 2;
}

// Smooth servo movement
void moveServoSmoothly(int fromAngle, int toAngle, int stepDelay) {
  if (fromAngle < toAngle) {
    for (int angle = fromAngle; angle <= toAngle; angle++) {
      myServo.write(angle);
      delay(stepDelay);
    }
  } else {
    for (int angle = fromAngle; angle >= toAngle; angle--) {
      myServo.write(angle);
      delay(stepDelay);
    }
  }
}

// OLED update
void updateOLED(String status, long distance) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Auto Door System");

  display.setTextSize(2);
  display.setCursor(0, 20);
  display.println(status);

  display.setTextSize(1);
  display.setCursor(0, 50);
  display.print("Distance: ");
  display.print(distance);
  display.println(" cm");

  display.display();
}
