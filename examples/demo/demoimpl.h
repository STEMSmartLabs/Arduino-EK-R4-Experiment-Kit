// ───── DRAW MENU ─────
void drawMenu(){
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0,0);
  display.print(currentIndex+1);
  display.print("/");
  display.println(MENU_LEN);
  int16_t y=(SCREEN_HEIGHT-16)/2;
  display.setCursor(0,y);
  display.println(menuItems[currentIndex]);

  if(currentIndex==0){
    display.println();
    display.setTextSize(1);
    display.print("Use IR remote");
  }
  display.display();
}


// ───── DRAW BUFFERED DIGITS ─────
void drawBuffered(){
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0,(SCREEN_HEIGHT-16)/2);
  display.print(digitBuffer);
  display.display();
}

// ───── EXIT ON OK ─────
bool demoExitRequested(){
  if (IrReceiver.decode()){
    uint32_t c=IrReceiver.decodedIRData.decodedRawData;
    IrReceiver.resume();
    if (c==IR_KEY_OK){
      return true;
    }
  }
  return false;
}

// ───── SPINNER ─────
void showSpinner(){
  static const char spin[4]={'|','/','-','\\'};
  static uint8_t i=0;
  display.setTextSize(1);
  display.setCursor(SCREEN_WIDTH-6, SCREEN_HEIGHT-8);
  display.write(spin[i++ & 3]);
  display.display();
}

// ───── DEMO FUNCTIONS ─────

void demoServo(){
  Servo s;
  s.attach(SERVO_PIN);

  int pos = 0, dir = 1;
  unsigned long last = millis();

  while (!demoExitRequested()) {
    if (millis() - last > 30) {
      // advance & bounce
      pos += dir * 5;
      if (pos <= 0 || pos >= 180) dir = -dir;
      s.write(pos);

      // display the angle
      display.clearDisplay();
      display.setTextSize(2);
      display.setCursor(0, 0);
      display.println(F("Angle:"));
      display.println();
      display.setTextSize(2);
      display.print(pos);
      display.print(F(" deg"));

      // spinner to show it’s running
      showSpinner();
      display.display();

      last = millis();
    }
  }

  s.detach();
  
}


void demoFan(){
  int v=0, dir=1; unsigned long last=millis();
  while(!demoExitRequested()){
    if (millis()-last>20){
      v+=dir*5;
      if (v<=0||v>=255) dir=-dir;
      analogWrite(FAN_PIN,v);
      display.clearDisplay();
      display.setTextSize(2);
      display.setCursor(0, 0);
      display.println(F("Fan Speed:"));
      display.println();   
      display.print(map(v,0,255,0,100));
      display.println("%");
      showSpinner();
      display.display();
      last=millis();
    }
  }
  analogWrite(FAN_PIN,0);
  
}

void demoUltrasonic(){
  while(!demoExitRequested()){
    digitalWrite(US_TRIG_PIN,LOW); delayMicroseconds(2);
    digitalWrite(US_TRIG_PIN,HIGH); delayMicroseconds(10);
    digitalWrite(US_TRIG_PIN,LOW);
    long d=pulseIn(US_ECHO_PIN,HIGH)*0.034/2;
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0,0); display.println("Distance:");
    display.println();
    display.setTextSize(2);
    display.print(d); display.print("cm");
    showSpinner(); display.display();
    delay(500);
  }
  
}

void demoHumidityTemp() {
  while (!demoExitRequested()) {
    float h = dht.readHumidity(), t = dht.readTemperature();
    display.clearDisplay();
    if (isnan(h)||isnan(t)) {
      display.setTextSize(2);
      display.setCursor(0,16);
      display.print(F("Err"));
      display.setTextSize(1);
    } else {
      display.setTextSize(2);
      display.setCursor(0,16);
      display.print(F("T(C) "));      
      display.println(t);
      display.print(F("H(%) "));
      display.print(h);
      display.setTextSize(1);
    }
    showSpinner();
    delay(1000);
  }
  
}

// returns an RGB color from 0–255 across a rainbow
uint32_t wheel(uint8_t pos) {
  if (pos < 85) {
    return strip.Color(pos * 3, 255 - pos * 3, 0);
  } else if (pos < 170) {
    pos -= 85;
    return strip.Color(255 - pos * 3, 0, pos * 3);
  } else {
    pos -= 170;
    return strip.Color(0, pos * 3, 255 - pos * 3);
  }
}

void demoNeoPixel(){
  uint16_t i = 0;
  uint8_t  hue = 0;
  while (!demoExitRequested()) {
    strip.clear();
    strip.setPixelColor(i, wheel(hue));
    strip.show();

    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0, 0);
    display.println("NeoPixel");
    showSpinner();
    display.display();

    // advance pixel + hue
    i   = (i + 1)   % strip.numPixels();
    hue = (hue + 8) % 256;       // adjust + for faster/slower color cycle

    delay(100);
  }
  strip.clear();
  strip.show();
  
}



void demoLDR(){
  while(!demoExitRequested()){
    int v=analogRead(LDR_PIN);
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0,0); display.println("LDR :");
    display.println();
    display.setTextSize(2);
    display.print(v);
    showSpinner(); display.display();
    delay(500);
  }
  
}

void demoIRSensor(){
  IrReceiver.resume();
  while(!demoExitRequested()){
    if (IrReceiver.decode()){
      uint32_t c=IrReceiver.decodedIRData.decodedRawData;
      IrReceiver.resume();
      if (c!=0xFFFFFFFFUL){
        const char* name;
        switch(c){
          case IR_KEY_0:     name="0"; break;
          case IR_KEY_1:     name="1"; break;
          case IR_KEY_2:     name="2"; break;
          case IR_KEY_3:     name="3"; break;
          case IR_KEY_4:     name="4"; break;
          case IR_KEY_5:     name="5"; break;
          case IR_KEY_6:     name="6"; break;
          case IR_KEY_7:     name="7"; break;
          case IR_KEY_8:     name="8"; break;
          case IR_KEY_9:     name="9"; break;
          case IR_KEY_STAR:  name="*"; break;
          case IR_KEY_HASH:  name="#"; break;
          case IR_KEY_OK:    name="OK"; break;
          case IR_KEY_UP:    name="UP"; break;
          case IR_KEY_DOWN:  name="DN"; break;
          case IR_KEY_LEFT:  name="LT"; break;
          case IR_KEY_RIGHT: name="RT"; break;
          default: {
            static char buf[11];
            sprintf(buf,"0x%08lX",c);
            name = buf;
          }
        }
        display.clearDisplay();
        display.setTextSize(1);
        display.setCursor(0,0); display.println("IR Key:");
        display.setTextSize(2);
        display.setCursor(0,16); display.println(name);
        display.display();
      }
    }
    showSpinner(); delay(50);
  }
  
}

void demoPotentiometer(){
  while(!demoExitRequested()){
    int v=analogRead(POT_PIN), p=map(v,0,1023,0,100);
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0,0); 
    display.println("Potentiometer");
    display.println();
    display.setTextSize(2);
    display.print(p); 
    display.print("%");
    showSpinner(); display.display();
    delay(500);
  }
  
}

void demoBuzzer(){
  while(!demoExitRequested()){
    tone(BUZZER_PIN,440,200);
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0,0); display.println("Buzzer");
    showSpinner(); display.display();
    delay(300);
  }
  noTone(BUZZER_PIN);
  
}

void demoLEDBlink(){
  while(!demoExitRequested()){
    digitalWrite(LED_PIN,HIGH);
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0,0); display.println("LED ON");
    showSpinner(); display.display();
    delay(300);
    digitalWrite(LED_PIN,LOW);
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0,0); display.println("LED OFF");
    showSpinner(); display.display();
    delay(300);
  }
  
}

void demoSwitchState(){
  while(!demoExitRequested()){
    int s=digitalRead(SWITCH_PIN);
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0,0); display.println("Switch:");
    display.setTextSize(2);
    display.println();    
    display.println(s?"ON":"OFF");
    showSpinner(); display.display();
    delay(500);
  }
  
}

void demoPushButton() {
  // ensure any old IR data is cleared
  IrReceiver.resume();

  while (!demoExitRequested()) {
    bool pressed = (digitalRead(BUTTON_PIN) == LOW);

    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0, 0);
    display.println(F("Button"));

    display.setTextSize(2);
    display.setCursor(0, 16);
    display.println();
    display.println(pressed ? F("PRESSED") : F("RELEASED"));

    showSpinner();
    display.display();
    delay(200);
  }
  
}


void demoExt1(){
  while (!demoExitRequested()) {
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0, 0);
    display.println(F("Ext1 Relay"));  
    digitalWrite(EXT1_PIN, 1);
    display.setTextSize(2);
    display.setCursor(0, 16);
    display.println();
    display.println(F("ON"));    
    display.display();    
    delay(2000);
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0, 0);
    display.println(F("Ext1 Relay"));      
    digitalWrite(EXT1_PIN, 0);
    display.setTextSize(2);
    display.setCursor(0, 16);
    display.println();
    display.println(F("OFF"));   
    display.display();
    delay(2000);
  }
}

void demoExt2(){
  while(!demoExitRequested()){
    bool touched = digitalRead(EXT2_PIN) == HIGH;
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0, 0);
    display.println(F("Touch Sens"));
    display.setTextSize(2);
    display.setCursor(0, 16);
    display.println();    
    display.println(touched ? F("Touched") : F("Idle"));
    showSpinner();
    display.display();
    delay(200);
  }
}


void demoExt3(){
  while (!demoExitRequested()) {
    bool motion = digitalRead(EXT3_RX_PIN) == HIGH;
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0, 0);
    display.println(F("PIR Motion"));

    display.setTextSize(2);
    display.setCursor(0, 16);
    display.println();
    display.println(motion ? F("MOTION") : F(""));

    showSpinner();
    display.display();

    delay(200);
  }

  Serial.println("Exit Demo");
}


void demoExt4(){
  
  // Initialize RTC
  if (!rtc.begin()) {
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0, 0);
    display.println(F("RTC failed!"));
    delay(1000);
    return;
  }
  // Uncomment to set RTC to sketch compile time:
   rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  while (!demoExitRequested()) {
    // fetch current time
    DateTime now = rtc.now();

    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println(F("Date       Time"));

    // date as DD/MM/YYYY
    display.setTextSize(2);
    display.setCursor(0, 16);
    char buf[20];
    sprintf(buf, "%02d/%02d/%04d", now.day(), now.month(), now.year());
    display.println(buf);

    // time as HH:MM:SS
    display.setCursor(0, 42);
    sprintf(buf, "%02d:%02d:%02d", now.hour(), now.minute(), now.second());
    display.println(buf);

    showSpinner();
    display.display();
    delay(1000);
  }

  Serial.println("Exit Demo");
}

