#include <Arduino.h>
#include <M5Stack.h>
#include <Adafruit_NeoPixel.h>
#include <esp_sleep.h>

#define LED_PIN 15
#define NUM_LEDS 10

Adafruit_NeoPixel strip(
  NUM_LEDS, 
  LED_PIN, 
  NEO_GRB + NEO_KHZ800
);

void setColor(uint8_t r, uint8_t g, uint8_t b, const char* name) {
  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, strip.Color(r, g, b));
  }
  strip.show();

  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextSize(3);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setCursor(30, 100);
  M5.Lcd.print(name);
}

void goToSleep() {
    // Turn off RGB LEDs
    strip.clear();
    strip.show();

    // Turn off the LCD
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setBrightness(0);

    // Button A (GPIO 39) wakes the ESP32
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_39, 0);

    // Enter deep sleep
    esp_deep_sleep_start();
}

void setup() {
  M5.begin();
  strip.begin();
  strip.setBrightness(50);
  strip.clear();
  strip.show();

  setColor(255, 0, 0, "Red");
}

void loop() {
  M5.update();

  if(M5.BtnA.wasPressed()) {
    setColor(255, 0, 0, "Red");
  }
  if(M5.BtnB.wasPressed()) {
    setColor(0, 255, 0, "Green");
  }
  if(M5.BtnC.wasPressed()) {
    setColor(0, 0, 255, "Blue");
  }
  if(M5.BtnC.pressedFor(1000)) {
    goToSleep();
  }
  delay(20);
}