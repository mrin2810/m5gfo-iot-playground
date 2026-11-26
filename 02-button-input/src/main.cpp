#include <Arduino.h>
#include <M5Stack.h>

int counter = 0;

void displayCounter() {
    M5.Lcd.fillScreen(BLACK);

    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setTextSize(3);
    
    M5.Lcd.setCursor(30, 40);
    M5.Lcd.printf("Counter:");

    M5.Lcd.setCursor(30, 110);
    M5.Lcd.printf(":%d", counter);
}

void setup() {
    M5.begin();
    displayCounter();
}

void loop() {
    M5.update();

    if (M5.BtnA.wasPressed()) {
        counter++;
        displayCounter();
    }

    if (M5.BtnB.wasPressed()) {
        counter--;
        displayCounter();
    }

    if (M5.BtnC.wasPressed()) {
        counter = 0;
        displayCounter();
    }

    delay(20);
}