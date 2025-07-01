#include <IRremote.hpp>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_NeoPixel.h>

#define OLED_RESET -1
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define BUZZER_PIN 11
#define NEOPIXEL_PIN 5
#define NUM_PIXELS 12

Adafruit_NeoPixel ring(NUM_PIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

const int RECV_PIN = 6;
const unsigned long BUTTON_A = 0xE916FF00; // 1
const unsigned long BUTTON_B = 0xE619FF00; // 2
const unsigned long CORRECT_ANSWER = BUTTON_A;

int score = 0;
bool waitingForAnswer = true;

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  IrReceiver.begin(RECV_PIN, ENABLE_LED_FEEDBACK);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  ring.begin();
  ring.show();
  showQuestion();
}

void loop() {
  if (waitingForAnswer && IrReceiver.decode()) {
    unsigned long code = IrReceiver.decodedIRData.decodedRawData;
    IrReceiver.resume();

    if (code == BUTTON_A || code == BUTTON_B) {
      waitingForAnswer = false;
      if (code == CORRECT_ANSWER) {
        score++;
        showResult("Correct!", 0x00FF00); // Green
        tone(BUZZER_PIN, 1000, 300);
      } else {
        showResult("Wrong!", 0xFF0000); // Red
        tone(BUZZER_PIN, 400, 600);
      }
      delay(2000);
      showQuestion();
      waitingForAnswer = true;
    }
  }
}

void showQuestion() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Quiz Time!");
  display.setCursor(0, 20);
  display.println("A: 2+2=4?");
  display.setCursor(0, 40);
  display.print("Score: ");
  display.print(score);
  display.display();
  ring.clear();
  ring.show();
}

void showResult(String result, uint32_t color) {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(2);
  display.println(result);
  display.setTextSize(1);
  display.setCursor(0, 40);
  display.print("Score: ");
  display.print(score);
  display.display();

  for (int i = 0; i < NUM_PIXELS; i++) {
    ring.setPixelColor(i, color);
  }
  ring.show();
}
