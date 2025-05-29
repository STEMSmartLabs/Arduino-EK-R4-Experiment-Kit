#include <Servo.h> // Include Servo library

Servo myServo; // Create a Servo object
int pos = 0;   // Variable to store servo position

void setup() {
  myServo.attach(A0); // Attach servo to PIN 9
}

void loop() {
  // Sweep from 0° to 180°
  for (pos = 0; pos <= 180; pos += 1) {
    myServo.write(pos); // Move servo to 'pos'
    delay(15);          // Wait for servo to reach position
  }

  delay(1000); // Pause for one second

  // Sweep back from 180° to 0°
  for (pos = 180; pos >= 0; pos -= 1) {
    myServo.write(pos);
    delay(15);
  }

  delay(1000); // Pause for one second
}
