#ifndef UTILS_H
#define UTILS_H

#include <Wire.h>
#include <Arduino.h>

constexpr int sdaEsp32{ 21 };
constexpr int sclEsp32{ 22 };
constexpr int baudRate{ 115200 };

enum FSM {INIT, SET_ALARM, MEASURE, WAKE_UP, SEND_DATA};
extern FSM currentDeviceState;

struct UserData {
    float accelX, accelY, accelZ;     // accelerometer (m/s^2)
    float gyroX, gyroY, gyroZ;        // gyroscop (rad/s)
    float bodyTemp;                   // body temperature (°C)
    float noiseLevel;                 // noise (dB)
    float ambientTemp;                // room temperature (°C)
    float humidity;                   // humidity (%)
};


/**
 * @brief Used only to discover i2c addresses
 * 
 * @return  0x27 for lcd
 *          0x68 for mpu6050 
 */
inline void discoverI2cDevices()
{
    Serial.println("Scanning I2C bus...");

    for (byte address = 1; address < 127; ++address) {
        Wire.beginTransmission(address);
        if (Wire.endTransmission() == 0) {
            Serial.print("Found device at 0x");
            Serial.println(address, HEX);
        }
    }

    Serial.println("Done scanning I2C bus.");
}

#endif
