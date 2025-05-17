#include "dht11.h"
#include "utils.h"
#include "sleep.h"

DHT dht(dhtPin, DHT11);

void dhtInit()
{
    dht.begin();
    Serial.println("Dht11 initialised");
}

void dhtReadData()
{
    if (currentDeviceState != MEASURE) {
        return;
    }
    
    float temp = dht.readTemperature();
    float hum = dht.readHumidity();

    if (isnan(temp) || isnan(hum)) {
        Serial.println("Failed to read from DHT11 sensor");
        return;
    }

    // Store data
    sleepData.ambientTemp = alpha * temp + (1.0 - alpha) * sleepData.ambientTemp;
    sleepData.humidity = alpha * hum    + (1.0 - alpha) * sleepData.humidity;
}
