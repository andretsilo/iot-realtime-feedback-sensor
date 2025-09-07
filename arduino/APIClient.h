#ifndef API_CLIENT_H
#define API_CLIENT_H
#include <Arduino.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include "Logger.h"
#include "model.h"
namespace ApiClient {
  inline void sendFeedback(const FeedbackModel &fb, const String &host, int port, const String &endpoint) {
    WiFiClient client;
    if (!client.connect(host.c_str(), port)) {
      Log.errorln("[REST] Connection failed: " + host + ":" + String(port));
      return;
    }
    DynamicJsonDocument doc(256);
    doc["sensorId"] = fb.sensorId;
    doc["feedback"] = fb.feedback;
    doc["timestamp"] = fb.timestamp;
    doc["timezone"] = fb.timezone;
    String body; serializeJson(doc, body);
    client.print(String("POST ") + endpoint + " HTTP/1.1\r\n" \
                 + "Host: " + host + "\r\n" \
                 + "Content-Type: application/json\r\n" \
                 + "Connection: close\r\n" \
                 + "Content-Length: " + body.length() + "\r\n\r\n" \
                 + body);
    Log.infoln("[REST] Sent payload: " + body); 
    String statusLine = client.readStringUntil('\n');
    statusLine.trim();
    if (statusLine.length()) {
      Log.infoln("[REST] Status: " + statusLine); 
      if (statusLine.indexOf("200") > 0) Log.infoln("[REST] OK (200)");
      else if (statusLine.indexOf("400") > 0) Log.errorln("[REST] BAD_REQUEST (400)");
      else if (statusLine.indexOf("500") > 0) Log.errorln("[REST] INTERNAL_SERVER_ERROR (500)");
    }
    while (client.connected()) {
      String line = client.readStringUntil('\n');
      if (line == "\r") break; 
    }
    client.stop();
  }
}
#endif