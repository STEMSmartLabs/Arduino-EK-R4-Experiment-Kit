#include <Wire.h>
#include <RTClib.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <IRremote.hpp>
#include "SMLimage.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define BUZZER_PIN 11
#define IR_PIN 6

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
RTC_DS3231 rtc;

int mode = 0; // 0: Clock, 1: Set Alarm, 2: Timer
DateTime alarmTime;
unsigned long timerStart = 0;
int timerMinutes = 0;
bool alarmSet = false;
bool timerRunning = false;

void showSMLLogo() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.drawBitmap(0, 0, SMLimage, 128, 64, 1);
  display.display();
  delay(2000);
  display.clearDisplay();
}

void displayClock() {
  DateTime now = rtc.now();
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.print("Time: ");
  display.print(now.hour());
  display.print(":");
  display.print(now.minute());
  display.print(":");
  display.print(now.second());

  if (alarmSet) {
    display.setCursor(0, 16);
    display.print("Alarm: ");
    display.print(alarmTime.hour());
    display.print(":");
    display.print(alarmTime.minute());
  }

  if (timerRunning) {
    int secondsLeft = timerMinutes * 60 - ((millis() - timerStart) / 1000);
    display.setCursor(0, 32);
    display.print("Timer: ");
    display.print(secondsLeft / 60);
    display.print("m ");
    display.print(secondsLeft % 60);
    display.print("s");

    if (secondsLeft <= 0) {
      timerRunning = false;
      tone(BUZZER_PIN, 1000, 1000);
    }
  }

  display.display();
}

void displaySetAlarm() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.println("Set Alarm (IR):");
  display.setTextSize(2);
  display.setCursor(0, 20);
  display.print(alarmTime.hour());
  display.print(":");
  if (alarmTime.minute() < 10) display.print("0");
  display.print(alarmTime.minute());
  display.display();
}

void displaySetTimer() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.println("Set Timer (min):");
  display.setTextSize(2);
  display.setCursor(0, 20);
  display.print(timerMinutes);
  display.print(" min");
  display.display();
}

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  Wire.begin();
  rtc.begin();
  IrReceiver.begin(IR_PIN);
  showSMLLogo();
  alarmTime = rtc.now();
}

void loop() {
  if (IrReceiver.decode()) {
    uint8_t code = IrReceiver.decodedIRData.command;

    if (code == 0x45) { // 1 → Clock
      mode = 0;
    } else if (code == 0x46) { // 2 → Set Alarm
      mode = 1;
    } else if (code == 0x47) { // 3 → Set Timer
      mode = 2;
    }

    if (mode == 1) { // Set Alarm
      if (code == 0x18) { // UP
        alarmTime = alarmTime + TimeSpan(0, 0, 1, 0);
      } else if (code == 0x52) { // DOWN
        alarmTime = alarmTime - TimeSpan(0, 0, 1, 0);
      } else if (code == 0x1C) { // OK
        alarmSet = true;
        mode = 0;
      }
    }

    if (mode == 2) { // Set Timer
      if (code == 0x18) timerMinutes++;
      else if (code == 0x52 && timerMinutes > 0) timerMinutes--;
      else if (code == 0x1C) {
        timerRunning = true;
        timerStart = millis();
        mode = 0;
      }
    }

    IrReceiver.resume();
  }

  if (mode == 0) {
    displayClock();

    DateTime now = rtc.now();
    if (alarmSet && now.hour() == alarmTime.hour() && now.minute() == alarmTime.minute() && now.second() == 0) {
      tone(BUZZER_PIN, 1000, 1000);
    }
  } else if (mode == 1) {
    displaySetAlarm();
  } else if (mode == 2) {
    displaySetTimer();
  }

  delay(200);
}
