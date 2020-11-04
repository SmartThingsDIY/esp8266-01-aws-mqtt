/**
  Reads data from RX pin, packages it into a Json object and sends it to AWS IoT, then responds through pin TX to the Arduino Board listening on the other side
  @author MecaHumArduino
  @version 3.0
*/

#include <time.h>
#include "secrets.h"
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h> //https://github.com/bblanchon/ArduinoJson (use v6.xx)
#include <PubSubClient.h>
#include <SoftwareSerial.h>
#include <WiFiClientSecure.h>

#define DEBUG true

const int MQTT_PORT         = 8883;
const char MQTT_PUB_TOPIC[] = "$aws/things/" THINGNAME "/shadow/pub";

uint8_t DST = 0;
WiFiClientSecure net;

// ESP TX => Uno Pin 2 (2 in binary is 10)
// ESP RX => Uno Pin 3 (3 in binary is 11)
SoftwareSerial UnoBoard(10, 11);

BearSSL::X509List cert(cacert);
BearSSL::X509List client_crt(client_cert);
BearSSL::PrivateKey key(privkey);

PubSubClient client(net);

unsigned long lastMillis = 0;
time_t now;
time_t nowish = 1510592825;

// **************
void connectToMqtt();
void NTPConnect(void);
void sendDataToAWS(void);
void checkWiFiThenMQTT(void);
void connectToWiFi(String init_str);
void messageReceived(char *topic, byte *payload, unsigned int length);
String sendDataToUno(String command, const int timeout, boolean debug);
void setup();
void loop();
// **************

void NTPConnect(void)
{
  Serial.print("Setting time using SNTP");
  sendDataToUno("Setting time using SNTP\r\n", 1000, DEBUG);

  configTime(TIME_ZONE * 3600, DST * 3600, "pool.ntp.org", "time.nist.gov");
  now = time(nullptr);

  while (now < nowish) {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }

  Serial.println(" done!");
  sendDataToUno(" done!\r\n", 1000, DEBUG);

  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);

  Serial.print("Current time: ");
  Serial.print(asctime(&timeinfo));
}

void messageReceived(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Received [");
  Serial.print(topic);
  Serial.print("]: ");

  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }

  Serial.println();
}

void connectToMqtt()
{
  Serial.print("MQTT connecting ");
  sendDataToUno("MQTT connecting \r\n", 1000, DEBUG);

  while (!client.connected()) {
    if (client.connect(THINGNAME)) {
      Serial.println("connected!");
      sendDataToUno("connected! \r\n", 1000, DEBUG);
    } else {
      Serial.print("failed, reason -> ");
      Serial.println(client.state());
      Serial.println(" < try again in 5 seconds");
      delay(5000);
    }
  }
}

void connectToWiFi(String init_str)
{
  Serial.print(init_str);

  WiFi.hostname(THINGNAME);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println(" ok!");
}

void checkWiFiThenMQTT(void)
{
  connectToWiFi("Checking WiFi");
  sendDataToUno("Checking WiFi \r\n", 1000, DEBUG);

  connectToMqtt();
}

void sendDataToAWS(void)
{
  StaticJsonDocument<200> doc;

  // read data coming from Uno board and put into variable "doc"
  DeserializationError error = deserializeJson(doc, Serial.readString());

  // Test if parsing succeeds.
  if (error) {
    Serial.print("deserializeJson() failed.");
    return;
  }

  // parsing succeeded, continue and set time
  doc["time"] = String(millis());

  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer);

  Serial.printf("Sending  [%s]: ", MQTT_PUB_TOPIC);
  if (!client.publish(MQTT_PUB_TOPIC, jsonBuffer, false)) {
    Serial.println(client.state());
  }
}

String sendDataToUno(String command, const int timeout, boolean debug)
{
  String response = "";
  UnoBoard.print(command); // send the read character to the Uno
  long int time = millis();

  while( (time+timeout) > millis()) {
    while(UnoBoard.available()) {
      // The esp has data so display its output to the serial window
      char c = UnoBoard.read(); // read the next character.
      response+=c;
    }
  }

  if (debug) {
    Serial.print(response);
  }

  return response;
}


void setup()
{
  Serial.begin(9600);
  Serial.println("starting setup");

  UnoBoard.begin(9600); // your esp's baud rate might be different
  delay(2000);

  connectToWiFi(String("Attempting to connect to SSID: ") + String(ssid));

  NTPConnect();

  net.setTrustAnchors(&cert);
  net.setClientRSACert(&client_crt, &key);

  client.setServer(MQTT_HOST, MQTT_PORT);
  client.setCallback(messageReceived);

  connectToMqtt();
}

void loop()
{
  now = time(nullptr);

  if (!client.connected()) {
    checkWiFiThenMQTT();
  } else {
    client.loop();
    if (millis() - lastMillis > 5000) {
      lastMillis = millis();
      sendDataToAWS();
    }
  }
}