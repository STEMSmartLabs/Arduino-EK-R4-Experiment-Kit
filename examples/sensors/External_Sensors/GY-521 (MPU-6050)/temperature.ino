#include <Wire.h>           // Required for I2C communication (used by OLED and MPU6050)
#include <Adafruit_GFX.h>   // Core graphics library for OLED
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
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;) delay(100); // Don't proceed, loop forever
  }

  // Clear the screen for output
  display.clearDisplay();
  display.display(); // Ensure the display is blank initially

  // --- Initialize External MPU-6050 Accelerometer/Gyroscope ---
  // Check if the MPU-6050 sensor is connected and initialized properly
  if (!mpu.begin()) { // Use 'mpu.begin()' for MPU6050
    Serial.println(F("Failed to initialize external MPU-6050 sensor!"));
    // While loop to halt execution if sensor initialization fails
    while(1) delay(100);
  }

  Serial.println(F("External MPU-6050 (GY-521) Initialized. Displaying Temperature data..."));
  // Initial display message
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println(F("MPU-6050 Temp"));
  display.println(F("Initializing..."));
  display.display();
  delay(2000);
}

void loop() {
  // Event objects to hold sensor data
  sensors_event_t a, g, temp; // 'a' for acceleration, 'g' for gyroscope, 'temp' for temperature
  // Get new sensor events (reads data from the accelerometer, gyroscope, and temperature)
  mpu.getEvent(&a, &g, &temp); // Use 'mpu.getEvent()' for MPU6050

  // Clear the display buffer for the new readings
  display.clearDisplay();

  // Set text properties
  display.setTextSize(1); // Larger text for temperature
  display.setTextColor(SSD1306_WHITE);

  // Display "Temperature" heading
  display.setCursor(0,0);
  display.println(F("Temperature:"));

  // --- Display Temperature ---
  display.setCursor(0, 30); // Position for the temperature value
  display.print(temp.temperature, 2); // Display with 2 decimal places
  display.print(F(" C")); // Append Celsius symbol

  // --- Update the OLED display ---
  display.display();

  // Small delay to make readings readable
  delay(500); // Slightly longer delay for temperature as it doesn't change as rapidly
}


