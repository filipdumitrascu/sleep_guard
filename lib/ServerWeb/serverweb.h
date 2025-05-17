#ifndef SERVERWEB_H
#define SERVERWEB_H

constexpr const char *ssid{ "Filip's WIFI" }; // Net pt sarac
constexpr const char *password{ "0766301419" }; // filipinezul44

constexpr long gmtOffsetSec{ 2 * 3600 }; // GMT + 2 (Romania)
constexpr int daylightOffsetSec{ 3600 }; // Summer hour

/**
 * @brief Shows 1-3 dots 
 * after "Connecting" word.
 */
void dotAnimation();

/**
 * @brief Conencts esp32 to mobile Hotspot.
 */
void connectToWiFi();

/**
 * @brief Send data through wifi
 * to external device
 */
void sendData();

/**
 * @brief Html data
 */
void handleRoot();

#endif
