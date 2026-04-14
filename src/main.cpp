#include <Arduino.h>
#include <Button.h>
#include <vector>
#include <NeoPixelBus.h>
#include <FrameFade.h>
#include <PixelRun.h>
#include <FadeIn.h>
#include <MultiFade.h>
#include <MultiFrame.h>
#include <MultiFadeIn.h>

constexpr int relayPin = 36;
constexpr int greenLEDPin = 1;
constexpr int redLEDPin = 2;
constexpr int redButtonPin = 9;
constexpr int blueButtonPin = 8;
constexpr int potiPin = 11;
constexpr int pixelPin = 100;
constexpr int numLeds = 99;
int constexpr maxModi = 7;
std::vector<int> cont = {50, 49};
int modi = 0;
int modiLast = 0;
NeoPixelModi *Mode[maxModi];

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

    digitalWrite(relayPin, HIGH); //Turn Relay on per default for testing

    Serial.begin(115200);

    strip.Begin();
    strip.Show();

    Mode[0] = new OneFrame{5};
    Mode[1] = new FrameFade{5};
    Mode[2] = new PixelRun{5};
    Mode[3] = new MultiFade{5, cont};
    Mode[4] = new MultiFrame{5, cont};
    Mode[5] = new FadeIn{5};
    Mode[6] = new MultiFadeIn{5, cont};

    Mode[modi]->setSpeed(255);
}

void loop()
{
    redButton.updateButton();
    blueButton.updateButton();

    if (blueButton.getState(blueButton.click))
    {
        Serial.println("loop");

        int rgb[3];
        Mode[modi]->createGoodRGB(rgb);
        Mode[modi]->setColor(rgb[0], rgb[1], rgb[2]);
    }

    if (redButton.getState(redButton.click))
    {
        ++modi;
        if (modi == maxModi)
        {
            modi = 0;
        }
    }

    if (modi != modiLast)
    {
        Mode[modi]->setBrightness(Mode[modiLast]->getBrightness());
        Mode[modi]->setSpeed(Mode[modiLast]->getSpeed());

        modiLast = modi;
    }

    Mode[modi]->run();

    for (size_t i = 0; i < numLeds; i++)
    {
        strip.SetPixelColor(i, RgbColor(Mode[modi]->getR(i), Mode[modi]->getG(i), Mode[modi]->getB(i)));
    }

    strip.Show();
}

