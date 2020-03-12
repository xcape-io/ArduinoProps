/*
  Name:    BridgeProps.h
  Created: 29/10/2019 09:20:31
  Author:  Marie Faure <dev at faure dot systems>
  Editor:  https://github.com/fauresystems
  License: MIT License (c) Marie Faure <dev at faure dot systems>

  Class Props for Arduino Yun and Dragino Yun shield.
*/
#ifndef BRIDGEPROPS_H
#define BRIDGEPROPS_H

#include <BridgeClient.h>
#include <IPAddress.h>
#include "Props.h"

class BridgeProps : public Props
{
  public:
    BridgeProps(const char*, const char*, const char*, const char*, const int);
    void begin(void(*)(String) = NULL);
	void setBrokerIpAddress(IPAddress, uint16_t port = 1883);

  private:
    BridgeClient _bridgeClient;
};

#endif
