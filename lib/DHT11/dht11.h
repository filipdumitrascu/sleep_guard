#ifndef DHT11_H
#define DHT11_H

#include <DHT.h>
#include <Wire.h>

constexpr int dhtPin{ 4 };

/**
 * @brief Initialize dht11
 */
void dhtInit();

/**
 * @brief Read ambiental
 * temperature and humidity
 */
void dhtReadData();

#endif
