#include <Arduino.h>
#include <M5Stack.h>
#include <Adafruit_NeoPixel.h>

#define LED_PIN 15
#define NUM_LEDS 10

Adafruit_NeoPixel strip(
    NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800
);
float accX = 0;
float accY = 0;
float accZ = 0;

unsigned long lastSensorRead = 0;
const unsigned long interval = 200; // 200 milliseconds

void setup() {
  M5.begin();
  Serial.begin(115200);

  M5.Lcd.setBrightness(200);

  // turn off the RGB light
  strip.begin();
  strip.clear();
  strip.show();

  // Initialize the IMU
  M5.Imu.Init();

  //Configure the DISPLAY
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.setTextSize(2);

  M5.Lcd.setCursor(20, 20);
  M5.Lcd.print("Motion Monitor");

  Serial.println("IMU Initialized");
}

void loop() {
  M5.update();

  // Update every 200 milliseconds
  if (millis() - lastSensorRead >= interval) {
    lastSensorRead = millis();

    M5.IMU.getAccelData(&accX, &accY, &accZ);

    // Clear previous readings
    M5.Lcd.fillRect(0, 60, 320, 140, BLACK);

    //Display the readings on the LCD
    M5.Lcd.setCursor(20, 70);
    M5.Lcd.printf("Accel X: %.2f g\n", accX);
    M5.Lcd.setCursor(20, 110);
    M5.Lcd.printf("Accel Y: %.2f g\n", accY);
    M5.Lcd.setCursor(20, 150);
    M5.Lcd.printf("Accel Z: %.2f g\n", accZ);

    // Debugging output to Serial Monitor
    Serial.printf(
        "X: %.2f, Y: %.2f, Z: %.2f\n",
        accX, accY, accZ
    );
  }
}