/*
  Name:    EthernetProp.h
  Created: 29/10/2019 09:20:31
  Author:  Marie Faure <dev at faure dot systems>
  Editor:  https://github.com/fauresystems
  License: MIT License (c) Marie Faure <dev at faure dot systems>

  Class Props for Arduino with Ethernet shield.
*/
#ifndef ETHERNETPROP_H
#define ETHERNETPROP_H


#include <SPI.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include "Prop.h"

class EthernetProp : public Props
{
  public:
    EthernetProp(const char*, const char*, const char*, const char*, const int);
    void begin(void(*)(String) = NULL);

  private:
      EthernetClient _ethernetClient;
};

#endif
