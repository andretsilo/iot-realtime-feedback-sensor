/*
#ifndef WIFIUTILS_H
#define WIFIUTILS_H
#include <WiFiS3.h>
#include "Logger.h"
namespace WifiUtils {
    void connect(const char* ssid, const char* password) {
        Log.infoln("[WiFi] Connecting...");
        WiFi.begin(ssid, password);
        unsigned long startTime = millis();
        while (WiFi.status() != WL_CONNECTED && millis() - startTime < 15000) {
            delay(500);
            Serial.print(".");
        }
        if (WiFi.status() == WL_CONNECTED) {
            Log.infoln("[WiFi] Connected!");
            Serial.print("IP: ");
            Serial.println(WiFi.localIP());
        } else {
            Log.errorln("[WiFi] Connection failed.");
        }
    }
    bool isConnected() {
        return WiFi.status() == WL_CONNECTED;
    }
}
#endif
*/
#ifndef WIFI_UTILS_H
#define WIFI_UTILS_H
#include <Arduino.h>
#include <WiFiS3.h>
#include "Logger.h"
namespace WifiUtils {
  inline void connect(const char* ssid, const char* password) {
    Log.infoln("[WiFi] Connecting...");
    WiFi.begin(ssid, password);
    unsigned long start = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - start < 15000) {
      delay(500);
      Serial.print(".");
    }
    if (WiFi.status() == WL_CONNECTED) {
      Log.infoln("\n[WiFi] Connected");
      IPAddress ip = WiFi.localIP();
      Serial.print("IP: "); Serial.println(ip);
    } else {
      Log.errorln("\n[WiFi] Connection failed");
    }
  }
  inline bool isConnected() {
    return WiFi.status() == WL_CONNECTED;
  }
}
#endif 