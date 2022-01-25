#include <ESP8266WiFi.h>

#include <DNSServer.h>
#include <ESP8266WebServer.h>

#include <Ticker.h>

#include "WiFiManager.h"
#include "CTBot.h"



Ticker ticker;
CTBot myBot;

String token = "yourToken";

void tick() {
  int state = digitalRead(LED_BUILTIN);
  digitalWrite(LED_BUILTIN, !state);
}

void configModeCallback(WiFiManager *myWiFiManager) {
  Serial.println("Entering config mode...");
  Serial.println(WiFi.softAPIP());
  ticker.attach(0.2, tick);
}

void startBot() {
  Serial.println("Starting Telegram Bot...");

  myBot.wifiConnect(WiFi.SSID(), WiFi.psk());
  myBot.setTelegramToken(token);

  if (myBot.testConnection()) {
    Serial.println("Bot connected.");
  } else {
    Serial.println("Failed to connect bot.");
    Serial.println("Make sure your WiFi connected to the internet");
    Serial.println("or recheck your Telegram token.");
    Serial.println("Resetting ESP...");
    ESP.reset();
    delay(1000);
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);

  ticker.attach(0.6, tick);

  WiFiManager wifiManager;
  wifiManager.resetSettings();
  wifiManager.setAPCallback(configModeCallback);

  if (wifiManager.autoConnect("AP SSID", "AP pass")) {
    Serial.println("WiFi connected.");
    ticker.detach();
    digitalWrite(LED_BUILTIN, LOW);
    startBot();
  } else {
    Serial.println("Failed to connect WiFi.");
    ESP.reset();
    delay(1000);
  }
}


void loop() {
  TBMessage msg;

  if(CTBotMessageText == myBot.getNewMessage(msg)) {
    myBot.sendMessage(msg.sender.id, msg.text);
  }

  delay(500);
}
