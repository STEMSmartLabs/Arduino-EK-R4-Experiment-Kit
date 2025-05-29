// Define analog pin for trimpot
const int potPin = A1;

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
}

void loop() {
  // Read analog value from trimpot
  int potValue = analogRead(potPin);
  
  // Convert to voltage (assuming 5V reference)
  float voltage = potValue * (5.0 / 1023.0);
  
  // Print value to Serial Monitor
  Serial.print("Analog Value: ");
  Serial.print(potValue);
  Serial.print(" | Voltage: ");
  Serial.println(voltage);
  
  delay(100); // Delay for stability
}
