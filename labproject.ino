#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Pin definitions
const int pH_pin = A0;    // Analog pin for pH sensor
const int TDS_pin = A1;   // Analog pin for TDS sensor

// Calibration values
float pH_offset = 0.0;    // pH calibration offset
float TDS_factor = 5.0;   // TDS calibration factor

// Variables for sensor readings
float pH_value = 0.0;
float TDS_value = 0.0;
float TDS_voltage = 0.0;

// Voltage and conversion factors
const float voltage_ref = 5.0;       // Reference voltage
const int ADC_resolution = 1024;    // 10-bit ADC resolution

// OLED display settings
#define SCREEN_WIDTH 128            // OLED display width, in pixels
#define SCREEN_HEIGHT 64            // OLED display height, in pixels
#define OLED_ADDR 0x3C              // I2C address for the OLED

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  Serial.println("Water Quality Monitoring - pH and TDS");

  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);  // Infinite loop if OLED initialization fails
  }

  display.clearDisplay();
  display.setTextSize(1);           // Set text size
  display.setTextColor(SSD1306_WHITE);
  display.display();
}

void loop() {
  // Read and calculate pH value
  int pH_raw = analogRead(pH_pin);
  float pH_voltage = (pH_raw * voltage_ref) / ADC_resolution;
  pH_value = ((3.5 * pH_voltage) + pH_offset)+5;

  // Read and calculate TDS value
  int TDS_raw = analogRead(TDS_pin);
  TDS_voltage = (TDS_raw * voltage_ref) / ADC_resolution;
  TDS_value = ((TDS_voltage * TDS_factor) * 100)-2150;  // Convert to ppm

  // Display readings on Serial Monitor
  Serial.print("pH: ");
  Serial.print(pH_value);
  Serial.print(" | TDS: ");
  Serial.print(TDS_value);
  Serial.println(" ppm");

  // Display readings on OLED
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Water Quality Monitor");
  
  display.setCursor(0, 20);
  display.print("pH: ");
  display.print(pH_value, 2);       // Show 2 decimal places

  display.setCursor(0, 40);
  display.print("TDS: ");
  display.print(TDS_value, 2);      // Show 2 decimal places
  display.print(" ppm");

  display.display();                // Update the OLED display

  delay(1000);                      // Delay for stability
}

