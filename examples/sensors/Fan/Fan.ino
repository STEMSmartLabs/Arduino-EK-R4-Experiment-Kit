
int fan = 10;         // the PWM pin the FAN is attached to
int speed = 0;  // how bright the FAN is
int fadeAmount = 5;  // how many points to fade the FAN by

void setup() {
  pinMode(fan, OUTPUT);
}

void loop() {
  // set the speed 
  analogWrite(fan, speed);

  // change the speed for next time through the loop:
  speed = speed + fadeAmount;

  // reverse the direction of the fading at the ends of the fade:
  if (speed <= 0 || speed >= 255) {
    fadeAmount = -fadeAmount;
  }
  // wait for 30 milliseconds to see the dimming effect
  delay(30);
}
