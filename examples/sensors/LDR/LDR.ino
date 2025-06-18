const int ledPin = LED_BUILTIN; // Digital pin connected to LED
const int ldrPin = A0; // Analog pin connected to LDR

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int ldrValue = analogRead(ldrPin);
  if (ldrValue < 50) { // Adjust threshold as needed
    digitalWrite(ledPin, HIGH); // Turn LED ON when dark
    Serial.println("LDR is DARK, LED is ON");
  } else {
    digitalWrite(ledPin, LOW); // Turn LED OFF when bright
    Serial.println("LDR is BRIGHT, LED is OFF");
  }
  Serial.println(ldrValue);
  delay(500);
}
