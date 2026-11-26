#include <Arduino.h>
#include <M5Stack.h>

void setup() {
    M5.begin();

    Serial.begin(115200);
    Serial.println("M5GO started!"); 
    
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setTextSize(3);
    M5.Lcd.setCursor(20, 100);
    M5.Lcd.print("Hello, World!");
}

void loop() {
    M5.update();
}