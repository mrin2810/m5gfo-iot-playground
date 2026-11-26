#include <M5Stack.h>

void setup() {
    M5.begin();

    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextSize(3);
    M5.Lcd.setTextColor(WHITE);

    M5.Lcd.setCursor(30, 100);
    M5.Lcd.print("Hello, World!");
}

void loop() {
    M5.update();
}