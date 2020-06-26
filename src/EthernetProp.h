/*
  Name:    EthernetProp.h
  Author:  Faure Systems <dev at faure dot systems>
  Editor:  https://github.com/fauresystems
  License: MIT License (c) Faure Systems <dev at faure dot systems>

  Prop for Arduino with Ethernet shield.
*/
#ifndef ETHERNETPROP_H
#define ETHERNETPROP_H

#if defined(ARDUINO_ARCH_STM32)
// Don't compile Arduino Ethernet library when compiling for STM32 ARCH
#else

#include <SPI.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include "Prop.h"

class EthernetProp : public Prop
{
  public:
    EthernetProp(const char*, const char*, const char*, const char*, const int);
    void begin(void(*)(String) = NULL);

  private:
      EthernetClient _ethernetClient;
};

#endif
#endif
