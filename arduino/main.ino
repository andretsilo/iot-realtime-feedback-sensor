#include <Arduino.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <EEPROM.h>
#include <SD.h>
#include "main.h"
#include "WifiUtils.h"
#include "Logger.h"
#include "ConfigUtils.h"
#include "EepromUtils.h"
#include "ApiClient.h"
#include "GrpcClient.h"
#include "model.h"
#include "feedback.h"
Sensor sensors[] = {
  {2, 3, LOW, 0},
  {4, 5, MID, 0},
  {6, 7, HIGH, 0}
};
const int NumSensors = sizeof(sensors) / sizeof(sensors[0]);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 0, 60000);
unsigned long ledUptimeMs   = 1000;   
unsigned long buttonResetMs = 4000;   
String sensorId;
int eepromAddr = 100;
typedef struct {
  String host;
  int port;
  String endpoint;
} ApiConfig;
ApiConfig apiCfg;
typedef struct {
  bool enabled;
  String host;
  int port;
  String endpoint;
} GrpcConfig;
GrpcConfig grpcCfg;
void updateTime() {
  static unsigned long last = 0;
  if (millis() - last >= 60000) { 
    timeClient.update();
    last = millis();
  }
}
void checkWiFi() {
  if (!WifiUtils::isConnected()) {
    Log.warningln("[WiFi] Disconnected. Reconnecting...");
    String ssid = ConfigUtils::get("sensor.wifi.ssid");
    String pass = ConfigUtils::get("sensor.wifi.password");
    WifiUtils::connect(ssid.c_str(), pass.c_str());
  }
}
void setup() {
  Logger::init();
  if (!SD.begin()) {
    Log.errorln("[SD] init failed. Insert SD or set correct CS pin.");
  } else {
    Log.infoln("[SD] OK");
  }
  if (!ConfigUtils::load("/config/sensor.properties")) {
    Log.errorln("[Config] Failed to load /config/sensor.properties");
  }
  String ssid = ConfigUtils::get("sensor.wifi.ssid", "network");
  String pass = ConfigUtils::get("sensor.wifi.password", "password");
  WifiUtils::connect(ssid.c_str(), pass.c_str());
  timeClient.begin();
  timeClient.update();
  ledUptimeMs   = ConfigUtils::get("sensor.led.uptime", "1").toInt() * 1000UL;
  buttonResetMs = ConfigUtils::get("sensor.button.reset", "4").toInt() * 1000UL;
  sensorId   = ConfigUtils::get("sensor.id", "ansklfn24asafd");
  {
    String addrStr = ConfigUtils::get("sensor.id.eeprom", "100");
    if (addrStr.startsWith("0x") || addrStr.startsWith("0X")) {
      eepromAddr = (int) strtoul(addrStr.c_str(), nullptr, 16);
    } else {
      eepromAddr = addrStr.toInt();
    }
  }
  EEPROM.begin(512);
  String stored = EepromUtils::readString(eepromAddr);
  if (stored != sensorId) {
    EepromUtils::writeString(eepromAddr, sensorId);
    Log.infoln("[EEPROM] Sensor ID written");
  } else {
    Log.infoln("[EEPROM] Sensor ID already present");
  }
  apiCfg.host     = ConfigUtils::get("sensor.api.host", "localhost");
  apiCfg.port     = ConfigUtils::get("sensor.api.port", "80").toInt();
  apiCfg.endpoint = ConfigUtils::get("sensor.api.endpoint", "/sensor");
  grpcCfg.enabled  = ConfigUtils::get("sensor.grpc.enabled", "0").toInt() == 1;
  grpcCfg.host     = ConfigUtils::get("sensor.api.grpc.host", "localhost");
  grpcCfg.port     = ConfigUtils::get("sensor.api.grpc.port", "80").toInt();
  grpcCfg.endpoint = ConfigUtils::get("sensor.api.grpc.endpoint", "/insertFeedback");
  for (int i = 0; i < NumSensors; i++) {
    pinMode(sensors[i].pinButton, INPUT);
    pinMode(sensors[i].pinLed, OUTPUT);
    digitalWrite(sensors[i].pinLed, LOW);
  }
  Log.infoln("[SETUP] Completed");
}
void loop() {
  checkWiFi();
  updateTime();
  handleSensors();
}

