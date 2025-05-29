#include "buzzer.h"
#include "utils.h"
#include "buttons.h"
#include "lcd.h"
#include "sleep.h"

volatile bool timerDone = false;
hw_timer_t *scanTimer = NULL;

void IRAM_ATTR scanISR()
{
    timerDone = true;
}

void buzzerInit()
{
    pinMode(buzzerPin, OUTPUT);
    digitalWrite(buzzerPin, LOW);

    Serial.println("Buzzer initialised");
}

void setAlarm()
{
    if (currentDeviceState != SET_ALARM) {
        return;
    }

    uint32_t targetSeconds = numHours * 3600 + numMinutes * 60 + numSeconds;

    // Timer0, prescaler 80 (1µs per tick), count up
    scanTimer = timerBegin(0, 80, true);
    timerAttachInterrupt(scanTimer, &scanISR, true);

    uint64_t targetMicros = (uint64_t)targetSeconds * 1000000ULL;
    timerAlarmWrite(scanTimer, targetMicros, false);

    // Timer starts
    lcdPrintMessage("Have a nice", "sleep!", 3000);
    timerAlarmEnable(scanTimer);

    // Phase2: While the timer that triggers the alarm runs down
    // (and the user sleeps), the user's sleep quality is measured
    currentDeviceState = MEASURE;

    lcd.noBacklight();
    lcd.clear();

    Serial.printf("Timer set to trigger in %lu seconds.\n\n", targetSeconds);
    
    // start timestamp
    stageStartTime = millis();
}

void buzzerLogic()
{
    if (timerDone) {
        timerDone = false;
        currentDeviceState = WAKEUP;
        lcd.backlight();

        lcdPrintMessage("Wake up!", "", 0);
        tone(buzzerPin, buzzerFrequency);
    }

    if (currentDeviceState == WAKEUP && buttonSetPressed) {
        noTone(buzzerPin);
        lcd.clear();

        buttonSetPressed = false;
        currentDeviceState = WIFI_CONNECT;
    }
}
