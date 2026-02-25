#include <Arduino.h>
#include <Button.h>

int relayPin = 36;
int greenLEDPin = 2;
int redLEDPin = 1;
int redButtonPin = 9;
int blueButtonPin = 8;
int potiPin = 11;

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

