
#include <Arduino.h>
#include <M5Stack.h>
#include <Adafruit_NeoPixel.h>
#include <esp_sleep.h>

// -----------------------------
// Hardware configuration
// -----------------------------

#define LED_PIN 15
#define NUM_LEDS 10

Adafruit_NeoPixel strip(
    NUM_LEDS,
    LED_PIN,
    NEO_GRB + NEO_KHZ800
);

// -----------------------------
// Global state
// -----------------------------

float accX = 0;
float accY = 0;
float accZ = 0;

bool motionMode = true;
bool buttonALongHandled = false;

uint16_t screenColor = BLUE;
const char* colorName = "BLUE";

unsigned long lastSensorRead = 0;
const unsigned long sensorInterval = 200;

const float tiltThreshold = 0.55;

// -----------------------------
// Display functions
// -----------------------------

void drawColorBand() {
    M5.Lcd.fillRect(
        0, 40, 320, 85, screenColor
    );

    M5.Lcd.fillRoundRect(
        85, 60, 150, 45, 6, BLACK
    );

    M5.Lcd.setTextSize(3);
    M5.Lcd.setTextColor(WHITE, BLACK);

    M5.Lcd.setCursor(100, 70);
    M5.Lcd.print(colorName);
}

void updateScreen() {
    // Header
    M5.Lcd.fillRect(0, 0, 320, 38, BLACK);

    M5.Lcd.setTextSize(2);
    M5.Lcd.setTextColor(WHITE, BLACK);
    M5.Lcd.setCursor(15, 10);
    M5.Lcd.println("Motion LCD");

    // Sensor information
    M5.Lcd.fillRect(0, 130, 320, 110, BLACK);

    M5.Lcd.setCursor(15, 135);
    M5.Lcd.printf(
        "Mode: %s",
        motionMode ? "AUTO" : "MANUAL"
    );

    M5.Lcd.setCursor(15, 160);
    M5.Lcd.printf(
        "X: %.2f  Y: %.2f",
        accX, accY
    );

    M5.Lcd.setCursor(15, 185);
    M5.Lcd.printf("Z: %.2f g", accZ);

    M5.Lcd.setCursor(10, 215);
    M5.Lcd.setTextSize(1);
    M5.Lcd.println(
        "Hold A: Mode | Hold C: Sleep"
    );
}

void setDisplayColor(
    uint16_t color,
    const char* name
) {
    screenColor = color;
    colorName = name;

    drawColorBand();
    updateScreen();
}

// -----------------------------
// Sensor functions
// -----------------------------

void readMotion() {
    M5.IMU.getAccelData(
        &accX,
        &accY,
        &accZ
    );

    Serial.printf(
        "X: %.2f | Y: %.2f | Z: %.2f\n",
        accX, accY, accZ
    );
}

void handleMotion() {
    if (!motionMode) {
        return;
    }

    // Tilt left
    if (accX < -tiltThreshold) {
        if (screenColor != BLUE) {
            setDisplayColor(BLUE, "BLUE");
        }
    }

    // Tilt right
    else if (accX > tiltThreshold) {
        if (screenColor != GREEN) {
            setDisplayColor(GREEN, "GREEN");
        }
    }

    // Tilt forward
    else if (accY > tiltThreshold) {
        if (screenColor != RED) {
            setDisplayColor(RED, "RED");
        }
    }

    // Tilt backward
    else if (accY < -tiltThreshold) {
        if (screenColor != YELLOW) {
            setDisplayColor(YELLOW, "YELLOW");
        }
    }
}

// -----------------------------
// Power management
// -----------------------------

void cleanup() {
    // Turn off RGB LEDs
    strip.clear();
    strip.show();

    // Turn off LCD
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setBrightness(0);

    Serial.println("Cleanup complete");
    Serial.flush();
}

void goToSleep() {
    cleanup();

    // Wake using Button A (GPIO 39)
    esp_sleep_enable_ext0_wakeup(
        GPIO_NUM_39, 0
    );

    esp_deep_sleep_start();
}

// -----------------------------
// Button handling
// -----------------------------

void handleButtons() {
    // Hold A: Toggle motion mode
    if (M5.BtnA.pressedFor(1200) &&
        !buttonALongHandled) {

        buttonALongHandled = true;
        motionMode = !motionMode;

        Serial.println(
            motionMode ? "AUTO MODE" : "MANUAL MODE"
        );

        updateScreen();
    }

    // Short press A: Red
    if (M5.BtnA.wasReleased()) {
        if (!buttonALongHandled) {
            motionMode = false;
            setDisplayColor(RED, "RED");
        }

        buttonALongHandled = false;
    }

    // Press B: Green
    if (M5.BtnB.wasPressed()) {
        motionMode = false;
        setDisplayColor(GREEN, "GREEN");
    }

    // Hold C: Deep sleep
    if (M5.BtnC.pressedFor(2000)) {
        Serial.println("Entering deep sleep");
        goToSleep();
    }

    // Short press C: Blue
    if (M5.BtnC.wasReleased()) {
        motionMode = false;
        setDisplayColor(BLUE, "BLUE");
    }
}

// -----------------------------
// Setup
// -----------------------------

void setup() {
    M5.begin();
    Serial.begin(115200);

    // Initialize LCD
    M5.Lcd.setBrightness(200);
    M5.Lcd.fillScreen(BLACK);

    // Initialize RGB LEDs and turn them off
    strip.begin();
    strip.setBrightness(40);
    strip.clear();
    strip.show();

    // Initialize accelerometer
    M5.IMU.Init();

    // Initial display
    drawColorBand();
    updateScreen();

    Serial.println("Project 5 started!");
}

// -----------------------------
// Main loop
// -----------------------------

void loop() {
    M5.update();

    // Check physical buttons
    handleButtons();

    // Read motion sensor every 200 ms
    if (millis() - lastSensorRead >= sensorInterval) {
        lastSensorRead = millis();

        readMotion();
        handleMotion();
        updateScreen();
    }
}
