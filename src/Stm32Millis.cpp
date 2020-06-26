/*
  Name:    Stm32Millis.cpp
  Author:  Faure Systems <dev at faure dot systems>
  Editor:  https://github.com/fauresystems
  License: MIT License (c) Faure Systems <dev at faure dot systems>

  Implement millis() missing in STM32duino.
*/
#if defined(ARDUINO_ARCH_STM32)

#include "Stm32Millis.h"

unsigned long Stm32MillisClass::milliseconds = 0;
Stm32MillisClass Stm32Millis;

#endif
