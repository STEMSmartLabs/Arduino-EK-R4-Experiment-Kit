#include <IRremote.hpp>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "SMLimage.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define IR_PIN 6
#define BUZZER_PIN 11

char grid[3][3];
int cursorX = 0, cursorY = 0;
bool isXTurn = true;
bool vsComputer = false;

const unsigned long UP     = 0xB946FF00;
const unsigned long DOWN   = 0xEA15FF00;
const unsigned long LEFT   = 0xBB44FF00;
const unsigned long RIGHT  = 0xBC43FF00;
const unsigned long SELECT = 0xBF40FF00;

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  IrReceiver.begin(IR_PIN, ENABLE_LED_FEEDBACK);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.drawBitmap(0, 0, SMLimage, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
  display.display();
  delay(2000);

  showMenu();
  resetGrid();
  drawBoard();
}

void loop() {
  if (IrReceiver.decode()) {
    unsigned long code = IrReceiver.decodedIRData.decodedRawData;

    if (code == UP && cursorY > 0) cursorY--;
    else if (code == DOWN && cursorY < 2) cursorY++;
    else if (code == LEFT && cursorX > 0) cursorX--;
    else if (code == RIGHT && cursorX < 2) cursorX++;
    else if (code == SELECT) {
      if (grid[cursorY][cursorX] == ' ') {
        grid[cursorY][cursorX] = isXTurn ? 'X' : 'O';
        tone(BUZZER_PIN, 800, 100);
        drawBoard();

        char winner = checkWinner();
        if (winner != ' ') {
          showWinner(winner);
          delay(2000);
          showMenu();
          resetGrid();
        } else if (isDraw()) {
          showDraw();
          delay(2000);
          showMenu();
          resetGrid();
        } else {
          isXTurn = !isXTurn;
        }
      }
    }

    drawBoard();
    IrReceiver.resume();
  }

  if (vsComputer && !isXTurn) {
    delay(500);
    computerMove();

    drawBoard();         // Show O's move
    delay(1000);         // Pause so user sees O's move

    char winner = checkWinner();
    if (winner != ' ') {
      showWinner(winner);
      delay(2000);
      showMenu();
      resetGrid();
    } else if (isDraw()) {
      showDraw();
      delay(2000);
      showMenu();
      resetGrid();
    } else {
      isXTurn = true;
    }
  }
}

void showMenu() {
  bool selectingFriend = true;

  // Flush any pending IR input before showing menu
  while (IrReceiver.decode()) {
    IrReceiver.resume();
  }

  while (true) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(20, 0);
    display.println("Select Game Mode");
    display.setCursor(20, 20);
    display.println(selectingFriend ? "> Friend" : "  Friend");
    display.setCursor(20, 35);
    display.println(!selectingFriend ? "> Computer" : "  Computer");
    display.display();

    if (IrReceiver.decode()) {
      unsigned long code = IrReceiver.decodedIRData.decodedRawData;

      if (code == UP || code == DOWN) {
        selectingFriend = !selectingFriend;
        tone(BUZZER_PIN, 400, 50);
      } else if (code == SELECT) {
        vsComputer = !selectingFriend;
        tone(BUZZER_PIN, 1000, 100);
        delay(2000);

        // Flush IR buffer after selection
        while (IrReceiver.decode()) {
          IrReceiver.resume();
        }
        break;
      }

      IrReceiver.resume();
    }
  }

  resetGrid();
  drawBoard();
}

void resetGrid() {
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
      grid[i][j] = ' ';
  cursorX = 0;
  cursorY = 0;
  isXTurn = true;
}

void drawBoard() {
  display.clearDisplay();
  display.setTextSize(2);
  for (int y = 0; y < 3; y++) {
    for (int x = 0; x < 3; x++) {
      display.setCursor(x * 40, y * 20);
      if (x == cursorX && y == cursorY)
        display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      else
        display.setTextColor(SSD1306_WHITE);
      display.print(grid[y][x] == ' ' ? '_' : grid[y][x]);
    }
  }
  display.display();
}

char checkWinner() {
  for (int i = 0; i < 3; i++) {
    if (grid[i][0] != ' ' && grid[i][0] == grid[i][1] && grid[i][1] == grid[i][2]) return grid[i][0];
    if (grid[0][i] != ' ' && grid[0][i] == grid[1][i] && grid[1][i] == grid[2][i]) return grid[0][i];
  }
  if (grid[0][0] != ' ' && grid[0][0] == grid[1][1] && grid[1][1] == grid[2][2]) return grid[0][0];
  if (grid[0][2] != ' ' && grid[0][2] == grid[1][1] && grid[1][1] == grid[2][0]) return grid[0][2];
  return ' ';
}

bool isDraw() {
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
      if (grid[i][j] == ' ') return false;
  return checkWinner() == ' ';
}

void showWinner(char winner) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 20);
  display.print(winner);
  display.println(" Wins!");
  display.display();
  tone(BUZZER_PIN, 1000, 300);
}

void showDraw() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor((SCREEN_WIDTH - 48) / 2, 20);
  display.println("Draw!");
  display.display();
  tone(BUZZER_PIN, 600, 300);
}

void computerMove() {
  // Try to win
  for (int y = 0; y < 3; y++) {
    for (int x = 0; x < 3; x++) {
      if (grid[y][x] == ' ') {
        grid[y][x] = 'O';
        if (checkWinner() == 'O') {
          tone(BUZZER_PIN, 700, 100);
          return;
        }
        grid[y][x] = ' ';
      }
    }
  }

  // Block X from winning
  for (int y = 0; y < 3; y++) {
    for (int x = 0; x < 3; x++) {
      if (grid[y][x] == ' ') {
        grid[y][x] = 'X';
        if (checkWinner() == 'X') {
          grid[y][x] = 'O';
          tone(BUZZER_PIN, 700, 100);
          return;
        }
        grid[y][x] = ' ';
      }
    }
  }

  // Otherwise, pick first empty spot
  for (int y = 0; y < 3; y++) {
    for (int x = 0; x < 3; x++) {
      if (grid[y][x] == ' ') {
        grid[y][x] = 'O';
        tone(BUZZER_PIN, 700, 100);
        return;
      }
    }
  }
}
