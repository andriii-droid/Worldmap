#include <Arduino.h>
#include <Button.h>

int relayPin = 30;
int greenLEDPin = 4;
int redLEDPin = 3;
int redButtonPin = 11;
int blueButtonPin = 10;
int potiPin = 13;

// Button Initialization
Button redButton{redButtonPin};
Button blueButton{blueButtonPin};

void setup()
{
    // Pinmodes
    pinMode(relayPin, OUTPUT);
    pinMode(greenLEDPin, OUTPUT);
    pinMode(redLEDPin, OUTPUT);
    pinMode(potiPin, INPUT);
    digitalWrite(greenLEDPin, HIGH);
    digitalWrite(LED_BUILTIN, HIGH);

    Serial.begin(115200);
}

void loop()
{
    if (redButton.getState(redButton.click))
    {
        Serial.println("Red button pressed");
        if (digitalRead(LED_BUILTIN))
        {
            digitalWrite(LED_BUILTIN, LOW);
        } else
        {
            digitalWrite(LED_BUILTIN, HIGH);
        }
    }

    if (blueButton.getState(blueButton.click))
    {
        Serial.println("Blue button pressed");
        if (digitalRead(LED_BUILTIN))
        {
            digitalWrite(LED_BUILTIN, LOW);
        }
        else
        {
            digitalWrite(LED_BUILTIN, HIGH);
        }
    }
}

