#include "serverweb.h"
#include "utils.h"
#include "lcd.h"

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

WebServer server(80);

unsigned long lastAnimTime = 0;
int dotCount = 0;

void dotAnimation() {
    unsigned long currentMillis = millis();

    // Update animation from 500 to 500 ms
    if (currentMillis - lastAnimTime >= 500) {
        lastAnimTime = currentMillis;

        lcd.setCursor(10, 0);
        for (int i = 0; i < 3; i++) {
            if (i < dotCount) {
                lcd.print(".");
            } else {
                lcd.print(" ");
            }
        }

        dotCount = (dotCount + 1) % 4;
    }
}

void connectToWiFi()
{
    if (currentDeviceState != INIT) {
        return;
    }

    lcdPrintMessage("Connecting", "to WiFi...", 2000);

    // WiFi.mode(WIFI_STA);

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
        WiFi.begin(ssid, password);

        delay(50);
    }

    lcdPrintMessage("Connected!", "", 3000);

    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP()); // 192.168.115.167

    // Config NTP
    configTime(gmtOffsetSec, daylightOffsetSec, "pool.ntp.org");
    Serial.println("Time synchronized with NTP");

    if (MDNS.begin("esp32")) {
        Serial.println("MDNS responder started");
    }

    server.begin();
    Serial.println("HTTP server started");
}

void sendData()
{
    if (currentDeviceState != SEND_DATA) {
        return;
    }
}
