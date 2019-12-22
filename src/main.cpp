/**
 * 
 * Some basic code to control a (short) LED strip.
 * If it has a network connection already known, it will use that.
 * Otherwise, it will start a new Access Point, to which you should connect.
 * (The password is below in the settings). 
 * From there you can define which network should be used.
 * 
 * Once it is connected, you can browse to the IP it was assigned 
 * (Unfortently, it is not easy to show that to you, so you will need to check your router!)
 * and change the color of the LEDs.
 * 
 * Color info when booting:
 * - Blue: System is booting, and no network is available.
 * - Green: System is ready, WiFi is connected, and you change colors
 * 
 * Color code when doing an OTA update:
 * - Purple: System started an OTA update
 * - Gold/Yellowish: System is done with the update and will reboot soon (And the colors will change to the boot colors!)
 * - Red: A error happened.
 * 
 * OTA code based from https://educ8s.tv/esp8266-ota-wemos-d1/
 * WebServer code based from https://randomnerdtutorials.com/esp8266-web-server/
 **/

#include <Arduino.h>
#include "FastLED.h"
#include <DNSServer.h>        //Local DNS Server used for redirecting all requests to the configuration portal
#include <ESP8266WebServer.h> //Local WebServer used to serve the configuration portal
#include <WiFiManager.h>      //https://github.com/tzapu/WiFiManager WiFi Configuration Magic
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

// Change if you want to have DEBUG enabled for the OTA
#define DEBUG_ESP_OTA true
// Number of LEDs in the ledstrip & the PIN it is defined at
#define NUMBER_OF_LEDS 8
#define LED_PIN 5

// Network SSID and password
#define NETWORK_SSID "Christmas"
#define NETWORK_PASSWORD "configure"

// No need to change anything after this.
CRGB leds[NUMBER_OF_LEDS];
WiFiServer server(80);
String header;
String color;
unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeoutTime = 2000;

void changeColor(CRGB color)
{
  for (int i = 0; i < 9; i++)
  {
    leds[i] = color;
  }
  FastLED.show();
}

void setup()
{
  Serial.begin(115200);
  delay(10);

  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUMBER_OF_LEDS);
  FastLED.setMaxPowerInMilliWatts(1000);
  FastLED.setBrightness(255);

  changeColor(CRGB::Red);
  WiFiManager wifiManager;
  // wifiManager.resetSettings();

  changeColor(CRGB::Blue);

  wifiManager.autoConnect(NETWORK_SSID, NETWORK_PASSWORD);

  Serial.println("connected...yeey :)");
  Serial.println("local ip");
  Serial.println(WiFi.localIP());

  ArduinoOTA.onStart([]() {
    changeColor(CRGB::Purple);
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    changeColor(CRGB::Gold);
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    changeColor(CRGB::Red);
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR)
      Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR)
      Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR)
      Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR)
      Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR)
      Serial.println("End Failed");
  });
  ArduinoOTA.begin();

  server.begin();
  changeColor(CRGB::Green);
  color = "green";
}

void loop()
{
  ArduinoOTA.handle();
  WiFiClient client = server.available();

  if (client)
  {
    Serial.println("New Client.");
    String currentLine = "";
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime)
    {
      ArduinoOTA.handle();
      currentTime = millis();
      if (client.available())
      {
        char c = client.read();
        Serial.write(c);
        header += c;
        if (c == '\n')
        {
          if (currentLine.length() == 0)
          {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // turns the GPIOs on and off
            if (header.indexOf("GET /c/red") >= 0)
            {
              color = "red";
              changeColor(CRGB::Red);
            }
            else if (header.indexOf("GET /c/green") >= 0)
            {
              color = "green";
              changeColor(CRGB::Green);
            }
            else if (header.indexOf("GET /c/yellow") >= 0)
            {
              color = "yellow";
              changeColor(CRGB::Yellow);
            }
            else if (header.indexOf("GET /c/orange") >= 0)
            {
              color = "orange";
              changeColor(CRGB::Orange);
            }
            else if (header.indexOf("GET /c/off") >= 0)
            {
              color = "off";
              changeColor(CRGB::Black);
            }

            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style></head>");

            client.println("<body><h1>Update christmas house lights</h1>");
            client.println("<p>Current color - State " + color + "</p>");
            client.println("<p><a href=\"/c/green\"><button class=\"button\">Green</button></a></p>");
            client.println("<p><a href=\"/c/red\"><button class=\"button\">Red</button></a></p>");
            client.println("<p><a href=\"/c/yellow\"><button class=\"button\">Yellow</button></a></p>");
            client.println("<p><a href=\"/c/orange\"><button class=\"button\">Orange</button></a></p>");
            client.println("<p><a href=\"/c/off\"><button class=\"button\">Off</button></a></p>");
            client.println("</body></html>");
            client.println();
            break;
          }
          else
          {
            currentLine = "";
          }
        }
        else if (c != '\r')
        {
          currentLine += c;
        }
      }
    }
    header = "";
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}