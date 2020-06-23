/*
  Name:    BridgeProp.cpp
  Author:  Faure Systems <dev at faure dot systems>
  Editor:  https://github.com/fauresystems
  License: MIT License (c) Faure Systems <dev at faure dot systems>

  Prop for Arduino Yun and Dragino Yun shield.
*/

#include "BridgeProp.h"

BridgeProp::BridgeProp(const char* client_id, const char* in_box, const char* out_box, const char* broker, const int port)
    : Prop(client_id, in_box, out_box, broker, port)
{
  _client.setClient(_bridgeClient);
}

void BridgeProp::begin(void(*on_message)(String))
{
  if (on_message) onInboxMessageReceived = on_message;
}

void BridgeProp::setBrokerIpAddress(IPAddress ip, uint16_t port)
{
    _brokerIpAddress = ip;
    _client.setServer(_brokerIpAddress, port);
}

