#include <Arduino.h>
#include <Button.h>
#include <vector>
#include <NeoPixelBus.h>
#include <pattern.h>

constexpr int relayPin = 36;
constexpr int greenLEDPin = 1;
constexpr int redLEDPin = 2;
constexpr int redButtonPin = 9;
constexpr int blueButtonPin = 8;
constexpr int potiPin = 11;
constexpr int pixelPin = 6;
constexpr int numLeds = 99;
std::vector<int> cont = {20, 30, 49};
Pattern worldMap(numLeds, cont);

NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt0800KbpsMethod> strip(numLeds, pixelPin);

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

    digitalWrite(redLEDPin, HIGH);
    digitalWrite(relayPin, LOW); 

    Serial.begin(115200);

    strip.Begin();
    strip.ClearTo(RgbColor(0, 0, 0));
    strip.Show();

    delay(1000);
    digitalWrite(redLEDPin, LOW);
}

void loop()
{
    redButton.updateButton();
    blueButton.updateButton();

    if (blueButton.getState(blueButton.click))
    {
        digitalWrite(relayPin, not digitalRead(relayPin));
    }

    if (redButton.getState(redButton.click))
    {
        worldMap.nextMode();
    }

    worldMap.run();

    for (size_t i = 0; i < numLeds; i++)
    {
        strip.SetPixelColor(i, RgbColor(worldMap.getRGB(i)));
    }
    strip.Show();
}

