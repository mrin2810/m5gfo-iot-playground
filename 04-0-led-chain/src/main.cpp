#include <Arduino.h>
#include <M5Stack.h>
#include <Adafruit_NeoPixel.h>

#define LED_PIN     15
#define NUM_LEDS   10
#define NUM_LEDS_HALF   5

Adafruit_NeoPixel strip(
  NUM_LEDS, 
  LED_PIN, 
  NEO_GRB + NEO_KHZ800
);

int currentLed = 0;
unsigned long lastUpdate = 0;
int speed = 500;
bool running = true;

void updateScreen(){
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextSize(2);

  M5.Lcd.setCursor(20, 30);
  M5.Lcd.printf("LED Animation");

  M5.Lcd.setCursor(20, 90);
  M5.Lcd.printf("Speed: %d", speed);

  M5.Lcd.setCursor(20, 140);
  M5.Lcd.printf(running ? "Running" : "Paused");

  M5.Lcd.setCursor(10, 210);
  M5.Lcd.printf("A: Pause  B: Speed  C: Reset");
}

void setup() {
  M5.begin();
  
  strip.begin();
  strip.setBrightness(50);
  strip.clear();
  strip.show();

  updateScreen();
}

void loop() {
  M5.update();
  for (int i = 0; i < NUM_LEDS_HALF; i++) {

        // Turn off all LEDs
        strip.clear();

        // Turn on only the current LED
        strip.setPixelColor(
            i,
            strip.Color(0, 0, 255)
        );
        strip.setPixelColor(
            (i + NUM_LEDS_HALF),
            strip.Color(0, 0, 255)
        );

        // Update the physical LEDs
        strip.show();

        // Display the LED number
        M5.Lcd.fillRect(0, 80, 320, 80, BLACK);
        M5.Lcd.setCursor(30, 100);
        M5.Lcd.printf("LED: %d", i + 1);

        Serial.printf("LED %d ON\n", i + 1);

        delay(800);
    }
}