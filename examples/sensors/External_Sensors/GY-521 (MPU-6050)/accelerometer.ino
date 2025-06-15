#include <Wire.h>             // Required for I2C communication (used by OLED and MPU6050)
#include <Adafruit_GFX.h>     // Core graphics library for OLED
#include <Adafruit_SSD1306.h> // OLED display library

// Libraries for external I2C Accelerometer/Gyroscope (MPU-6050 on GY-521 module)
#include <Adafruit_Sensor.h>  // Required for Adafruit Unified Sensor framework
#include <Adafruit_MPU6050.h> // Library for MPU6050

// --- OLED Display Configuration ---
#define SCREEN_WIDTH 128      // OLED display width, in pixels
#define SCREEN_HEIGHT 64      // OLED display height, in pixels
#define OLED_RESET -1         // Reset pin # (or -1 if sharing Arduino reset pin)
#define OLED_I2C_ADDRESS 0x3C // Common I2C address for 128x64 OLED (or 0x3D)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// --- External Accelerometer/Gyroscope Configuration (MPU-6050) ---
Adafruit_MPU6050 mpu;

void setup() {
  Serial.begin(9600); // Initialize serial communication for debugging

  // --- Initialize OLED Display ---
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;) delay(100); // Don't proceed, loop forever
  }

  display.clearDisplay();
  display.display(); // Ensure the display is blank initially
  
  // --- Initialize External MPU-6050 Accelerometer/Gyroscope ---
  if (!mpu.begin()) {
    Serial.println(F("Failed to initialize external MPU-6050 sensor!"));
    while(1) delay(100);
  }

  // Configure MPU6050 settings (optional)
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ); // Set DLPF for smoothing readings

  Serial.println(F("External MPU-6050 (GY-521) Initialized. Reading Accelerometer data..."));
}

void loop() {
  // Event objects to hold sensor data (only 'a' for acceleration is used here)
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp); // Get new sensor events (reads all data, but we only use 'a')

  // Clear the display buffer for the new readings
  display.clearDisplay();
  

  // Set text properties
  display.setTextSize(1);
  
  display.setTextColor(SSD1306_WHITE);
  
  display.setCursor(0, 0);
  display.print(F("Accelerometer")); 
  
  // --- Display X-axis acceleration ---
  display.setCursor(0, 10);
  display.print(F("x: "));
  display.print(a.acceleration.x, 2);
  display.println(F(" m/s^2"));

  // --- Display Y-axis acceleration ---
  display.setCursor(0, 30);
  display.print(F("y: "));
  display.print(a.acceleration.y, 2);
  display.println(F(" m/s^2"));

  // --- Display Z-axis acceleration ---
  display.setCursor(0, 50);
  display.print(F("z: "));
  display.print(a.acceleration.z, 2);
  display.println(F(" m/s^2"));

  // --- Update the OLED display ---
  display.display();

  // Small delay to make readings readable
  delay(100);
}
