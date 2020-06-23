/*
  Name:    BridgeProp.h
  Author:  Faure Systems <dev at faure dot systems>
  Editor:  https://github.com/fauresystems
  License: MIT License (c) Faure Systems <dev at faure dot systems>

  Prop for Arduino Yun and Dragino Yun shield.
*/
#ifndef BRIDGEPROP_H
#define BRIDGEPROP_H

#include <BridgeClient.h>
#include <IPAddress.h>
#include "Prop.h"

class BridgeProp : public Prop
{
  public:
    BridgeProp(const char*, const char*, const char*, const char*, const int);
    void begin(void(*)(String) = NULL);
    void setBrokerIpAddress(IPAddress, uint16_t port = 1883);

  private:
    BridgeClient _bridgeClient;
};

#endif
