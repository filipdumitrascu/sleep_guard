#include "utils.h"
#include "lcd.h"
#include "serverweb.h"
#include "buttons.h"
#include "buzzer.h"
#include "mpu6050.h"
#include "dht11.h"
#include "max4466.h"
#include "sleep.h"

#include <esp_bt.h>
#include <esp32-hal-bt.h>

FSM currentDeviceState;
UserData sleepData;
SleepType currentSleepType;

void setup()
{
    // Disable bluetooth (better power management)
    btStop();
    esp_bt_controller_disable();
    esp_bt_controller_deinit();

    // Phase0: init hardware components and the time
    // after which the alarm will ring
    currentDeviceState = INIT;
    currentSleepType = RESTLESS;

    /* 0. Initializations
    - i2c (on esp32)
    - 115200 8N1 serial
    - lcd
    - buttons
    - buzzer
    - mpu6050
    - microphone max4466
    - dht11 */
    Wire.begin(sdaEsp32, sclEsp32);
    Serial.begin(baudRate);

    lcd.init();
    lcd.backlight();

    buttonsInit();
    buzzerInit();

    mpuInit();
    dhtInit();
    micInit();

    // Welcome screen and set sleep duration
    lcdPrintMessage("SleepGuard", "welcomes you!", 3500);
    lcdPrintMessage("How much do you", "want to sleep?", 3500);

    // No lcd flickering in loop function
    lcd.clear();
}

void loop()
{
    buttonsLogic();

    // Phase1: Set the alarm
    setAlarm();

    // Phase2: Measure user's sleep quality
    mpuReadData();
    dhtReadData();
    micReadData();
    updateSleepType();

    // Phase3: Wake up
    buzzerLogic();

    // Phase4: Connect to wifi (send sleep data to external device)
    connectToWiFi();
    handleConnections();
}



// #include <WiFi.h>  // Pentru ESP32
// // #include <ESP8266WiFi.h> // Decomentează dacă folosești ESP8266

// const char* ssid = "Net pt sarac";
// const char* password = "filipinezul44";

// void setup() {
//   Serial.begin(115200);
//   WiFi.begin(ssid, password);

//   Serial.print("Conectare la WiFi...");
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.print(".");
//   }

//   Serial.println("\nConectat cu succes!");
//   Serial.print("IP: ");
//   Serial.println(WiFi.localIP());
// }

// void loop() {
//   // Codul tău aici
// }
