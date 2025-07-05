#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <IRremote.h>

// OLED display settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1

// IR Receiver pin
#define IR_PIN 6

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// IR Remote codes
#define IR_OK     0xBF40FF00
#define IR_UP     0xB946FF00
#define IR_DOWN   0xEA15FF00
#define IR_LEFT   0xBB44FF00
#define IR_RIGHT  0xBC43FF00

// --- Landscape Tetris settings ---
// Playfield: 32 columns (width), 16 rows (height), block size 4x4 pixels
#define BOARD_WIDTH  32
#define BOARD_HEIGHT 16
#define BLOCK_SIZE   4

// No offset needed, playfield fills the screen
#define X_OFFSET 0
#define Y_OFFSET 0

byte board[BOARD_HEIGHT][BOARD_WIDTH];
int curX, curY, curShape, curRot;
unsigned long lastFallTime = 0;
int fallDelay = 350; // ms

const byte tetromino[7][4][4][4] = {
  // I
  {{{0,0,0,0},{1,1,1,1},{0,0,0,0},{0,0,0,0}},
   {{0,0,1,0},{0,0,1,0},{0,0,1,0},{0,0,1,0}},
   {{0,0,0,0},{1,1,1,1},{0,0,0,0},{0,0,0,0}},
   {{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0}}},
  // J
  {{{1,0,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}},
   {{0,1,1,0},{0,1,0,0},{0,1,0,0},{0,0,0,0}},
   {{0,0,0,0},{1,1,1,0},{0,0,1,0},{0,0,0,0}},
   {{0,1,0,0},{0,1,0,0},{1,1,0,0},{0,0,0,0}}},
  // L
  {{{0,0,1,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}},
   {{0,1,0,0},{0,1,0,0},{0,1,1,0},{0,0,0,0}},
   {{0,0,0,0},{1,1,1,0},{1,0,0,0},{0,0,0,0}},
   {{1,1,0,0},{0,1,0,0},{0,1,0,0},{0,0,0,0}}},
  // O
  {{{0,1,1,0},{0,1,1,0},{0,0,0,0},{0,0,0,0}},
   {{0,1,1,0},{0,1,1,0},{0,0,0,0},{0,0,0,0}},
   {{0,1,1,0},{0,1,1,0},{0,0,0,0},{0,0,0,0}},
   {{0,1,1,0},{0,1,1,0},{0,0,0,0},{0,0,0,0}}},
  // S
  {{{0,1,1,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}},
   {{0,1,0,0},{0,1,1,0},{0,0,1,0},{0,0,0,0}},
   {{0,0,0,0},{0,1,1,0},{1,1,0,0},{0,0,0,0}},
   {{1,0,0,0},{1,1,0,0},{0,1,0,0},{0,0,0,0}}},
  // T
  {{{0,1,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}},
   {{0,1,0,0},{0,1,1,0},{0,1,0,0},{0,0,0,0}},
   {{0,0,0,0},{1,1,1,0},{0,1,0,0},{0,0,0,0}},
   {{0,1,0,0},{1,1,0,0},{0,1,0,0},{0,0,0,0}}},
  // Z
  {{{1,1,0,0},{0,1,1,0},{0,0,0,0},{0,0,0,0}},
   {{0,0,1,0},{0,1,1,0},{0,1,0,0},{0,0,0,0}},
   {{0,0,0,0},{1,1,0,0},{0,1,1,0},{0,0,0,0}},
   {{0,1,0,0},{1,1,0,0},{1,0,0,0},{0,0,0,0}}}
};

void spawnTetromino() {
  curShape = random(0, 7);
  curRot = 0;
  curX = BOARD_WIDTH / 2 - 2;
  curY = 0; // Top row
}

// Collision check: blocks can't go past left, right, or bottom border
bool checkCollision(int x, int y, int rot) {
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++)
      if (tetromino[curShape][rot][i][j]) {
        int nx = x + j;
        int ny = y + i;
        // Check left wall, right wall, bottom wall
        if (nx < 0 || nx >= BOARD_WIDTH || ny >= BOARD_HEIGHT)
          return true;
        if (ny >= 0 && board[ny][nx])
          return true;
      }
  return false;
}

void mergeTetromino() {
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++)
      if (tetromino[curShape][curRot][i][j]) {
        int nx = curX + j;
        int ny = curY + i;
        if (ny >= 0 && nx >= 0 && nx < BOARD_WIDTH && ny < BOARD_HEIGHT)
          board[ny][nx] = 1;
      }
}

void clearLines() {
  for (int i = 0; i < BOARD_HEIGHT; i++) {
    bool full = true;
    for (int j = 0; j < BOARD_WIDTH; j++)
      if (!board[i][j]) full = false;
    if (full) {
      for (int k = i; k > 0; k--)
        for (int j = 0; j < BOARD_WIDTH; j++)
          board[k][j] = board[k - 1][j];
      for (int j = 0; j < BOARD_WIDTH; j++)
        board[0][j] = 0;
    }
  }
}

void drawBoard() {
  display.clearDisplay();
  // Draw border outline
  display.drawRect(X_OFFSET, Y_OFFSET, BOARD_WIDTH * BLOCK_SIZE, BOARD_HEIGHT * BLOCK_SIZE, SSD1306_WHITE);

  // Draw grid
  for (int i = 0; i < BOARD_HEIGHT; i++)
    for (int j = 0; j < BOARD_WIDTH; j++)
      if (board[i][j])
        display.fillRect(X_OFFSET + j * BLOCK_SIZE, Y_OFFSET + i * BLOCK_SIZE, BLOCK_SIZE - 1, BLOCK_SIZE - 1, SSD1306_WHITE);

  // Draw current tetromino
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++)
      if (tetromino[curShape][curRot][i][j]) {
        int nx = curX + j;
        int ny = curY + i;
        if (ny >= 0 && nx >= 0 && nx < BOARD_WIDTH && ny < BOARD_HEIGHT)
          display.fillRect(X_OFFSET + nx * BLOCK_SIZE, Y_OFFSET + ny * BLOCK_SIZE, BLOCK_SIZE - 1, BLOCK_SIZE - 1, SSD1306_WHITE);
      }

  display.display();
}

void setup() {
  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(20, 30);
  display.print("TILE TOWER");
  display.display();
  delay(1000);

  IrReceiver.begin(IR_PIN, ENABLE_LED_FEEDBACK);
  randomSeed(analogRead(A0));
  memset(board, 0, sizeof(board));
  spawnTetromino();
  drawBoard();
}

void loop() {
  // Handle IR input
  if (IrReceiver.decode()) {
    uint32_t code = IrReceiver.decodedIRData.decodedRawData;
    if (code == IR_LEFT) {
      if (!checkCollision(curX - 1, curY, curRot)) curX--;
    } else if (code == IR_RIGHT) {
      if (!checkCollision(curX + 1, curY, curRot)) curX++;
    } else if (code == IR_DOWN) {
      if (!checkCollision(curX, curY + 1, curRot)) curY++;
    } else if (code == IR_UP) {
      int nextRot = (curRot + 1) % 4;
      if (!checkCollision(curX, curY, nextRot)) curRot = nextRot;
    } else if (code == IR_OK) {
      while (!checkCollision(curX, curY + 1, curRot)) curY++;
    }
    IrReceiver.resume();
    drawBoard();
  }

  // Gravity (falling)
  if (millis() - lastFallTime > fallDelay) {
    if (!checkCollision(curX, curY + 1, curRot)) {
      curY++;
    } else {
      mergeTetromino();
      clearLines();
      spawnTetromino();
      if (checkCollision(curX, curY, curRot)) {
        // Game over
        display.clearDisplay();
        display.setTextSize(1);
        display.setCursor(30, 30);
        display.print("GAME OVER!");
        display.display();
        delay(1200);
        memset(board, 0, sizeof(board));
        spawnTetromino();
      }
    }
    lastFallTime = millis();
    drawBoard();
  }
}
