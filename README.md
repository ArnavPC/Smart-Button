# Smart Button

A Wi-Fi-enabled smart button for controlling room lights and displaying their current state on a 16×2 LCD.

The project runs on an ESP8266-based board and uses a button keypad to switch the lights on or off. Button presses are sent through [SinricPro](https://sinric.pro/), allowing the light state to be integrated with a smart-home setup.

## Features

- Control room lights with physical buttons
- Display Wi-Fi, service, and light status on a 16×2 LCD
- Connect to a 2.4 GHz Wi-Fi network using an ESP8266
- Send light-state changes through SinricPro
- Debounce state changes before sending events
- Built with PlatformIO and the Arduino framework

## Hardware

- ESP8266 development board compatible with the PlatformIO `d1` board definition
- 16×2 character LCD
- Analog button keypad connected to `A0`
- Jumper wires and a suitable power supply

### LCD pins used by the firmware

The LCD is initialized with the following ESP8266 pins:

```text
RS: D8
EN: D9
D4: D4
D5: D5
D6: D6
D7: D7
```

The button keypad is read through the ESP8266 analog input `A0`.

## Software requirements

- [PlatformIO](https://platformio.org/)
- ESP8266 platform support
- Arduino framework
- A SinricPro account and configured device credentials

The required libraries are declared in [`platformio.ini`](platformio.ini):

- `LiquidCrystal`
- `SinricPro`
- `LiquidCrystal_I2C`

## Configuration

Before uploading the firmware, update the configuration values in `src/main.cpp`:

```cpp
#define WIFI_SSID "your-wifi-name"
#define WIFI_PASS "your-wifi-password"
#define APP_KEY "your-sinricpro-app-key"
#define APP_SECRET "your-sinricpro-app-secret"
#define MOTIONSENSOR_ID "your-sinricpro-device-id"
```

> **Security:** Do not commit Wi-Fi passwords, API keys, or device secrets to a public repository. Prefer environment-specific configuration or a separate untracked header file, and rotate any credentials that have already been exposed.

## Build and upload

1. Clone the repository:

   ```bash
   git clone https://github.com/ArnavPC/Smart-Button.git
   cd Smart-Button
   ```

2. Open the project in VS Code with the PlatformIO extension installed.

3. Configure the credentials in `src/main.cpp`.

4. Connect the ESP8266 board over USB.

5. Build and upload the firmware:

   ```bash
   pio run
   pio run --target upload
   ```

6. Monitor the device output:

   ```bash
   pio device monitor
   ```

## Usage

- Press the **UP** button to turn the lights on.
- Press the **DOWN** button to turn the lights off.
- The LCD displays the current light state and connection status.
- The ESP8266 connects to Wi-Fi during startup and then connects to SinricPro.

The button thresholds are defined in the `but()` function in `src/main.cpp`. If a different keypad or resistor network is used, these thresholds may need to be recalibrated.

## Project structure

```text
.
├── include/          # Project header files
├── lib/              # Project-specific libraries
├── src/
│   └── main.cpp      # Main firmware source
├── test/             # PlatformIO test directory
└── platformio.ini    # PlatformIO project configuration
```

## Troubleshooting

### The board cannot connect to Wi-Fi

- Verify the SSID and password.
- Make sure the network provides 2.4 GHz Wi-Fi support.
- Check the serial monitor for connection progress.

### Button presses are not detected correctly

- Confirm that the keypad output is connected to `A0`.
- Check the keypad wiring and power supply.
- Adjust the analog thresholds in `but()` for the hardware being used.

### SinricPro does not receive updates

- Verify the application key, application secret, and device ID.
- Confirm that the ESP8266 has internet access.
- Check the serial monitor for SinricPro connection and event errors.

## License

No license has been specified yet. Add a license file if you plan to distribute or reuse this project.