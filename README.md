# M5GO IoT Playground

A hands-on embedded systems learning project built with the **M5GO IoT Starter Kit V2.7**, **ESP32**, **C++**, and **PlatformIO**.

The goal is to learn how software interacts with real hardware, one small experiment at a time. So far, the project covers physical button input, LCD output, RGB LED control, and non-blocking animations.

## Hardware and Tools

- **Board:** M5GO IoT Starter Kit V2.7 (M5Stack Core ESP32)
- **Microcontroller:** ESP32
- **Inputs:** Three physical buttons (A, B, C)
- **Outputs:** LCD display and 10 RGB LEDs
- **Development environment:** VS Code with PlatformIO
- **Programming language:** C++
- **Framework:** Arduino
- **Libraries:** M5Stack and Adafruit NeoPixel

## Projects

### 1. Button-Controlled Counter

A counter displayed on the LCD and controlled by three physical buttons.

| Button | Action |
|---|---|
| A | Increase counter by 1 |
| B | Decrease counter by 1 |
| C | Reset counter to 0 |

**Concepts learned:**
- Reading button events using `M5.update()` and `wasPressed()`
- Managing variables and program state
- Creating reusable functions
- Updating the LCD in response to hardware inputs

### 2. RGB LED Controller

Control the color of the M5GO's 10 RGB LEDs using physical buttons. The LCD displays the selected color.

| Button | LED color |
|---|---|
| A | Red |
| B | Green |
| C | Blue |

**Concepts learned:**
- Controlling addressable RGB LEDs
- Understanding RGB color values
- Using `setPixelColor()` and `show()`
- Creating reusable functions for hardware control
- Working with GPIO pins

### 3. Running LED Animation

Animate a single blue light moving across all 10 LEDs continuously.

| Button | Action |
|---|---|
| A | Pause or resume animation |
| B | Toggle between 150 ms and 70 ms |
| C | Reset animation |

**Concepts learned:**
- Non-blocking timing using `millis()`
- Tracking LED positions
- Using the modulo operator (`%`) for cyclic behavior
- Managing animation state
- Handling button input while animations are running

## Getting Started

### Prerequisites

- M5GO IoT Starter Kit V2.7
- USB-C data cable
- Visual Studio Code
- PlatformIO IDE extension

### PlatformIO Configuration

Create a PlatformIO project using:

- **Board:** `m5stack-core-esp32`
- **Framework:** Arduino

Add the following to `platformio.ini`:

```ini
[env:m5stack-core-esp32]
platform = espressif32@6.9.0
board = m5stack-core-esp32
framework = arduino

lib_deps =
    m5stack/M5Stack
    adafruit/Adafruit NeoPixel

monitor_speed = 115200
```

The ESP32 platform version is pinned for compatibility with the M5Stack library.

### Build and Upload

1. Open the project in VS Code.
2. Place the experiment code in `src/main.cpp`.
3. Open the PlatformIO sidebar.
4. Navigate to **Project Tasks → m5stack-core-esp32 → General**.
5. Select **Build**.
6. Once the build succeeds, select **Upload**.

Alternatively, use the PlatformIO CLI:

```bash
pio run
pio run -t upload
pio device monitor -b 115200
```

**Note:** The project was developed incrementally. The current experiment is stored in `src/main.cpp`, and previous implementations can be accessed through Git history.

## Understanding the LED Hardware

The M5GO RGB LEDs are controlled using GPIO 15.

```cpp
#define LED_PIN 15
#define NUM_LEDS 10
```

To change an LED color:

```cpp
strip.setPixelColor(0, strip.Color(0, 150, 255));
strip.show();
```

To turn off all LEDs:

```cpp
strip.clear();
strip.show();
```

The LEDs retain their last displayed color while receiving power, so stopping an animation does not automatically turn them off.

## Power Management

The project also explores software-controlled shutdown and cleanup.

Important observations:

- Pausing an animation does not necessarily turn off its LEDs.
- Hardware outputs should be explicitly reset before entering a low-power state.
- ESP32 deep sleep reduces power consumption but is not equivalent to a complete hardware shutdown.
- Physical power-off does not guarantee that cleanup code will execute.
- Arduino does not provide a universal shutdown callback.

A future enhancement is to implement a long-press gesture that clears the LEDs, dims the screen, and enters deep sleep.

## Troubleshooting

| Problem | Solution |
|---|---|
| `M5Stack.h` not found | Verify dependencies and rebuild the IntelliSense index |
| Library compilation errors | Use `espressif32@6.9.0` |
| `pio: command not found` | Use the PlatformIO extension or configure the CLI PATH |
| LEDs remain illuminated | Call `strip.clear()` followed by `strip.show()` |
| Upload failure | Check USB cable, serial port, and device connection |

## Future Projects

- [ ] Detect single, double, and triple button clicks
- [ ] Implement long-press gestures
- [ ] Create LED trails and gradients
- [ ] Build custom LED animation patterns
- [ ] Read sensor data
- [ ] Connect the ESP32 to Wi-Fi
- [ ] Send sensor readings to a web dashboard
- [ ] Build a complete IoT monitoring application

## Why I Am Building This

I started this project to gain practical experience with embedded systems and understand how software interacts with physical hardware.

Instead of starting with a complex IoT application, I am building small, incremental experiments that introduce one concept at a time.

The project currently covers embedded C++, hardware interfaces, state management, event handling, timing, and basic power management.

Each experiment builds on the previous one, with the long-term goal of creating a fully connected IoT application.

### Project 4: Running LED Animation Controller

Built an interactive LED animation controller using the M5GO V2.7's 10 built-in RGB LEDs, LCD screen, and physical buttons.

#### Features

- Animates one blue LED at a time across all 10 LEDs.
- Displays the active LED number on the LCD.
- Supports three animation speeds: 800 ms, 400 ms, and 150 ms.
- Allows users to pause, resume, and stop the animation.
- Automatically switches off all LEDs when paused or stopped.
- Uses non-blocking timing to keep button interactions responsive.

#### Button Controls

| Button | Action |
|---|---|
| A | Pause or resume the animation |
| B | Cycle through animation speeds |
| C | Stop the animation and turn off all LEDs |
| A after stopping | Restart the animation |

#### Technical Implementation

**1. Non-blocking timing**

Used `millis()` instead of `delay()` to control LED animation timing without blocking button input.

```cpp
if (running &&
    millis() - lastUpdate >= speeds[speedIndex]) {

    lastUpdate = millis();
    showNextLED();
}
```

**2. Cycling through LEDs**

Used the modulo operator to continuously cycle through the 10 LEDs.

```cpp
nextLed = (nextLed + 1) % NUM_LEDS;
```

**3. Animation state management**

Used a Boolean variable to manage the running and stopped states.

```cpp
running = !running;
```

**4. Hardware cleanup**

Implemented a reusable function to turn off all LEDs when the animation is stopped.

```cpp
void turnOffLEDs() {
    strip.clear();
    strip.show();
    activeLed = -1;
}
```

**5. LCD updates**

Created a reusable `updateScreen()` function to display:

- Current LED number
- Animation speed
- Running or stopped status

#### Concepts Learned

- Embedded C++ programming
- Non-blocking execution with `millis()`
- Hardware event handling
- State management
- Modular programming with reusable functions
- GPIO-based RGB LED control
- LCD rendering and screen refresh
- Hardware cleanup and output state management

#### Power Management Notes

Stopping an animation does not automatically turn off the LEDs. The LED hardware retains its last color while powered.

The program explicitly clears the LEDs using `strip.clear()` and `strip.show()`.

Software-controlled LED shutdown is different from ESP32 deep sleep or complete hardware power-off.

#### Future Improvements

- [ ] Add single-click, double-click, and triple-click detection
- [ ] Add multiple animation patterns
- [ ] Create a moving LED trail
- [ ] Add brightness controls
- [ ] Implement graceful deep sleep and wake-up
