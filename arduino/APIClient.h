/*
#ifndef API_CLIENT_H
#define API_CLIENT_H
#include <ArduinoJson.h>
#include <WiFiClient.h>
#include "Logger.h"
#include "model.h"
namespace ApiClient {
    void sendFeedback(const FeedbackModel &fb, const String &host, int port, const String &endpoint) {
        WiFiClient client;
        if (!client.connect(host.c_str(), port)) {
            Log.errorln("[REST] Connection failed to " + host + ":" + String(port));
            return;
        }
        DynamicJsonDocument doc(256);
        doc["id"] = fb.id;
        doc["feedback"] = fb.feedback;
        doc["timestamp"] = fb.timestamp;
        doc["timezone"] = fb.timezone;
        String requestBody;
        serializeJson(doc, requestBody);
        Log.infoln("[REST] Sending JSON: " + requestBody);
        client.println("POST " + endpoint + " HTTP/1.1");
        client.println("Host: " + host);
        client.println("Content-Type: application/json");
        client.print("Content-Length: ");
        client.println(requestBody.length());
        client.println();
        client.println(requestBody);
        while (client.connected()) {
            String line = client.readStringUntil('\n');
            if (line == "\r") break; 
        }
        String response = client.readString();
        Log.infoln("[REST] Response: " + response);
        if (response.indexOf("200") != -1) {
            Log.infoln("[REST] Response: OK (200)");
        } else if (response.indexOf("400") != -1) {
            Log.errorln("[REST] Response: BAD_REQUEST (400)");
        } else if (response.indexOf("500") != -1) {
            Log.errorln("[REST] Response: INTERNAL_SERVER_ERROR (500)");
        }
    }
}
#endif
*/
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