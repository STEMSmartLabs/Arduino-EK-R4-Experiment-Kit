#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <IRremote.hpp>
#include <Servo.h>
#include <Adafruit_NeoPixel.h>
#include <DHT.h>
#include "RTClib.h"
#include "demoheader.h"
#include "demoimpl.h"
#include "startup.h"



// ───── SETUP ─────
void setup(){
  Serial.begin(9600);
  Wire.begin();

  // OLED
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  // IRremote
  IrReceiver.begin(IR_PIN, DISABLE_LED_FEEDBACK);

  // sensors/init
  pinMode(FAN_PIN, OUTPUT);
  pinMode(US_TRIG_PIN, OUTPUT);
  pinMode(US_ECHO_PIN, INPUT);
  dht.begin();
  strip.begin(); strip.show();
  pinMode(LDR_PIN, INPUT);
  pinMode(POT_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(SWITCH_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(EXT1_PIN,OUTPUT);
  pinMode(EXT2_PIN,INPUT);
  pinMode(EXT3_RX_PIN,INPUT);

  drawFlag();
  showStartup();
  playNationalAnthem();

  Serial.println("Arduino EK R4 Demo");
  Serial.println("STEM Smart Labs");
  Serial.println("Use IR remote to navigate Menu");
  drawMenu();
}



// ───── MAIN LOOP ─────
void loop(){
  if (IrReceiver.decode()){
    uint32_t code = IrReceiver.decodedIRData.decodedRawData;
    IrReceiver.resume();
    if (code == 0xFFFFFFFFUL) return; // skip repeat

    // numeric entry
    int d = -1;
    if      (code==IR_KEY_0) d=0;
    else if (code==IR_KEY_1) d=1;
    else if (code==IR_KEY_2) d=2;
    else if (code==IR_KEY_3) d=3;
    else if (code==IR_KEY_4) d=4;
    else if (code==IR_KEY_5) d=5;
    else if (code==IR_KEY_6) d=6;
    else if (code==IR_KEY_7) d=7;
    else if (code==IR_KEY_8) d=8;
    else if (code==IR_KEY_9) d=9;
    if (d>=0){
      if (millis()-lastDigitTime > DIGIT_TIMEOUT) digitBuffer=0;
      int tmp = digitBuffer*10 + d;
      if (tmp>=1 && tmp<=MENU_LEN){
        digitBuffer=tmp;
        lastDigitTime=millis();
        drawBuffered();
      }
      return;
    }

    // nav keys
    if (code==IR_KEY_UP||code==IR_KEY_LEFT){
      digitBuffer=0;
      currentIndex=(currentIndex+MENU_LEN-1)%MENU_LEN;
      drawMenu();
      return;
    }
    if (code==IR_KEY_DOWN||code==IR_KEY_RIGHT){
      digitBuffer=0;
      currentIndex=(currentIndex+1)%MENU_LEN;
      drawMenu();
      return;
    }

    // OK: jump or select
    if (code==IR_KEY_OK){
      if (digitBuffer>0){
        currentIndex=digitBuffer-1;
        digitBuffer=0;
      }
      launchDemo(currentIndex);
      drawMenu();
    }
  }
}

// ───── LAUNCH DEMO ─────
void launchDemo(int idx){
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("Running ");
  display.println(menuItems[idx]);
  display.display();
  delay(300);

  // dispatch
  switch(idx){
    case 0: demoServo(); break;
    case 1: demoNeoPixel(); break; 
    case 2: demoFan(); break; 
    case 3: demoHumidityTemp(); break;
    case 4: demoLEDBlink(); break;
    case 5: demoLDR(); break;
    case 6: demoUltrasonic(); break;
    case 7: demoBuzzer(); break;
    case 8: demoPushButton(); break;
    case 9: demoPotentiometer(); break;
    case 10: demoSwitchState(); break;
    case 11: demoExt1(); break;
    case 12: demoExt2(); break;
    case 13: demoExt3(); break;
    case 14: demoExt4(); break;
  }

  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0,0);
  display.println("Demo ended");
  display.display();
  delay(500);
}

