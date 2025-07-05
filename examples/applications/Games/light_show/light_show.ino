#include <Adafruit_NeoPixel.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "SMLimage.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NEOPIXEL_PIN 5
#define NUM_LEDS 8
Adafruit_NeoPixel ring(NUM_LEDS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  ring.begin();
  ring.show();

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (true);
  }

  display.clearDisplay();
  display.drawBitmap(0, 0, SMLimage, 128, 64, SSD1306_WHITE);
  display.display();
  delay(2000);
  display.clearDisplay();
  display.display();
}

void loop() {
  blackPantherChase(100);
  rainbowCycle(5);
  theaterChaseRainbow(50);
  pulseWhite();
  colorWipe(ring.Color(255, 0, 0), 50);
  colorWipe(ring.Color(0, 255, 0), 50);
  colorWipe(ring.Color(0, 0, 255), 50);
}

void blackPantherChase(int wait) {
  uint32_t purple = ring.Color(128, 0, 128);
  uint32_t gold   = ring.Color(255, 215, 0);
  uint32_t white  = ring.Color(255, 255, 255);
  uint32_t colors[] = {purple, gold, white, purple, gold, white, purple, gold};

  for (int offset = 0; offset < NUM_LEDS; offset++) {
    for (int i = 0; i < NUM_LEDS; i++) {
      ring.setPixelColor(i, colors[(i + offset) % NUM_LEDS]);
    }
    ring.show();
    delay(wait);
  }
}

void rainbowCycle(uint8_t wait) {
  for (long firstPixelHue = 0; firstPixelHue < 5 * 65536; firstPixelHue += 256) {
    for (int i = 0; i < ring.numPixels(); i++) {
      int pixelHue = firstPixelHue + (i * 65536L / ring.numPixels());
      ring.setPixelColor(i, ring.gamma32(ring.ColorHSV(pixelHue)));
    }
    ring.show();
    delay(wait);
  }
}

void theaterChaseRainbow(int wait) {
  for (int j = 0; j < 256; j++) {
    for (int q = 0; q < 3; q++) {
      for (int i = 0; i < ring.numPixels(); i += 3) {
        int hue = (i * 256 / ring.numPixels() + j) & 255;
        ring.setPixelColor(i + q, ring.gamma32(ring.ColorHSV(hue * 256)));
      }
      ring.show();
      delay(wait);
      for (int i = 0; i < ring.numPixels(); i += 3) {
        ring.setPixelColor(i + q, 0);
      }
    }
  }
}

void pulseWhite() {
  for (int j = 0; j < 256; j++) {
    for (int i = 0; i < ring.numPixels(); i++) {
      ring.setPixelColor(i, ring.Color(j, j, j));
    }
    ring.show();
    delay(5);
  }
  for (int j = 255; j >= 0; j--) {
    for (int i = 0; i < ring.numPixels(); i++) {
      ring.setPixelColor(i, ring.Color(j, j, j));
    }
    ring.show();
    delay(5);
  }
}

void colorWipe(uint32_t color, int wait) {
  for (int i = 0; i < ring.numPixels(); i++) {
    ring.setPixelColor(i, color);
    ring.show();
    delay(wait);
  }
}
