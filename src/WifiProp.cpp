/*
  Name:    WifiProp.cpp
  Author:  Marie Faure <dev at faure dot systems>
  Editor:  https://github.com/fauresystems
  License: MIT License (c) Marie Faure <dev at faure dot systems>

  Prop for Arduino with WiFiNINA.
*/

#include "WifiProp.h"


#if defined(ARDUINO_AVR_UNO_WIFI_REV2) || defined(ARDUINO_SAMD_MKRWIFI1010)  || defined(ARDUINO_SAMD_NANO_33_IOT)  || defined(ARDUINO_SAMD_MKRVIDOR4000)


WifiProp::WifiProp(const char* client_id, const char* in_box, const char* out_box, const char* broker, const int port)
  : Prop(client_id, in_box, out_box, broker, port)
{
    _client.setClient(_wifiClient);
}

void WifiProp::begin(void(*on_message)(String))
{
    if (on_message) onInboxMessageReceived = on_message;
}


#endif
