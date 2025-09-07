#ifndef EEPROM_UTILS_H
#define EEPROM_UTILS_H
#include <Arduino.h>
#include <EEPROM.h>
namespace EepromUtils {
  inline void writeString(int addr, const String &str) {
    for (size_t i = 0; i < str.length(); i++) {
      EEPROM.write(addr + i, (uint8_t)str[i]);
    }
    EEPROM.write(addr + str.length(), '\0');
    #if defined(ESP_PLATFORM) || defined(ARDUINO_ARCH_ESP32)
      EEPROM.commit();
    #endif
  }
  inline String readString(int addr) {
    String out;
    char c = 0; int i = 0;
    do {
      c = (char)EEPROM.read(addr + i);
      if (c == '\0' || i > 255) break;
      out += c; i++;
    } while (true);
    return out;
  }
}
#endif