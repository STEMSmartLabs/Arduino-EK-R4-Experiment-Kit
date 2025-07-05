/* Tiny Tetris by Anthony Russell 17-09-2016 */
/* V0.91 modified for IR Remote control, Adafruit OLED Library, and SMLimage startup */

// To do:
// High score functionality (using EEPROM as discussed, but not implemented in this version).
// Decent random number generator.
// Create a proper system for rendering numbers and letters (current score display is a bit hacky).
// Tidy up code and optimize for memory, sort out the globals and types.
// Create defines for all the magic numbers but they are useful for now.

#include <Wire.h>
#include <IRremote.hpp> // Using the latest IRremote.hpp library
#include <Adafruit_GFX.h> // Core graphics library
#include <Adafruit_SSD1306.h> // Library for SSD1306 OLED displays
#include "SMLimage.h" // Custom image for startup logo

// Define the pin for the IR receiver
const int RECV_PIN = 6; // Using Digital Pin 6 for IR receiver as per your connections

// Create an IR receiver object
// Note: With IRremote.hpp, you use IrReceiver.begin() and IrReceiver.decode()
// You don't create an IRrecv object directly anymore.

// Define IR codes for your remote's buttons
// IMPORTANT: These are placeholder codes (common NEC protocol from your password lock example).
// You MUST replace these with the actual codes from YOUR remote.
// Use the IR receiver sketch provided in the instructions to find your codes.
#define IR_LEFT   0xFFA25D // Example: NEC Left Arrow
#define IR_RIGHT  0xFFC23D // Example: NEC Right Arrow
#define IR_DOWN   0xFFA857 // Example: NEC Down Arrow
#define IR_ROTATE 0xFF629D // Example: NEC Up Arrow (using Up for Rotate)


// OLED display settings for Adafruit_SSD1306
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1    // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


// The pieces, now stored in program memory (PROGMEM)
// This saves SRAM, which is very limited on Arduino boards.
const bool BlockI[4][4] PROGMEM = { { 0, 1, 0, 0 }, { 0, 1, 0, 0 }, { 0, 1, 0, 0 }, { 0, 1, 0, 0 }, };
const bool BlockJ[4][4] PROGMEM = { { 0, 1, 0, 0 }, { 0, 1, 0, 0 }, { 1, 1, 0, 0 }, { 0, 0, 0, 0 }, };
const bool BlockL[4][4] PROGMEM = { { 0, 1, 0, 0 }, { 0, 1, 0, 0 }, { 0, 1, 1, 0 }, { 0, 0, 0, 0 }, };
const bool BlockO[4][4] PROGMEM = { { 0, 0, 0, 0 }, { 0, 1, 1, 0 }, { 0, 1, 1, 0 }, { 0, 0, 0, 0 }, };
const bool BlockS[4][4] PROGMEM = { { 0, 0, 0, 0 }, { 0, 1, 1, 0 }, { 1, 1, 0, 0 }, { 0, 0, 0, 0 }, };
const bool BlockT[4][4] PROGMEM = { { 0, 0, 0, 0 }, { 1, 1, 1, 0 }, { 0, 1, 0, 0 }, { 0, 0, 0, 0 }, };
const bool BlockZ[4][4] PROGMEM = { { 0, 0, 0, 0 }, { 1, 1, 0, 0 }, { 0, 1, 1, 0 }, { 0, 0, 0, 0 }, };


// The numbers for score, stored in program memory (PROGMEM)
const byte NumberFont[10][8] PROGMEM = {
  { 0x00, 0x1c, 0x22, 0x26, 0x2a, 0x32, 0x22, 0x1c }, // 0
  { 0x00, 0x1c, 0x08, 0x08, 0x08, 0x08, 0x0c, 0x08 }, // 1
  { 0x00, 0x3e, 0x02, 0x04, 0x18, 0x20, 0x22, 0x1c }, // 2
  { 0x00, 0x1c, 0x22, 0x20, 0x18, 0x20, 0x22, 0x1c }, // 3
  { 0x00, 0x10, 0x10, 0x3e, 0x12, 0x14, 0x18, 0x10 }, // 4
  { 0x00, 0x1c, 0x22, 0x20, 0x20, 0x1e, 0x02, 0x3e }, // 5
  { 0x00, 0x1c, 0x22, 0x22, 0x1e, 0x02, 0x04, 0x18 }, // 6
  { 0x00, 0x04, 0x04, 0x04, 0x08, 0x10, 0x20, 0x3e }, // 7
  { 0x00, 0x1c, 0x22, 0x22, 0x1c, 0x22, 0x22, 0x1c }, // 8
  { 0x00, 0x0c, 0x10, 0x20, 0x3c, 0x22, 0x22, 0x1c }  // 9
};

// struct for pieces
struct PieceSpace {
  byte umBlock[4][4];
  char Row;
  char Coloum;
};

// Globals
// Note: pageArray, scoreDisplayBuffer, nextBlockBuffer, optomizePageArray, blockColoum
// are specific to the old direct OLED control. They are largely obsolete with Adafruit_SSD1306.
// However, to minimize changes and potential bugs, I will keep them but their usage will change.
byte pageArray[8] = { 0 }; // Kept for minimal refactor, but not used for direct OLED data
byte scoreDisplayBuffer[8][6] = { { 0 }, { 0 } }; // Kept for minimal refactor
byte nextBlockBuffer[8][2] = { { 0 }, { 0 } }; // Kept for minimal refactor
bool optomizePageArray[8] = { 0 }; // Kept for minimal refactor
byte blockColoum[10] = { 0 }; // Kept for minimal refactor

byte tetrisScreen[14][25] = { { 1 } , { 1 } }; // Game board: 14 columns, 25 rows (including borders)
PieceSpace currentPiece = { 0 };
PieceSpace oldPiece = { 0 };
byte nextPiece = 0;
bool gameOver = false;
unsigned long moveTime = 0;
int pageStart = 0; // Not directly used by Adafruit_SSD1306
int pageEnd = 0;   // Not directly used by Adafruit_SSD1306

int score = 0;
int acceleration = 0;
int level = 0;
int levellineCount = 0;
int dropDelay = 1000; // Initial delay for piece dropping (milliseconds)

// No custom OLEDCommand/OLEDData functions needed with Adafruit_SSD1306

void setup() {
  Serial.begin(9600);
  while (!Serial) { ; } // Wait for Serial Monitor to open

  Wire.begin(); // Initialize I2C communication

  // Initialize OLED display with Adafruit_SSD1306
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x64 OLED
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  // Display startup image
  display.clearDisplay();
  display.drawBitmap(0, 0, SMLimage, 128, 64, SSD1306_WHITE); // Draw the SMLimage logo
  display.display();
  delay(2000); // Show logo for 2 seconds

  // Clear display after logo
  display.clearDisplay();
  display.display();

  // Corrected: Using A2 for randomSeed as it's likely an unused analog pin on R4 Minima
  randomSeed(analogRead(A2));

  // Initialize IR receiver
  IrReceiver.begin(RECV_PIN, ENABLE_LED_FEEDBACK); // Start the IR receiver

  // Simple startup LED blink (using D13, as per original code)
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  delay(100);
  digitalWrite(13, LOW);
  delay(200);
  digitalWrite(13, HIGH);
  delay(50);
  digitalWrite(13, LOW);
}

// Fills the entire tetrisScreen array (including borders)
void fillTetrisArray(byte value) {
  for (char r = 0; r < 25; r++) { // Iterate through all rows (0 to 24)
    for (char c = 0; c < 14; c++) { // Iterate through all columns (0 to 13)
      tetrisScreen[c][r] = value;
    }
  }
  // Clear the bottom 3 rows (these are usually off-screen for collision detection)
  for (char r = 21; r < 24; r++) {
    for (char c = 0; c < 14; c++) {
      tetrisScreen[c][r] = 0;
    }
  }
}

// Fills only the playable area of the Tetris screen
void fillTetrisScreen(byte value) {
  for (int r = 1; r < 21; r++) { // Rows 1 to 20 (playable area)
    for (int c = 2; c < 12; c++) { // Columns 2 to 11 (playable area)
      tetrisScreen[c][r] = value;
    }
  }
}

// Draws the Tetris game board to the OLED display using Adafruit_SSD1306
void drawTetrisScreen() {
  display.clearDisplay(); // Clear the entire display for redraw

  // Draw borders (now using Adafruit GFX primitives)
  drawSides();
  drawBottom();

  // Iterate through playable rows and columns of the game board
  // Map game coordinates to OLED coordinates (game rows -> OLED X, game columns -> OLED Y)
  // Each block is 6x6 pixels
  for (byte gameRow = 1; gameRow < 21; gameRow++) { // Game rows 1 to 20
    for (byte gameCol = 2; gameCol < 12; gameCol++) { // Game columns 2 to 11
      byte blockType = tetrisScreen[gameCol][gameRow];

      // Calculate OLED pixel coordinates for the top-left corner of the block
      int16_t oledX = (gameRow - 1) * 6; // Game row 1 maps to OLED X=0
      int16_t oledY = (gameCol - 2) * 6; // Game col 2 maps to OLED Y=0

      if (blockType == 1 || blockType == 2) { // Landed block or current piece
        display.fillRect(oledX, oledY, 6, 6, SSD1306_WHITE);
      } else if (blockType == 3) { // Block to be cleared (draw black)
        display.fillRect(oledX, oledY, 6, 6, SSD1306_BLACK);
        tetrisScreen[gameCol][gameRow] = 0; // Clear the '3' marker after drawing black
      }
    }
  }
  display.display(); // Push changes to the OLED
}

// This function is no longer needed as drawTetrisScreen handles all drawing
// and the old bit-packing logic is replaced by Adafruit GFX's fillRect.
// Keeping it as a stub to avoid breaking calls, but it does nothing.
void drawTetrisLine(byte x) {
  // This function is obsolete with Adafruit_SSD1306
  // All drawing is now handled by drawTetrisScreen which clears and redraws
  // affected parts using higher-level GFX primitives.
}

// Loads a new piece into currentPiece struct from PROGMEM
void loadPiece(byte pieceNumber, byte row, byte coloum, bool loadScreen) {
  const bool (*blockPtr)[4]; // Pointer to a 4x4 array of bools
  switch (pieceNumber) {
    case 1: blockPtr = &BlockI[0]; break;
    case 2: blockPtr = &BlockJ[0]; break;
    case 3: blockPtr = &BlockL[0]; break;
    case 4: blockPtr = &BlockO[0]; break;
    case 5: blockPtr = &BlockS[0]; break;
    case 6: blockPtr = &BlockT[0]; break;
    case 7: blockPtr = &BlockZ[0]; break;
    default: return; // Should not happen
  }

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      currentPiece.umBlock[i][j] = pgm_read_byte(&blockPtr[i][j]);
    }
  }

  currentPiece.Row = row;
  currentPiece.Coloum = coloum;

  if (loadScreen) {
    oldPiece = currentPiece; // Store current piece as old for later deletion
    for (char c = 0; c < 4; c++) { // Iterate through piece's local columns
      for (char r = 0; r < 4; r++) { // Iterate through piece's local rows
        if (currentPiece.umBlock[c][r]) { // If this part of the piece exists
          // Map local piece coordinates to global tetrisScreen coordinates
          tetrisScreen[currentPiece.Coloum + c][currentPiece.Row + r] = 2; // Mark as current piece block
        }
      }
    }
  }
}

// Draws the current piece by marking its old position for deletion (3)
// and its new position for drawing (2) in the tetrisScreen array.
// drawTetrisScreen() will then handle the actual OLED rendering.
void drawPiece() {
  // Delete old blocks first by marking them as 3
  for (char c = 0; c < 4; c++) {
    for (char r = 0; r < 4; r++) {
      if (oldPiece.umBlock[c][r]) {
        tetrisScreen[oldPiece.Coloum + c][oldPiece.Row + r] = 3;
      }
    }
  }

  // Draw new blocks by marking them as 2
  for (char c = 0; c < 4; c++) {
    for (char r = 0; r < 4; r++) {
      if (currentPiece.umBlock[c][r]) {
        tetrisScreen[currentPiece.Coloum + c][currentPiece.Row + r] = 2;
      }
    }
  }
}

// Marks the current piece as landed (1) in the tetrisScreen array
void drawLandedPiece() {
  for (char c = 0; c < 4; c++) {
    for (char r = 0; r < 4; r++) {
      if (currentPiece.umBlock[c][r]) {
        tetrisScreen[currentPiece.Coloum + c][currentPiece.Row + r] = 1; // Mark as landed block
      }
    }
  }
  processCompletedLines();
}

// Rotates the current piece 90 degrees clockwise
void RotatePiece() {
  byte umFig[4][4] = { 0 };

  // Save current piece as oldPiece before rotation
  oldPiece = currentPiece;

  // Perform rotation: umFig[new_col][new_row] = currentPiece.umBlock[old_col][old_row]
  // (4 - old_row - 1) maps row to column for 90-degree rotation
  for (byte i = 0; i < 4; ++i) {
    for (byte j = 0; j < 4; ++j) {
      umFig[j][i] = currentPiece.umBlock[4 - i - 1][j];
    }
  }

  // Copy rotated block back to currentPiece
  memcpy(currentPiece.umBlock, umFig, 16);

  // Check for collision after rotation. If collision, revert to oldPiece.
  if (checkColloision()) {
    currentPiece = oldPiece;
  }
}

// Moves the current piece down by one row. Returns true if the piece landed.
bool movePieceDown() {
  bool pieceLanded = false;

  oldPiece = currentPiece; // Save current position as old

  currentPiece.Row = currentPiece.Row - 1; // Attempt to move down

  // Check for collision after moving down
  if (checkColloision()) {
    // If collision, it means the piece has landed
    currentPiece = oldPiece; // Revert to the position before collision
    drawLandedPiece(); // Mark the piece as landed on the board
    pieceLanded = true;
  }

  if (pieceLanded) {
    // If a piece landed, load a new piece
    loadPiece(random(1, 8), 19, 4, false); // Try to load at top-center (row 19 is top of screen)
    acceleration = 0; // Reset acceleration for new piece

    // Check if the new piece immediately collides (game over condition)
    if (checkColloision()) {
      gameOver = true;
    } else {
      loadPiece(nextPiece, 19, 4, true); // Actually place the new piece on screen
      acceleration = 0; // Reset acceleration for new piece
    }

    nextPiece = random(1, 8); // Generate next piece (1-7)
    setNextBlock(nextPiece); // Display the next piece
  }
  return pieceLanded;
}

// Moves the current piece left by one column
void movePieceLeft() {
  oldPiece = currentPiece; // Save current position as old
  currentPiece.Coloum = currentPiece.Coloum - 1; // Attempt to move left

  // Check for collision. If collision, revert.
  if (checkColloision()) {
    currentPiece = oldPiece;
  }
}

// Moves the current piece right by one column
void movePieceRight() {
  oldPiece = currentPiece; // Save current position as old
  currentPiece.Coloum = currentPiece.Coloum + 1; // Attempt to move right

  // Check for collision. If collision, revert.
  if (checkColloision()) {
    currentPiece = oldPiece;
  }
}

// Checks if the currentPiece position collides with landed blocks (1) or borders (1)
bool checkColloision() {
  for (char c = 0; c < 4; c++) { // Iterate through piece's local columns (0-3)
    for (char r = 0; r < 4; r++) { // Iterate through piece's local rows (0-3)
      // Only check if the piece itself has a block at this relative position
      if (currentPiece.umBlock[c][r]) {
        // Calculate absolute coordinates on the tetrisScreen
        char absCol = currentPiece.Coloum + c;
        char absRow = currentPiece.Row + r;

        // Check if the absolute coordinates are within the valid bounds of tetrisScreen
        // (columns 0-13, rows 0-24)
        // and if the target cell on tetrisScreen is already occupied by a landed block (1)
        // or is part of the border (1, initially set by fillTetrisArray)
        if (absCol < 0 || absCol >= 14 || absRow < 0 || absRow >= 25 || tetrisScreen[absCol][absRow] == 1) {
          return true; // Collision detected
        }
      }
    }
  }
  return false; // No collision
}

// Processes completed lines, clears them, moves blocks down, and updates score/level
void processCompletedLines() {
  char linesProcessed = 0;
  char clearedLines = 0;
  int amountScored = 0;

  // Start checking from the bottom of the current piece's impact area upwards
  char startRowCheck = currentPiece.Row;
  if (startRowCheck < 1) startRowCheck = 1; // Ensure we don't go below playable area (row 1)

  for (int rowCheck = startRowCheck; rowCheck < startRowCheck + 4 && rowCheck < 21; rowCheck++) { // Check rows affected by the piece
    bool fullLine = true;
    for (char coloumCheck = 2; coloumCheck < 12; coloumCheck++) { // Check playable columns (2-11)
      if (tetrisScreen[coloumCheck][rowCheck] == 0) { // If any cell in the line is empty
        fullLine = false;
        break;
      }
    }

    if (fullLine) {
      // Mark line for deletion (set to 3)
      for (char c = 2; c < 12; c++) tetrisScreen[c][rowCheck] = 3;
      linesProcessed++;

      delay(77); // Animation effect for line clear
      drawTetrisScreen(); // Refresh display to show cleared line (now black)
    }
  }

  // If any lines were processed, shift blocks down
  if (linesProcessed > 0) {
    clearedLines = linesProcessed;
    while (clearedLines > 0) {
      bool lineFoundToClear = false;
      for (char currentRow = 1; currentRow < 20; currentRow++) { // Iterate from bottom to top of playable area
        bool isEmptyRow = true;
        for (char c = 2; c < 12; c++) {
          if (tetrisScreen[c][currentRow] != 0) { // If any block exists in this row
            isEmptyRow = false;
            break;
          }
        }

        if (isEmptyRow) { // Found an empty row that was just cleared (or became empty due to shifting)
          lineFoundToClear = true;
          // Move all lines above this empty row down by one
          for (int r = currentRow; r < 20; r++) { // Start from the cleared row
            for (char c = 2; c < 12; c++) {
              tetrisScreen[c][r] = tetrisScreen[c][r + 1]; // Shift down
            }
          }
          // Clear the very top playable row (it's now shifted down)
          for (char c = 2; c < 12; c++) {
            tetrisScreen[c][20] = 0; // Make the new top row empty
          }
          break; // Process one empty line at a time, then re-check from bottom
        }
      }
      if (!lineFoundToClear) break; // No more empty lines to shift down
      clearedLines--;
      // After shifting, ensure all '2' markers (current piece) are '1' (landed)
      // This is important for subsequent collision checks.
      for (char r = 1; r < 21; r++) {
        for (char c = 2; c < 12; c++) {
          if (tetrisScreen[c][r] == 2) tetrisScreen[c][r] = 1;
        }
      }
      drawTetrisScreen(); // Refresh display after shifting
      delay(77); // Animation delay for shifting
    }
  }

  // Update score based on lines cleared
  switch (linesProcessed) {
    case 1: amountScored = 40 * (level + 1); break;
    case 2: amountScored = 100 * (level + 1); break;
    case 3: amountScored = 300 * (level + 1); break;
    case 4:
      amountScored = 1200 * (level + 1);
      // Tetris! Do a screen inverse effect
      display.invertDisplay(true);
      delay(100);
      display.invertDisplay(false);
      break;
  }

  // Score animation
  for (long s = score; s < score + amountScored; s = s + (1 * (level + 1))) {
    setScore(s, false);
    delay(5); // Small delay for score animation
  }
  score = score + amountScored;
  setScore(score, false); // Display final score

  // Update level line count and check for level up
  levellineCount = levellineCount + linesProcessed;
  if (levellineCount >= 10) { // Level up every 10 lines
    level++;
    levellineCount = levellineCount % 10; // Reset line count for next level

    // Level up effect
    display.invertDisplay(true);
    delay(100);
    display.invertDisplay(false);
    delay(100);
    display.invertDisplay(true);
    delay(100);
    display.invertDisplay(false);
  }
}

// For debug: prints the tetrisScreen array to Serial Monitor
void tetrisScreenToSerial() {
  for (int r = 24; r >= 0; r--) { // Print from top to bottom
    for (int c = 0; c < 14; c++) {
      Serial.print(tetrisScreen[c][r], DEC);
    }
    Serial.println();
  }
  Serial.println();
}

// Handles IR remote input
void handleIRInput() {
  if (IrReceiver.decode()) { // Check if an IR signal is received
    unsigned long irValue = IrReceiver.decodedIRData.decodedRawData; // Get the decoded value

    // For debugging: print the received IR code
    Serial.print("IR Code: 0x");
    Serial.println(irValue, HEX);

    bool handled = false; // Flag to indicate if a game action was performed

    if (irValue == IR_LEFT) {
      movePieceLeft();
      handled = true;
    } else if (irValue == IR_RIGHT) {
      movePieceRight();
      handled = true;
    } else if (irValue == IR_DOWN) {
      movePieceDown();
      handled = true;
    } else if (irValue == IR_ROTATE) {
      RotatePiece();
      handled = true;
    }

    if (handled) {
      drawPiece(); // Update piece position on game board array
      drawTetrisScreen(); // Redraw affected parts of the screen
    }
    IrReceiver.resume(); // Resume receiving the next IR signal
  }
}

// Sets and displays the score on the OLED using Adafruit_SSD1306
void setScore(long scoreToDisplay, bool blank) {
  display.setTextSize(1); // Small font for score
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(70, 0); // Position for score display (right side, top)

  if (blank) {
    display.fillRect(70, 0, 58, 16, SSD1306_BLACK); // Clear area for blinking
  } else {
    display.fillRect(70, 0, 58, 16, SSD1306_BLACK); // Clear previous score
    display.print(scoreToDisplay);
  }
  display.display(); // Push changes to the OLED
}

// Sets and displays the next block preview on the OLED using Adafruit_SSD1306
void setNextBlock(byte pieceNumber) {
  display.fillRect(70, 20, 58, 30, SSD1306_BLACK); // Clear previous next block area

  // Draw a simple representation of the next block
  // Position it below the score, e.g., starting at X=70, Y=20
  int startX = 70;
  int startY = 20;
  int blockSize = 6; // Each block is 6x6 pixels

  // This is a simplified drawing, not using the original nextBlockBuffer
  // as Adafruit GFX allows direct drawing.
  const bool (*blockPtr)[4];
  switch (pieceNumber) {
    case 1: blockPtr = &BlockI[0]; break;
    case 2: blockPtr = &BlockJ[0]; break;
    case 3: blockPtr = &BlockL[0]; break;
    case 4: blockPtr = &BlockO[0]; break;
    case 5: blockPtr = &BlockS[0]; break;
    case 6: blockPtr = &BlockT[0]; break;
    case 7: blockPtr = &BlockZ[0]; break;
    default: return;
  }

  for (int c = 0; c < 4; c++) {
    for (int r = 0; r < 4; r++) {
      if (pgm_read_byte(&blockPtr[c][r])) {
        display.fillRect(startX + r * blockSize, startY + c * blockSize, blockSize, blockSize, SSD1306_WHITE);
      }
    }
  }
  display.display(); // Push changes to the OLED
}

// Draws the bottom border of the Tetris screen using Adafruit_SSD1306
void drawBottom() {
  // Game area is 60 pixels wide (X: 0-59) and 120 pixels tall (Y: 0-119, but rotated to X: 0-119, Y: 0-59)
  // Bottom border is at the bottom of the game area, which is Y=59 (or 60 for the line itself)
  display.drawRect(0, 60, 60, 4, SSD1306_WHITE); // X, Y, Width, Height
}

// Draws the side borders of the Tetris screen using Adafruit_SSD1306
void drawSides() {
  // Draw a rectangle around the main game area (60x60 pixels in OLED coordinates)
  display.drawRect(0, 0, 60, 60, SSD1306_WHITE); // X, Y, Width, Height
}


void loop() {
  // Main game loop
  // To do: create high score system that saves to EEPROM

  gameOver = false;
  score = 0;
  level = 0;
  levellineCount = 0;
  dropDelay = 1000; // Reset game parameters

  fillTetrisArray(1); // Fill with 1's to make border (collision detection)
  fillTetrisScreen(0); // Clear playable area (set to 0)

  // Initial display animation
  // The original code had specific animations using '2' and '3' markers.
  // We'll simulate a brief full-screen flash then clear.
  display.clearDisplay();
  display.fillRect(0, 0, 60, 60, SSD1306_WHITE); // Fill game area white
  display.display();
  delay(200);
  display.clearDisplay();
  display.display();
  delay(200);

  // Draw static borders
  drawSides();
  drawBottom();
  display.display(); // Update display after drawing borders

  // Initial display inverse effect
  display.invertDisplay(true);
  delay(200);
  display.invertDisplay(false);

  // Load first piece
  loadPiece(random(1, 8), 19, 4, true); // Random piece (1-7), start at row 19, col 4 (top-center)
  drawTetrisScreen(); // Draw the first piece

  // Prepare and display the next piece
  nextPiece = random(1, 8); // Random piece (1-7)
  setNextBlock(nextPiece);

  // Initialize and animate score display
  setScore(0, false);
  delay(300);
  setScore(0, true); // Blink score
  delay(300);
  setScore(0, false); // Display score again

  // Game loop
  while (!gameOver) {
    movePieceDown(); // Attempt to move piece down
    drawPiece(); // Update piece's state on the game board
    drawTetrisScreen(); // Redraw affected parts of the screen

    moveTime = millis(); // Record time for drop delay
    // Loop to handle IR input until it's time for the piece to drop automatically
    while (millis() - moveTime < (dropDelay - (level * 50))) {
      handleIRInput(); // Check for and process IR remote presses
    }
  }

  // Game Over sequence (can be expanded)
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(15, 20);
  display.println("GAME OVER");
  display.display();
  delay(1000);

  // Flash screen for game over
  display.invertDisplay(true);
  delay(500);
  display.invertDisplay(false);
  delay(500);
  display.invertDisplay(true);
  delay(500);
  display.invertDisplay(false);
  delay(1000); // Wait a bit before restarting
}
