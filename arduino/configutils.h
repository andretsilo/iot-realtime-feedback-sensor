
/*
#ifndef CONFIGUTILS_H
#define CONFIGUTILS_H
#include <Arduino.h>
#include <SD.h>
#include <map>
namespace ConfigUtils {
    std::map<String, String> props;
    bool loadProperties(const char* filename) {
        File file = SD.open(filename);
        if (!file) {
            Serial.println("[Config] Failed to open properties file!");
            return false;
        }
        while (file.available()) {
            String line = file.readStringUntil('\n');
            line.trim();
            if (line.length() == 0 || line.startsWith("#")) continue;
            int eq = line.indexOf('=');
            if (eq > 0) {
                String key = line.substring(0, eq);
                String value = line.substring(eq + 1);
                key.trim();
                value.trim();
                props[key] = value;
            }
        }
        file.close();
        return true;
    }
    String get(const String& key, const String& defaultValue = "") {
        if (props.count(key)) return props[key];
        return defaultValue;
    }
}
#endif
*/
#ifndef CONFIG_UTILS_H
#define CONFIG_UTILS_H
#include <Arduino.h>
#include <SD.h>
#include <map>
namespace ConfigUtils {
  static std::map<String, String> props;
  inline bool load(const char* path = "/config/sensor.properties") {
    props.clear();
    File f = SD.open(path);
    if (!f) {
      Serial.println("[Config] Cannot open properties file");
      return false;
    }
    while (f.available()) {
      String line = f.readStringUntil('\n');
      line.trim();
      if (line.length() == 0 || line.startsWith("#")) continue;
      int eq = line.indexOf('=');
      if (eq <= 0) continue;
      String k = line.substring(0, eq); k.trim();
      String v = line.substring(eq + 1); v.trim();
      props[k] = v;
    }
    f.close();
    return true;
  }
  inline String get(const String& key, const String& def = "") {
    auto it = props.find(key);
    if (it != props.end()) return it->second;
    return def;
  }
}
#endif 