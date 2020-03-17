/*
  Name:    WifiProps.h
  Created: 29/10/2019 09:20:31
  Author:  Marie Faure <dev at faure dot systems>
  Editor:  https://github.com/fauresystems
  License: MIT License (c) Marie Faure <dev at faure dot systems>

  Class Props for Arduino with WiFiNINA.
*/
#ifndef WIFIPROPS_H
#define WIFIPROPS_H


#if defined(ARDUINO_AVR_UNO_WIFI_REV2) || defined(ARDUINO_SAMD_MKRWIFI1010)  || defined(ARDUINO_SAMD_NANO_33_IOT)  || defined(ARDUINO_SAMD_MKRVIDOR4000)


#include <WiFiNINA.h>

#include <SPI.h>
#include <WiFiClient.h>
#include "Props.h"

class WifiProps : public Props
{
  public:
    WifiProps(const char*, const char*, const char*, const char*, const int);
    void begin(void(*)(String) = NULL);

private:
    WiFiClient _wifiClient;
};

#endif


#endif
