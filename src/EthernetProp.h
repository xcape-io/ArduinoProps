/*
  Name:    EthernetProp.h
  Author:  Marie Faure <dev at faure dot systems>
  Editor:  https://github.com/fauresystems
  License: MIT License (c) Marie Faure <dev at faure dot systems>

  Prop for Arduino with Ethernet shield.
*/
#ifndef ETHERNETPROP_H
#define ETHERNETPROP_H


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
