/*
  Name:	   EthernetProp.cpp
  Author:  Faure Systems <dev at faure dot systems>
  Editor:  https://github.com/fauresystems
  License: MIT License (c) Faure Systems <dev at faure dot systems>

  Prop for Arduino with Ethernet shield.
*/

#include "EthernetProp.h"

EthernetProp::EthernetProp(const char* client_id, const char* in_box, const char* out_box, const char* broker, const int port)
    : Prop(client_id, in_box, out_box, broker, port)
{
    _client.setClient(_ethernetClient);
}

void EthernetProp::begin(void(*on_message)(String))
{
    if (on_message) onInboxMessageReceived = on_message;
}
