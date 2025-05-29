#include "max4466.h"
#include "utils.h"
#include "sleep.h"

constexpr int adcCenter{ 2048 };
constexpr float Vref{ 3.3 };
constexpr int samples{ 100 };

void micInit()
{
    pinMode(micPin, INPUT);
    Serial.println("Microphone initialised");
}

void micReadData() {
    if (currentDeviceState != MEASURE) {
        return;
    }

    long sumSquares = 0;

    for (int i = 0; i < samples; i++) {
        int raw = analogRead(micPin);
        int amplitude = raw - adcCenter;     // remove DC offset

        sumSquares += amplitude * amplitude; // accumulate square of amplitudes
        delay(1);                            // small delay between samples
    }

    float rms = sqrt((float)sumSquares / samples);  // compute RMS value

    // Convert RMS ADC value to voltage
    float Vrms = (rms / 4095.0) * Vref;

    // Estimate Sound Pressure Level (SPL) in decibels
    // 0.00631 V corresponds to -44 dBV microphone sensitivity
    float dBSPL = 20.0 * log10(Vrms / 0.00631) + 94.0;

    // Store filtered noise level
    sleepData.noise = alpha * dBSPL + (1.0 - alpha) * sleepData.noise;
}
