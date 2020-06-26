/*
  Name:    WifiProp.h
  Author:  Faure Systems <dev at faure dot systems>
  Editor:  https://github.com/fauresystems
  License: MIT License (c) Faure Systems <dev at faure dot systems>

  Prop for Arduino with WiFiNINA.
*/
#ifndef WIFIPROP_H
#define WIFIPROP_H

#if defined(ARDUINO_ARCH_STM32)
// Don't compile Arduino WiFi libraries when compiling for STM32 ARCH
#else

#if defined(ARDUINO_AVR_UNO_WIFI_REV2) || defined(ARDUINO_SAMD_MKRWIFI1010)  || defined(ARDUINO_SAMD_NANO_33_IOT)  || defined(ARDUINO_SAMD_MKRVIDOR4000)


#include <WiFiNINA.h>

#include <SPI.h>
#include <WiFiClient.h>
#include "Prop.h"

class WifiProp : public Prop
{
  public:
    WifiProp(const char*, const char*, const char*, const char*, const int);
    void begin(void(*)(String) = NULL);

private:
    WiFiClient _wifiClient;
};

#endif
#endif
#endif
