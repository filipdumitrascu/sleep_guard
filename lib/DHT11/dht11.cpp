#include "dht11.h"
#include "utils.h"

DHT dht(dhtPin, DHT11);

void dhtInit()
{
    dht.begin();
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
}
