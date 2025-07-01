#define BUZZER_PIN 11
#define POT_PIN A1

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
  int potValue = analogRead(POT_PIN);
  int frequency = map(potValue, 0, 1023, 100, 2000); // Map to audible range
  tone(BUZZER_PIN, frequency);
  delay(10);
}
