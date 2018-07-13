#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <MqttConnector.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_ADS1015.h>
#include <LiquidCrystal_I2C.h>

#include "init_mqtt.h"
#include "_publish.h"
#include "_receive.h"
#include "_config.h"


LiquidCrystal_I2C lcd(0x27, 16, 2);
int val, x;

Adafruit_ADS1115 ads1115;

MqttConnector *mqtt;

int relayPin = 15;
int relayPinState = HIGH;
char myName[40];

void init_hardware()
{
  pinMode(relayPin, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(relayPin, relayPinState);;
  // serial port initialization

  Serial.begin(57600);
  delay(10);
  Serial.println();
  Serial.println("Starting...");

  lcd.begin();
  lcd.setCursor(0, 0);
  lcd.print("reading = ");
  lcd.setCursor(0, 2);
  lcd.print("voltage = ");
  ads1115.setGain(GAIN_ONE);
  ads1115.begin();
}

void init_wifi() {
  WiFi.disconnect();
  delay(20);
  WiFi.mode(WIFI_STA);
  delay(50);
  const char* ssid =  WIFI_SSID.c_str();
  const char* pass =  WIFI_PASSWORD.c_str();
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.printf ("Connecting to %s:%s\r\n", ssid, pass);
    delay(300);
  }
  Serial.println("WiFi Connected.");
  digitalWrite(LED_BUILTIN, HIGH);
}

void setup()
{
  lcd.begin();
  lcd.setCursor(0, 0);
  lcd.print("reading = ");
  lcd.setCursor(0, 2);
  lcd.print("voltage = ");
  ads1115.setGain(GAIN_ONE);
  ads1115.begin();
  init_hardware();
  init_wifi();
  init_mqtt();
}

void loop()
{
  mqtt->loop();
}
