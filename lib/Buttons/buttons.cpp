#include "buttons.h"
#include "utils.h"
#include "lcd.h"

TimeSettingStage timeStage = HOURS;

int numHours = 0;
int numMinutes = 0;
int numSeconds = 0;

volatile bool buttonNextPressed = false;
volatile bool buttonSetPressed = false;

unsigned long lastPressNext = 0;
unsigned long lastPressSet = 0;

void IRAM_ATTR handleButtonNext()
{
    unsigned long currentMillis = millis();
    if (currentMillis - lastPressNext > debounceDelay) {
        buttonNextPressed = true;
        lastPressNext = currentMillis;
    }
}

void IRAM_ATTR handleButtonSet()
{
    unsigned long currentMillis = millis();
    if (currentMillis - lastPressSet > debounceDelay) {
        buttonSetPressed = true;
        lastPressSet = currentMillis;
    }
}

void buttonsInit() 
{
    // Activates pull up resistance
    pinMode(buttonNextPin, INPUT_PULLUP);
    pinMode(buttonSetPin, INPUT_PULLUP);

    // Detect interrupt
    attachInterrupt(digitalPinToInterrupt(buttonNextPin), handleButtonNext, FALLING);
    attachInterrupt(digitalPinToInterrupt(buttonSetPin), handleButtonSet, FALLING);
}

void buttonsLogic()
{
    if (currentDeviceState != INIT) {
        return;
    }

    switch (timeStage) {
        case HOURS:
            lcd.setCursor(0, 0);
            lcd.print("Hours: ");

            lcd.setCursor(7, 0);
            lcd.printf("%02d", numHours);

            if (buttonNextPressed) {
                numHours = (numHours + 1) % 24;
                buttonNextPressed = false;
            }

            if (buttonSetPressed) {
                buttonSetPressed = false;
                delay(200);

                timeStage = MINUTES;
            }

            break;

        case MINUTES:
            lcd.setCursor(0, 0);
            lcd.print("Minutes: ");

            lcd.setCursor(9, 0);
            lcd.printf("%02d", numMinutes);

            if (buttonNextPressed) {
                numMinutes = (numMinutes + 1) % 60;
                buttonNextPressed = false;
            }

            if (buttonSetPressed) {
                buttonSetPressed = false;
                delay(200);

                timeStage = SECONDS;
            }

            break;

        case SECONDS:
            lcd.setCursor(0, 0);
            lcd.print("Seconds: ");

            lcd.setCursor(9, 0);
            lcd.printf("%02d", numSeconds);

            if (buttonNextPressed) {
                numSeconds = (numSeconds + 1) % 60;
                buttonNextPressed = false;
            }

            if (buttonSetPressed) {
                buttonSetPressed = false;

                delay(200);
                Serial.printf("Timer set for %02d:%02d:%02d\n", numHours, numMinutes, numSeconds);

                lcdPrintMessage("Timer set for: ", "", 0);

                lcd.setCursor(0, 1);
                lcd.printf("%02d:%02d:%02d", numHours, numMinutes, numSeconds);

                delay(2000);
                timeStage = DONE;
            }

            break;

        case DONE:
            currentDeviceState = SET_ALARM;
            break;
    }
}
