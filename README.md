# ESP32 Health Monitoring System

## Overview
This project is an ESP32-based health monitoring system that combines multiple sensors to track:

- Heart rate monitoring
- Step counting
- Calories burned estimation
- GPS distance tracking
- Force sensor readings
- Emergency heart-rate alerts

The system uses an ESP32 microcontroller with MPU6050, GPS, pulse sensor, and force sensor modules.

---

## Features
- Real-time heart rate monitoring
- Emergency alert when BPM drops below threshold
- GPS-based movement tracking
- Step counting using MPU6050 accelerometer
- Calories burned estimation
- Serial monitor output for debugging and monitoring

---

## Hardware Required
- ESP32 Development Board
- MPU6050 Accelerometer & Gyroscope
- Pulse Sensor
- GPS Module (NEO-6M or similar)
- Force Sensor (FSR)
- Jumper Wires
- Breadboard

---

## Pin Connections

| Component | ESP32 Pin |
|---|---|
| Pulse Sensor | GPIO 34 |
| Force Sensor | GPIO 35 |
| GPS RX | GPIO 16 |
| GPS TX | GPIO 17 |

---

## Libraries Required

Install the following libraries from the Arduino Library Manager:

- Adafruit MPU6050
- Adafruit Unified Sensor
- TinyGPSPlus
- PulseSensor Playground

---

## How It Works

### Heart Rate Monitoring
The pulse sensor reads BPM values continuously.  
If the BPM falls below the threshold value, a warning message is displayed.

### GPS Tracking
The GPS module tracks location changes and calculates distance traveled.

### Step Counting
The MPU6050 accelerometer detects movement and increments the step counter when acceleration crosses a threshold.

### Calories Burned
Calories are estimated using step count and MET calculations.

---

## Upload Instructions

1. Open the `.ino` file in Arduino IDE.
2. Select the correct ESP32 board.
3. Install all required libraries.
4. Connect the ESP32 board.
5. Upload the code.
6. Open Serial Monitor at `115200 baud`.

---

## Recommended File
Use `sketch_sep25b.ino` for GitHub because it has:
- Cleaner pulse detection logic
- Better variable handling
- Improved readability
- More stable loop timing

Rename it to:
`ESP32_Health_Monitor.ino`

---

## Future Improvements
- Add OLED/LCD display support
- Send alerts through GSM/WiFi
- Store data in cloud platforms
- Improve calorie calculation accuracy
- Add fall detection

---

## License
This project is open-source and free to use for educational purposes.
