/*
  Name:	   Stm32Nucleo144Prop.cpp
  Author:  Faure Systems <dev at faure dot systems>
  Editor:  https://github.com/fauresystems
  License: MIT License (c) Faure Systems <dev at faure dot systems>

  Prop for Arduino with Ethernet shield.
*/

#include "Stm32Nucleo144Prop.h"

Stm32Nucleo144Prop::Stm32Nucleo144Prop(const char* client_id, const char* in_box, const char* out_box, const char* broker, const int port)
    : Prop(client_id, in_box, out_box, broker, port)
{
    _client.setClient(_ethernetClient);
}

void Stm32Nucleo144Prop::begin(void(*on_message)(String))
{
    if (on_message) onInboxMessageReceived = on_message;
}
