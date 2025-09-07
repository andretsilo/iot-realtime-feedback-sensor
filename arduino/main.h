/*#ifndef MAIN_H
#define MAIN_H
#include "model.h"
#include "feedback.h"
#include "Logger.h"
#include "ConfigUtils.h"
#include "ApiClient.h"
#include <NTPClient.h>
#include <WiFiUdp.h>
struct Sensor {
  int pinButton;
  int pinLed;
  FeedbackLevel fb;
  unsigned long lastPressed;
};
extern Sensor sensors[];         
extern const int NumSensors;     
extern NTPClient timeClient;    
extern String sensorId;        
extern int uptimeSec;            
extern int resetSec;             
String formatTimestamp() {
    String formatted = timeClient.getFormattedDate();
    int splitT = formatted.indexOf("T");
    String datePart = formatted.substring(0, splitT);
    String timePart = formatted.substring(splitT + 1, splitT + 9);
    unsigned long ms = millis() % 1000;
    String day = datePart.substring(8, 10);
    String month = datePart.substring(5, 7);
    String year = datePart.substring(0, 4);
    char buffer[40];
    sprintf(buffer, "%s.%s.%s %s.%03lu", 
            day.c_str(), month.c_str(), year.c_str(),
            timePart.c_str(), ms);
    return String(buffer);
}
void handleSensors() {
  for (int i = 0; i < NumSensors; i++) {
    if (digitalRead(sensors[i].pinButton) == HIGH) {
      unsigned long now = millis();
      if (now - sensors[i].lastPressed > (resetSec * 1000)) {
        sensors[i].lastPressed = now;
        Log.infoln("[Sensor] Button pressed");
        digitalWrite(sensors[i].pinLed, HIGH);
        delay(uptimeSec * 1000);
        digitalWrite(sensors[i].pinLed, LOW);
        String ts = formatTimestamp();
        FeedbackModel fb(sensorId, sensors[i].fb, ts, "+2"); 
        Log.infoln("[Data] Feedback generated");
        if (ConfigUtils::get("sensor.grpc.enabled", "0") == "1") {
            Log.infoln("[gRPC] Sending feedback (TODO)");
        } else {
            ApiClient::sendFeedback(fb,
                ConfigUtils::get("sensor.api.host"),
                ConfigUtils::get("sensor.api.port").toInt(),
                ConfigUtils::get("sensor.api.endpoint"));
        }
      }
    }
  }
}
#endif
*/
#ifndef MAIN_H
#define MAIN_H
#include <Arduino.h>
#include <NTPClient.h>
#include "feedback.h"
#include "model.h"
#include "Logger.h"
#include "ConfigUtils.h"
#include "ApiClient.h"
#include "GrpcClient.h"
struct Sensor {
  int pinButton;
  int pinLed;
  FeedbackLevel fb;
  unsigned long lastPressed;
};
extern Sensor sensors[];
extern const int NumSensors;
extern NTPClient timeClient;
extern unsigned long ledUptimeMs;   
extern unsigned long buttonResetMs; 
extern String sensorId;
inline String getTimezoneFromGPS() {
  String mock = ConfigUtils::get("sensor.tz.mock", "+2");
  if (mock.length() == 2 && (mock[0] == '+' || mock[0] == '-')) return mock; // +n
  if (mock.length() == 3 && (mock[0] == '+' || mock[0] == '-')) return mock; // +nn
  return "+2";
}
inline String formatTimestamp() {
  String iso = timeClient.getFormattedDate(); 
  int splitT = iso.indexOf('T');
  String datePart = iso.substring(0, splitT);
  String timePart = iso.substring(splitT + 1, splitT + 9); 
  unsigned long ms = millis() % 1000;
  String day = datePart.substring(8, 10);
  String mon = datePart.substring(5, 7);
  String yr  = datePart.substring(0, 4);
  char buf[40];
  sprintf(buf, "%s.%s.%s %s.%03lu", day.c_str(), mon.c_str(), yr.c_str(), timePart.c_str(), ms);
  return String(buf);
}
inline void sendFeedbackUnified(const FeedbackModel &fb) {
  if (ConfigUtils::get("sensor.grpc.enabled", "0") == "1") {
    GrpcClient::sendFeedback(fb); 
  } else {
    ApiClient::sendFeedback(
      fb,
      ConfigUtils::get("sensor.api.host", "localhost"),
      ConfigUtils::get("sensor.api.port", "80").toInt(),
      ConfigUtils::get("sensor.api.endpoint", "/sensor")
    );
  }
}
inline void handleSensors() {
  unsigned long now = millis();
  for (int i = 0; i < NumSensors; i++) {
    if (digitalRead(sensors[i].pinButton) == HIGH) {
      if (now - sensors[i].lastPressed >= buttonResetMs) {
        sensors[i].lastPressed = now;
        Log.infoln("[Sensor] Button pressed"); 
        digitalWrite(sensors[i].pinLed, HIGH);
        delay(ledUptimeMs);
        digitalWrite(sensors[i].pinLed, LOW);
        String ts  = formatTimestamp();
        String tz  = getTimezoneFromGPS();
        FeedbackModel fb(sensorId, sensors[i].fb, ts, tz);
        Log.infoln("[Data] Feedback generated");
        sendFeedbackUnified(fb);
      }
    }
  }
}
#endif
