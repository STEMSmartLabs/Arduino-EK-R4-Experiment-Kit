// Pin for the switch
const int switchPin = 2;

// Variable to hold switch state
int switchState = 0;

void setup() {
  // Start serial communication
  Serial.begin(9600);

  // Set switch pin as input
  pinMode(switchPin, INPUT); // Assumes switch connects to HIGH when ON, LOW when OFF (e.g., with pull-down)
}

void loop() {
  // Read switch state
  switchState = digitalRead(switchPin);

  // Print state to serial monitor
  if (switchState == HIGH) {
    Serial.println("Switch State: ON");
  } else {
    Serial.println("Switch State: OFF");
  }

  // Small delay for readability
  delay(100);
}
