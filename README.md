# SleepGuard - Sleep Quality Monitor

#### Author: Dumitrascu Filip-Teodor

## Introduction

SleepGuard is an integrated system for real-time sleep quality monitoring, using specialized sensors. It captures user movement through an accelerometer (MPU6050), detects ambient noise and snoring via a microphone with amplifier (MAX4466), measures ambient temperature and humidity using a DHT11 sensor, and includes a programmable alarm clock. At the end of the sleep cycle, it sends a detailed report via WiFi to an external device (phone or laptop), including personalized advice for improving sleep quality.

## Purpose

The project aims to help users understand and improve their sleep habits by collecting relevant data such as motion, noise, and ambient temperature. The system also triggers an alarm designed to support a more natural and efficient wake-up experience.

## Inspiration

SleepGuard was inspired by the author's struggle with poor sleep quality and its critical role in both physical and mental health. Many individuals are unaware of their sleep quality, which is not determined solely by duration, but also by depth and continuity.

## Importance

SleepGuard offers an accessible and DIY-friendly solution for sleep monitoring, providing a viable alternative to commercial devices. By delivering clear and actionable data on night-time behavior, it supports users in adopting practical measures for better rest, which directly impacts overall well-being and daily productivity.

## Flow

Before going to bed, turn on the device, set the alarm by pressing a button, check the set time on the LCD, and press the start button. After the countdown ends, the buzzer sounds and the sleep data is transmitted over WiFi to an external device, showing how restless the sleep was (based on accelerometer readings), whether the user snored (from microphone input), and if the sleeping environment was optimal. Simple sleep advice is also displayed.

## Configuration

1. Run: `sudo chmod a+rw /dev/ttyUSB0` in terminal
2. Open browser and go to: `http://192.168.252.117`

## Hardware Overview

### Components:

* **ESP32-WROOM-32** - Microcontroller
* **MPU6050** - Accelerometer & Gyroscope module
* **MAX4466** - Microphone with amplifier
* **DHT11** - Temperature and humidity sensor
* **LCD 1602** - Liquid Crystal Display
* **Passive Buzzer 3.3V** - Audio output
* **Push Buttons** - Input for alarm and start
* **Breadboard Power Supply**
* **6x AA Battery Holder (9V total)**
* **External Device** - Phone/Laptop for data viewing

### Functionality:

* **Actuators**: LCD and buzzer
* **Sensors**: MPU6050 (movement, rotation, body temp), MAX4466 (sound), DHT11 (ambient temp & humidity), Buttons (alarm interaction)
* **WiFi**: Sends results to external device

### Interactions:

* ESP32 <-> LCD: status display, backlight off while sleeping
* Buttons <-> ESP32: alarm setup and stop
* Sensors <-> ESP32: collect sleep data
* ESP32 <-> Buzzer: wake-up signal
* ESP32 <-> External Device: transmit report

## Hardware Design

### Notes:

* Buttons use `INPUT_PULLUP`, no external resistors needed
* 100uF capacitor added between GND and VCC on ESP32 to prevent brownout during WiFi activity
* 6x AA batteries (9V total) power the breadboard supply

  * 5V rail -> LCD
  * 3.3V rail -> all other components

### Power Consumption:

| Component    | Idle (mA) | Running (mA) |
| ------------ | --------- | ------------ |
| ESP32 (WiFi) | 80        | 120-150      |
| MPU6050      | 0.5       | 3.9          |
| MAX4466      | 0.7       | 0.7          |
| DHT11        | 0.1       | 0.5          |
| LCD 1602     | 1         | 15-20        |
| Buttons      | 0         | 0            |
| Buzzer       | 0         | 30           |
| **Total**    | **82.3**  | **170-205**  |

### Battery Life:

* 6 AA batteries (2000–2500 mAh total):

  * \~12 hrs in Running
  * \~30 hrs in Idle
  * With software optimizations (sleep-only sensors, WiFi only at wake, LCD backlight off, no Bluetooth), practical runtime: \~24 hrs (3 nights)

## Software Design

* **PlatformIO + Arduino Framework**
* **IDE**: VS Code + PlatformIO extension

### Libraries:

* `MPU6050 @ ^1.0.0`
* `adafruit/DHT sensor @ ^1.4.6`
* `LiquidCrystal_I2C @ ^1.1.0`

### Architecture:

* FSM-based control (see `utils.h`)
* Main states:

  * `INIT`: initialize components, set sleep time
  * `SET_ALARM`: configure timer
  * `MEASURE`: collect sensor data during sleep
  * `WAKEUP`: sound buzzer, stop sensors
  * `WIFI_CONNECT`: connect and prepare data
  * `SEND_DATA`: send data to web client

### Data Displayed:

* Sleep stages (deep, rem, light, restless) and duration
* Body temperature
* Noise level (snoring, noise)
* Ambient temperature & humidity

### Sleep Stage Thresholds:

| Parameter      | Deep Sleep   | REM Sleep   | Light Sleep | Restless Sleep |
| -------------- | ------------ | ----------- | ----------- | -------------- |
| Acceleration Δ | < 0.8 m/s²   | < 1.5 m/s²  | < 3.0 m/s²  | ≥ 3.0 m/s²     |
| Gyroscope Δ    | < 0.25 rad/s | < 0.5 rad/s | < 1.0 rad/s | ≥ 1.0 rad/s    |
| Noise (dB SPL) | < 100 dB     | < 105 dB    | < 115 dB    | ≥ 115 dB       |

## Results

* The accelerometer is placed in the pajama chest pocket
* Example web output after sleep period shows summary and stage breakdown

## Demo

\[Insert link or image of the demo result here]

## Conclusions

SleepGuard highlights the importance of sleep quality regardless of sleep duration. Monitoring can be done with accessible components and an ESP32 board. Key accomplishments:

* Low power despite hardware complexity
* Structured FSM state handling
* Clear data visualization via web interface

### Future Work:

* Deeper data analysis and sleep optimization tips
* Custom alarm tones
* Smartwatch integration for portability
* Better health-based result interpretation

## Download

Source code, images and documentation available on \[GitHub Repository]

### GitHub Content:

* Source code
* README file
* All images from OCW

## Timeline / Journal

* Project idea: 29.04.2025
* Confirmation: 30.04.2025
* Order components: 03.05.2025
* Documentation skeleton: 04.05.2025
* Components delivered: 08.05.2025
* LCD and power wired: 10.05.2025
* LCD code working: 11.05.2025
* Buttons, buzzer, MPU6050 installed: 13.05.2025
* Code for buttons, buzzer, MPU6050: 13.05.2025
* MAX4466 and DHT11 wired: 15.05.2025
* Code for mic and DHT11: 17.05.2025
* WiFi & web server integration: 17.05.2025
* Final software: 17.05.2025
* Final documentation: 18.05.2025

## References / Resources

### Hardware:

* ESP32-WROOM: \[Optimus Digital]
* MPU6050: \[Sigmanortec]
* MAX4466: \[Sigmanortec]
* Passive Buzzer: \[Optimus Digital]
* Buttons: \[Optimus Digital]
* DHT11: \[Optimus Digital]
* LCD 1602: \[Optimus Digital]
* Power Supply: \[Optimus Digital]
* Battery holder: \[Optimus Digital]

### Software:

* PlatformIO + Arduino
* ESP32 (30 pin version) config
* Estimating sleep via accelerometer
* Calculating decibels using MAX4466
* Brownout WiFi fix on ESP32
