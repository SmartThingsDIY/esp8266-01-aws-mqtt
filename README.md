# Connects and Arduino board to WiFi using an ESP8266 ESP01 module, then sends data to AWS IOT through an MQTT topic

This repo accompanies the ["Connected Weather Station" project](https://github.com/MecaHumArduino/arduino-uno-aws-weather-station) but can also be used as a standalone repo to learn how an [Arduino Uno](https://amzn.to/2RnHhPY) board can communicate with an ESP8266 WiFi module in order to gain access to the internet.

⚡️ COMPONENTS AND SUPPLIES
--------------------------

<img align="right" src="https://i0.wp.com/schemobotics.com/wp-content/uploads/2019/12/Esp8266-Esp-01-Serial-Wifi-Wireless-Transceiver-Module-b.jpg?fit=500%2C500" style="max-width:100%;" height="300">


*   [Arduino Uno](https://amzn.to/2RnHhPY)
*   [Breadboard](https://amzn.to/2Ei40tP) - [Jumper Wires](https://amzn.to/2Ehh2ru) - [Male to Male Jumper Wires + Tweezer](https://amzn.to/3jcf9eX)
*   [ESP8266 ESP-01 WiFi Module](https://amzn.to/30fUWNS)
*   [ESP8266 ESP-01 programmable USB](https://amzn.to/345egi6)
*   [ESP8266 ESP-01 Breadboard Adapter](https://amzn.to/3kSFVcP)

🖥 APPS
------

*   [VSCode](https://code.visualstudio.com/)
*   [Fritzing](https://fritzing.org/)
*   [PlatformIO](https://platformio.org/)

📦 Libraries
---------
*   [Time](https://www.arduinolibraries.info/libraries/time)
*   [ESP8266WiFi](https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi)
*   [ArduinoJson](https://github.com/bblanchon/ArduinoJson)
*   [PubSubClient](https://github.com/knolleary/pubsubclient)
*   [SoftwareSerial](https://www.arduino.cc/en/Reference.SoftwareSerial)
*   [WiFiClientSecure](https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi)


ESP8266 ESP01 - Schematic Diagram
------------------------------

The ESP8266 WiFi Module is a self-contained stack that can give any microcontroller access to your WiFi network. The ESP8266 is capable of either hosting an application or offloading all Wi-Fi networking functions from another application processor. The ESP8266 module is an extremely cost-effective board with a huge, and ever-growing, community.

<img align="center" src="https://github.com/MecaHumArduino/arduino-uno-aws-weather-station/blob/master/docs/esp8266-schema.png?raw=true" style="max-width:100%;" height="600">

Code Walkthrough
--------------------

- Use ESP8266 Arduino Core version **2.5.0-beta2 or greater**
- Will **NOT** work on ESP8266 Arduino Core v2.4.2 !!!


WIP....
