#include "serverweb.h"
#include "utils.h"
#include "lcd.h"
#include "sleep.h"

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>

WebServer server(80);

unsigned long lastAnimTime = 0;
int dotCount = 0;

void dotAnimation() {
    unsigned long currentMillis = millis();

    // Update animation from 500 to 500 ms
    if (currentMillis - lastAnimTime >= 500) {
        lastAnimTime = currentMillis;

        lcd.setCursor(7, 1);
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
    if (currentDeviceState != WIFI_CONNECT) {
        return;
    }

    lcdPrintMessage("Connecting", "to WiFi", 2000);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    
    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
        dotAnimation();
        delay(100);
    }

    lcdPrintMessage("Connected!", "", 3000);

    Serial.printf("\nConnected to: %s\n IP address:", ssid);
    Serial.println(WiFi.localIP()); // 192.168.252.117

    server.on("/", handleRoot);

    server.begin();
    Serial.println("HTTP server started");

    // The end
    lcdPrintMessage("Data sent!", "Have a nice day!", 4000);
    lcd.clear();

    lcd.noBacklight();
    currentDeviceState = SERVER_ON;
}

void handleConnections()
{
    if (currentDeviceState != SERVER_ON) {
        return;
    }

    server.handleClient();
}

void handleRoot() {
    char msg[2500];

    auto formatTime = [](uint64_t ms) -> String {
        uint64_t sec = ms / 1000;
        unsigned int h = sec / 3600;
        unsigned int m = (sec % 3600) / 60;
        unsigned int s = sec % 60;

        char buffer[20];
        snprintf(buffer, sizeof(buffer), "%02u:%02u:%02u", h, m, s);
        return String(buffer);
    };

    const char* message = getSleepMessage();

    snprintf(msg, sizeof(msg),
            "<html>\
    <head>\
        <meta name='viewport' content='width=device-width, initial-scale=1'>\
        <link rel='stylesheet' href='https://use.fontawesome.com/releases/v5.7.2/css/all.css'>\
        <title>ESP32 Sleep Monitor</title>\
        <style>\
        body { font-family: Arial; text-align: center; margin: 20px; background-color: #f4f4f4; }\
        h2 { font-size: 2.5rem; margin-bottom: 10px; }\
        p { font-size: 1.3rem; margin: 12px 0; }\
        .data { font-weight: bold; color: #222; }\
        .section { margin: 25px auto; padding: 15px; background: #fff; border-radius: 10px; max-width: 500px; box-shadow: 0 2px 8px rgba(0,0,0,0.1); }\
        i { margin-right: 8px; }\
        </style>\
    </head>\
    <body>\
        <h2><i class='fas fa-bed'></i> ESP32 Sleep Monitor</h2>\
        <div class='section'>\
        <p><i class='fas fa-thermometer-half' style='color:#ca3517;'></i> Body Temp: <span class='data'>%.2f &deg;C</span></p>\
        <p><i class='fas fa-temperature-low' style='color:#ff8800;'></i> Ambient Temp: <span class='data'>%.2f &deg;C</span></p>\
        <p><i class='fas fa-tint' style='color:#00add6;'></i> Humidity: <span class='data'>%.2f %%</span></p>\
        <p><i class='fas fa-volume-up' style='color:#4444cc;'></i> Noise Level: <span class='data'>%.1f dB</span></p>\
        </div>\
        <div class='section'>\
        <p>Last Accel Y/Z: <span class='data'>%.2f / %.2f m/s&sup2;</span></p>\
        <p><i class='fas fa-sync-alt'></i> Last Gyro X/Y/Z: <span class='data'>%.2f / %.2f / %.2f rad/s</span></p>\
        </div>\
        <div class='section'>\
        <p><i class='fas fa-moon'></i> Deep Sleep: <span class='data'>%s</span></p>\
        <p><i class='fas fa-cloud'></i> REM Sleep: <span class='data'>%s</span></p>\
        <p><i class='fas fa-lightbulb'></i> Light Sleep: <span class='data'>%s</span></p>\
        <p><i class='fas fa-bed'></i> Restless: <span class='data'>%s</span></p>\
        </div>\
        <div class='section'>\
        <p><i class='fas fa-info-circle'></i> <span class='data'>%s</span></p>\
        </div>\
    </body>\
    </html>",
            sleepData.bodyTemp,
            sleepData.ambientTemp,
            sleepData.humidity,
            sleepData.noise,
            sleepData.accelY, sleepData.accelZ,
            sleepData.gyroX, sleepData.gyroY, sleepData.gyroZ,
            formatTime(deepSleepTime).c_str(),
            formatTime(remSleepTime).c_str(),
            formatTime(lightSleepTime).c_str(),
            formatTime(restlessTime).c_str(),
            message
    );

    server.send(200, "text/html", msg);
}
