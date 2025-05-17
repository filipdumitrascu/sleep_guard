#include "serverweb.h"
#include "utils.h"
#include "lcd.h"
#include "sleep.h"

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
//#include <ESPmDNS.h>

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
    }

    lcdPrintMessage("Connected!", "", 3000);

    Serial.printf("\nConnected to: %s\n IP address:", ssid);
    Serial.println(WiFi.localIP()); // 192.168.115.167

    // // Config NTP
    // configTime(gmtOffsetSec, daylightOffsetSec, "pool.ntp.org");
    // Serial.println("Time synchronized with NTP");

    // if (MDNS.begin("esp32")) {
    //     Serial.println("MDNS responder started");
    // }

    server.begin();
    Serial.println("HTTP server started");

    currentDeviceState = SEND_DATA;
}

void sendData()
{
    if (currentDeviceState != SEND_DATA) {
        return;
    }

    server.on("/", handleRoot);

    // The end
    currentDeviceState = END;
    lcdPrintMessage("Data sent!", "Have a nice day!", 2000);

    lcd.clear();
    lcd.noBacklight();

    WiFi.disconnect();
}

void handleRoot() {
    char msg[2000];

    // Format miliseconds → h:m:s
    auto formatTime = [](uint64_t ms) -> String {
        uint64_t sec = ms / 1000;
        uint64_t h = sec / 3600;
        uint64_t m = (sec % 3600) / 60;
        uint64_t s = sec % 60;

        char buffer[20];
        snprintf(buffer, sizeof(buffer), "%02llu:%02llu:%02llu", h, m, s);
        return String(buffer);
    };

    snprintf(msg, sizeof(msg),
             "<html>\
  <head>\
    <meta http-equiv='refresh' content='5'/>\
    <meta name='viewport' content='width=device-width, initial-scale=1'>\
    <title>ESP32 Sleep Monitor</title>\
    <style>\
      body { font-family: Arial; text-align: center; margin: 20px; }\
      h2 { font-size: 2.5rem; }\
      p { font-size: 1.2rem; margin: 10px 0; }\
      .data { font-weight: bold; color: #333; }\
    </style>\
  </head>\
  <body>\
    <h2>ESP32 Sleep Monitor</h2>\
    <p>Body Temp: <span class='data'>%.2f &deg;C</span></p>\
    <p>Ambient Temp: <span class='data'>%.2f &deg;C</span></p>\
    <p>Humidity: <span class='data'>%.2f %%</span></p>\
    <p>Noise (dB): <span class='data'>%.2f dB</span></p>\
    <p>Last Accel: <span class='data'>Y=%.2f m/s², Z=%.2f m/s²</span></p>\
    <p>Last Gyro: <span class='data'>X=%.2f, Y=%.2f, Z=%.2f rad/s</span></p>\
    <hr>\
    <p>Deep Sleep Time: <span class='data'>%s</span></p>\
    <p>REM Sleep Time: <span class='data'>%s</span></p>\
    <p>Light Sleep Time: <span class='data'>%s</span></p>\
    <p>Restless Time: <span class='data'>%s</span></p>\
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
             formatTime(restlessTime).c_str()
    );

    server.send(200, "text/html", msg);
}
