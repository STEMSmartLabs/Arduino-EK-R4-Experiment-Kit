#include <IRremote.hpp>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET -1
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define IR_PIN 6
#define BUZZER_PIN 11

char grid[3][3];
int cursorX = 0, cursorY = 0;
bool isXTurn = true;

const unsigned long UP = 0xB946FF00;
const unsigned long DOWN = 0xEA15FF00;
const unsigned long LEFT = 0xBB44FF00;
const unsigned long RIGHT = 0xBC43FF00;
const unsigned long SELECT = 0xBF40FF00;

void setup() {
  IrReceiver.begin(IR_PIN, ENABLE_LED_FEEDBACK);
  pinMode(BUZZER_PIN, OUTPUT);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
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
        isXTurn = !isXTurn;
        tone(BUZZER_PIN, 800, 100);
        char winner = checkWinner();
        if (winner != ' ') {
          showWinner(winner);
          delay(2000);
          resetGrid();
        }
      }
    }

    drawBoard();
    IrReceiver.resume();
  }
}

void resetGrid() {
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
      grid[i][j] = ' ';
}

void drawBoard() {
  display.clearDisplay();
  display.setTextSize(2);
  for (int y = 0; y < 3; y++) {
    for (int x = 0; x < 3; x++) {
      display.setCursor(x * 40, y * 20);
      if (x == cursorX && y == cursorY) {
        display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      } else {
        display.setTextColor(SSD1306_WHITE);
      }
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

void showWinner(char winner) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(10, 20);
  display.print(winner);
  display.println(" Wins!");
  display.display();
  tone(BUZZER_PIN, 1000, 300);
}
