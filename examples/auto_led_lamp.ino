const int ldrPin = A0;      // LDR connected to Analog Pin A0
const int lightPin = D3;    // LED connected to Digital Pin D3 (PWM capable)


const int minLDRValue = 50; // LDR value for "completely dark" 
const int maxLDRValue = 800; // LDR value for "bright" 

void setup() {
  pinMode(lightPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int ldrValue = analogRead(ldrPin); // Read LDR value (0-1023)
  Serial.print("LDR Value: ");
  Serial.println(ldrValue);

  
  int brightness = map(ldrValue, minLDRValue, maxLDRValue, 255, 0);

 
  brightness = constrain(brightness, 0, 255);

  analogWrite(lightPin, brightness);

  Serial.print("LED Brightness: ");
  Serial.println(brightness);

  delay(100); 
}