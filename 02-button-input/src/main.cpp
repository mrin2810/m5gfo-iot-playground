#include <Arduino.h>
#include <M5Stack.h>

int counter = 0;

void displayCounter() {
    M5.Lcd.fillScreen(BLACK);
    if (counter < 10) {
        M5.Lcd.setTextColor(WHITE);
    } else {
        M5.Lcd.setTextColor(GREEN);
    }

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
        counter = counter + 5;
        displayCounter();
    }

    if (M5.BtnB.wasPressed()) {
        counter--;
        if (counter < 0) {
            counter = 0;
        }
        displayCounter();
    }

    if (M5.BtnC.wasPressed()) {
        counter = 0;
        displayCounter();
    }

    delay(20);
}