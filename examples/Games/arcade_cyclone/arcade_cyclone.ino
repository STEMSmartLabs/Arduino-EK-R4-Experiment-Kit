#include <Adafruit_NeoPixel.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <EEPROM.h>
#include "SMLimage.h"

// --- Hardware Pins ---
#define NEOPIXEL_PIN 5
#define NUM_PIXELS 8
#define BUTTON_PIN 12
#define RESET_SWITCH_PIN A3

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

#define EEPROM_ADDR 0           // High score storage
#define EEPROM_FLAG_ADDR 1
#define EEPROM_VALID_FLAG 42

// --- Objects ---
Adafruit_NeoPixel pixels(NUM_PIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// --- Game State ---
int currentLED = 0;
int lastRedLED = 0;
int greenTargetLED = 0;
int score = 0;
int highScore = 0;
int lives = 3;
bool gameActive = true;

unsigned long lastMoveTime = 0;
const int moveInterval = 150;

int lastSwitchState = HIGH; // Previous state of A3

void setup() {
  Serial.begin(9600);

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(RESET_SWITCH_PIN, INPUT_PULLUP);

  // Check for EEPROM reset at startup
  if (digitalRead(RESET_SWITCH_PIN) == LOW) {
    EEPROM.write(EEPROM_ADDR, 0);
    EEPROM.write(EEPROM_FLAG_ADDR, 0);
    Serial.println("EEPROM cleared via switch at startup.");
    delay(1000);
  }

  // Init NeoPixel
  pixels.begin();
  pixels.clear();
  pixels.show();

  // Init OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (true); // Stop if OLED fails
  }

  // Show logo
  display.clearDisplay();
  display.drawBitmap(0, 0, SMLimage, 128, 64, SSD1306_WHITE);
  display.display();
  delay(2000);
  display.clearDisplay();

  // Load high score from EEPROM
  if (EEPROM.read(EEPROM_FLAG_ADDR) == EEPROM_VALID_FLAG) {
    highScore = EEPROM.read(EEPROM_ADDR);
  } else {
    highScore = 0;
    EEPROM.write(EEPROM_ADDR, 0);
    EEPROM.write(EEPROM_FLAG_ADDR, EEPROM_VALID_FLAG);
  }

  // Initialize last switch state
  lastSwitchState = digitalRead(RESET_SWITCH_PIN);

  startNewGame();
}

void loop() {
  // --- Game Loop ---
  if (!gameActive) {
    delay(2000);
    startNewGame();
  }

  if (millis() - lastMoveTime >= moveInterval) {
    lastMoveTime = millis();
    moveRedCursor();
  }

  if (digitalRead(BUTTON_PIN) == LOW) {
    handlePress();
    delay(300); // Debounce
  }

  // --- EEPROM Reset Monitoring ---
  int currentSwitchState = digitalRead(RESET_SWITCH_PIN);
  if (currentSwitchState != lastSwitchState) {
    lastSwitchState = currentSwitchState;
    resetHighScore();
  }
}

void startNewGame() {
  score = 0;
  lives = 3;
  gameActive = true;
  greenTargetLED = random(NUM_PIXELS);
  showGameStatus("New Game!");
}

void moveRedCursor() {
  pixels.clear();
  pixels.setPixelColor(greenTargetLED, pixels.Color(0, 255, 0)); // Green
  pixels.setPixelColor(currentLED, pixels.Color(255, 0, 0));     // Red
  pixels.show();

  lastRedLED = currentLED;
  currentLED = (currentLED + 1) % NUM_PIXELS;
}

void handlePress() {
  if (lastRedLED == greenTargetLED) {
    score++;

    if (score > highScore) {
      highScore = score;
      EEPROM.write(EEPROM_ADDR, highScore);
    }

    showGameStatus("Hit!");
  } else {
    lives--;
    showGameStatus("Miss!");
    if (lives == 0) {
      gameOver();
      return;
    }
  }

  greenTargetLED = random(NUM_PIXELS);
}

void gameOver() {
  gameActive = false;
  showGameStatus("Game Over!");
}

void showGameStatus(String status) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);
  display.println("Arcade Cyclone");

  display.setCursor(0, 20);
  display.println(status);

  display.setCursor(0, 40);
  display.print("Score: ");
  display.print(score);

  display.setCursor(64, 40);
  display.print("High: ");
  display.print(highScore);

  display.setCursor(0, 55);
  display.print("Lives: ");
  display.print(lives);

  display.display();
}

void resetHighScore() {
  EEPROM.write(EEPROM_ADDR, 0);
  EEPROM.write(EEPROM_FLAG_ADDR, 0);
  highScore = 0;
  showGameStatus("High Score Reset");
  Serial.println("EEPROM cleared mid-game via switch toggle.");
  delay(1000);
}
