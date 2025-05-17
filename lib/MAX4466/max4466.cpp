#include "max4466.h"
#include "utils.h"
#include "sleep.h"

constexpr int adcCenter{ 2048 };    // ADC baseline (12 bits)
constexpr int threshold{ 200 };     // noise threshold
constexpr int samples {100};       // num of samples

void micInit()
{
    pinMode(micPin, INPUT);
    Serial.println("Microphone initialised");
}

void micReadData()
{
    if (currentDeviceState != MEASURE) {
        return;
    }
    
    long sumSquares = 0;

    for (int i = 0; i < samples; i++) {
        int raw = analogRead(micPin);
        int amplitude = raw - adcCenter;

        sumSquares += amplitude * amplitude;
        delay(1);
    }

    float rmsAmplitude = sqrt((float)sumSquares / samples);
    float dB = 0.0;

    if (rmsAmplitude > threshold) {
        dB = 20.0 * log10(rmsAmplitude / threshold);
    }

    // Update stored data
    sleepData.noise = alpha * dB + (1.0 - alpha) * sleepData.noise;
}
