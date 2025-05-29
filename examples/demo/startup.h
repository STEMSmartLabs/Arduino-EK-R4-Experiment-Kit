const float   VOLUME     = 0.1;   // 30% duty-cycle → quieter buzzer

//int length = 149; // the number of notes 
char notes[] = "cdeeeeeeeeedef eeedddbdc";// ccgggggggggaf";// fffffeddf eedeedeggaff eeeeeddbdc cdeeeedf efgggfedfe feeeddbdc ccggggeggggga fffffedfe egc bab aga ccddeedef ";
int length=strlen(notes);
int beats[] = { 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1,
                2, 1, 1, 2, 1, 1, 1, 1, 1, 1,
                2, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1,
                2, 1, 1, 2, 1, 1, 1, 1, 1, 1,
                2, 1, 1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1,
                2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1,
                1, 1, 1, 1, 2, 2, 2, 1, 1,
                1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1,
                2, 2, 1, 1, 1, 1, 1, 1, 1, 1,
                1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 1,
                2, 1, 1, 2, 1, 1, 1, 1, 1, 1,
                1, 1, 4, 1, 1, 1, 4, 1, 1, 1, 4, 1,
                1, 1, 1, 1, 1, 1, 1, 1, 4, 1 };
int tempo    = 200; // set tempo

// playTone now respects VOLUME
void playTone(int halfPeriod, int duration) {
  // full period = halfPeriod * 2
  unsigned int period   = halfPeriod * 2;
  unsigned int highTime = period * VOLUME;
  unsigned int lowTime  = period - highTime;
  
  long endMicros = micros() + duration * 1000L;
  while (micros() < endMicros) {
    digitalWrite(BUZZER_PIN, HIGH);
    delayMicroseconds(highTime);
    digitalWrite(BUZZER_PIN, LOW);
    delayMicroseconds(lowTime);
  }
}

void playNote(char note, int duration) {
  char names[] = { 'c','d','e','f','g','a','b','C' };
  int  tones[] = { 956,851,758,716,638,568,1014,478 };
  for (int i = 0; i < 8; i++) {
    if (names[i] == note) {
      playTone(tones[i], duration);
      return;
    }
  }
  // if no match, just rest
  delay(duration);
}

void playNationalAnthem() {
  for (int i = 0; i < length; i++) {
    if (notes[i] == ' ') {
      delay(beats[i] * tempo);
    } else {
      playNote(notes[i], beats[i] * tempo);
    }
    delay(tempo / 2);  // gap between notes
    if(demoExitRequested() || digitalRead(BUTTON_PIN)==LOW) break;
  }
}

void drawFlag() {
  strip.setBrightness(50);
  strip.setPixelColor(0, COLOR_GREEN);
  strip.setPixelColor(1, COLOR_GREEN);
  strip.setPixelColor(2, COLOR_WHITE);
  strip.setPixelColor(3, COLOR_SAFFRON);
  strip.setPixelColor(4, COLOR_SAFFRON);
  strip.setPixelColor(5, COLOR_SAFFRON);
  strip.setPixelColor(6, COLOR_WHITE);
  strip.setPixelColor(7, COLOR_GREEN);
  strip.show();
}


void showStartup() {
  display.clearDisplay();
  
  // Title: big
  display.setTextSize(2);
  display.setCursor(0, 10);
  display.println(F("Arduino EK"));
  display.setCursor(0, 30);
  display.println(F("R4 Demo"));
  
  // Subtitle: small
  display.setTextSize(1);
  display.setCursor(0, 52);
  display.println(F("STEM Smart Labs"));
  
  display.display();
  //delay(3000);        // show for 3 seconds
  display.clearDisplay();
}