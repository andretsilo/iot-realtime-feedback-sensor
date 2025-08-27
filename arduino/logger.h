/*
#ifndef LOGGER_H
#define LOGGER_H
#include <ArduinoLog.h>
namespace Logger {
    void init() {
        Log.begin(LOG_LEVEL_VERBOSE, &Serial);
    }
}
#endif
*/
#ifndef LOGGER_H
#define LOGGER_H
#include <Arduino.h>
#include <ArduinoLog.h>
namespace Logger {
  inline void init(unsigned long baud = 115200) {
    Serial.begin(baud);
    while (!Serial) 
    Log.begin(LOG_LEVEL_VERBOSE, &Serial);
    Log.infoln("[Logger] Started");
  }
}

#endif