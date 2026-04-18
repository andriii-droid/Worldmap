#include <Arduino.h>
#include <Button.h>
#include <vector>
#include <NeoPixelBus.h>
#include <pattern.h>
#include <WiFi.h>
#include <PubSubClient.h>

void setup_wifi();
void reconnect();
void callback(char *topic, byte *payload, unsigned int length);
void newBrightness();
    void power(bool value);

    // --- Configuration ---
const char *ssid = "Zyxel_4061";
const char *password = "ip7r3am4f3g8ih7f";
const char *mqtt_server = "192.168.1.136"; // Your HA IP Address

constexpr int relayPin = 36;
constexpr int greenLEDPin = 1;
constexpr int redLEDPin = 2;
constexpr int redButtonPin = 9;
constexpr int blueButtonPin = 8;
constexpr int potiPin = 11;
constexpr int pixelPin = 6;
constexpr int numLeds = 99;
std::vector<int> cont = {20, 30, 49};

WiFiClient espClient;
PubSubClient client(espClient);
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

    digitalWrite(redLEDPin, HIGH);

    // Serial.begin(115200);

    strip.Begin();
    strip.ClearTo(RgbColor(0, 0, 0));
    strip.Show();
    setup_wifi();
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
    reconnect();
    power(false);

    delay(1000);
    digitalWrite(redLEDPin, LOW);
}

void loop()
{
    if (!client.connected()) { reconnect(); }
    client.loop();
    redButton.updateButton();
    blueButton.updateButton();

    if (blueButton.getState(blueButton.click)) {
        if (digitalRead(relayPin)) {
            power(false);
        } else {
            power(true);
        }
    }

    if (redButton.getState(redButton.click)) { worldMap.nextMode();}
    newBrightness();

    if (digitalRead(relayPin))
    {
        worldMap.run();

        for (size_t i = 0; i < numLeds; i++)
        {
            strip.SetPixelColor(i, RgbColor(worldMap.getRGB(i)));
        }
        strip.Show();
    }
}

void callback(char *topic, byte *payload, unsigned int length)
{
    String topicStr = String(topic);
    String message;
    for (int i = 0; i < length; i++)
    {
        message += (char)payload[i];
    }

    if (topicStr == "worldmap/led/set") {
        // Simple Command Logic
        if (message == "ON")
        {
            power(true);
        }
        else if (message == "OFF")
        {
            power(false);
        }
    }
    else if (topicStr == "worldmap/led/brightness/set")
    {
        int brightness;
        sscanf(message.c_str(), "%d", &brightness);
        worldMap.setBrightness(brightness);
        client.publish("worldmap/led/brightness/state", message.c_str());
    }
    else if (topicStr == "worldmap/led/rgb/set") {
        int r, g, b;
        if (sscanf(message.c_str(), "%d,%d,%d", &r, &g, &b) == 3)
        {
            worldMap.setColor(r, g, b);
            client.publish("worldmap/led/rgb/state", message.c_str());
        }
    }
}

void setup_wifi()
{
    delay(10);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
    }
}

void reconnect()
{
    while (!client.connected())
    {
        // Attempt to connect (Client ID, Username, Password)
        if (client.connect("ESP32S2_LED", "andri", "Harmonize7-Award6-Onscreen3-Length2-Implant2"))
        {
            client.subscribe("worldmap/led/set");            // For ON/OFF
            client.subscribe("worldmap/led/brightness/set"); // For Brightness slider
            client.subscribe("worldmap/led/rgb/set");
        }
        else
        {
            delay(5000);
        }
    }
}

void power(bool value) {
    if (value) {
        digitalWrite(relayPin, HIGH);
        client.publish("worldmap/led/state", "ON");
    } else {
        digitalWrite(relayPin, LOW);
        client.publish("worldmap/led/state", "OFF");
        strip.ClearTo(RgbColor(0, 0, 0));
        strip.Show();
    }
}

void newBrightness() {
    static int potiOld = 0;
    static int filterCycle = 0;
    static int brightness[5];
    brightness[filterCycle] = map(analogRead(potiPin), 20, 8100, 0, 255);
    if (filterCycle == 4) {filterCycle = 0; } else {++filterCycle;}
    int brightnessFiltered = 0;
    for (size_t i = 0; i < 5; i++)
    {
        brightnessFiltered += brightness[i];
    }
    brightnessFiltered /= 5;

    if (abs(brightnessFiltered - potiOld) > 10)
    {
        potiOld = brightnessFiltered;
        worldMap.setBrightness(brightnessFiltered);
        client.publish("worldmap/led/brightness/state", std::to_string(brightnessFiltered).c_str());
    }
}