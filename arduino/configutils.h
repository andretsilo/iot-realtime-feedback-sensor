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