/*
  Name:    Stm32Nucleo144Prop.h
  Author:  Faure Systems <dev at faure dot systems>
  Editor:  https://github.com/fauresystems
  License: MIT License (c) Faure Systems <dev at faure dot systems>

  Prop for Arduino with Ethernet shield.
*/
#ifndef STM32NUCLEO144PROP_H
#define STM32NUCLEO144PROP_H


#include <LwIP.h>
#include <STM32Ethernet.h>
#include "Prop.h"

class Stm32Nucleo144Prop : public Prop
{
  public:
    Stm32Nucleo144Prop(const char*, const char*, const char*, const char*, const int);
    void begin(void(*)(String) = NULL);

  private:
      EthernetClient _ethernetClient;
};

#endif
