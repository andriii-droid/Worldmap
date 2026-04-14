#include <Arduino.h>
#include <Button.h>
#include <vector>
#include <Adafruit_NeoPixel.h>
#include <OneFrame.h>
#include <FrameFade.h>
#include <PixelRun.h>
#include <FadeIn.h>
#include <MultiFade.h>
#include <MultiFrame.h>
#include <MultiFadeIn.h>

int relayPin = 36;
int greenLEDPin = 1;
int redLEDPin = 2;
int redButtonPin = 9;
int blueButtonPin = 8;
int potiPin = 11;
int pixelPin = 100;

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
    pinMode(LED_BUILTIN, OUTPUT);

    Serial.begin(115200);
}

void loop()
{
    redButton.updateButton();
    blueButton.updateButton();

    if (redButton.getState(redButton.click))
    {
        if (digitalRead(relayPin))
        {
            digitalWrite(relayPin, LOW);
        } else
        {
            digitalWrite(relayPin, HIGH);
        }
    }

    if (blueButton.getState(blueButton.click))
    {
        Serial.println("Blue button pressed");
        if (digitalRead(greenLEDPin))
        {
            digitalWrite(greenLEDPin, LOW);
        }
        else
        {
            digitalWrite(greenLEDPin, HIGH);
        }
    }
}

