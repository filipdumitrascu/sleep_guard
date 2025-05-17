#include "max4466.h"
#include "utils.h"

constexpr int adcCenter = 2048;    // ADC baseline (12 bits)
constexpr int threshold = 200;     // noise threshold
constexpr int samples = 100;       // num of samples

void micInit()
{
    pinMode(micPin, INPUT);
}

void micReadData()
{
    if (currentDeviceState != MEASURE) {
        return;
    }
    
    long sumAmplitude = 0;

    for (int i = 0; i < samples; i++) {
        int raw = analogRead(micPin);
        int amplitude = abs(raw - adcCenter);

        sumAmplitude += amplitude;
        delay(1);
    }

    int avgAmplitude = sumAmplitude / samples;
    float db{};

    if (avgAmplitude > threshold) {
        db = 20.0 * log10((float)avgAmplitude / (float)threshold);
    }
}
