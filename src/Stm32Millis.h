/*
  Name:		 Stm32Millis.h
  Author:  Faure Systems <dev at faure dot systems>
  Editor:  https://github.com/fauresystems
  License: MIT License (c) Faure Systems <dev at faure dot systems>

  Implement millis() missing in STM32duino.
  https://github.com/stm32duino/wiki/wiki/HardwareTimer-library
*/

#ifndef STM32MILLIS_H
#define STM32MILLIS_H

#if defined(ARDUINO_ARCH_STM32)

#include <Arduino.h>

class Stm32MillisClass {

  public:
    HardwareTimer *_timer;
    static unsigned long milliseconds;

    static void update(void) {
      ++Stm32MillisClass::milliseconds;
    }

    void begin() {

#if defined(TIM1)
      TIM_TypeDef *Instance = TIM1;
#else
      TIM_TypeDef *Instance = TIM2;
#endif

      // Instantiate HardwareTimer object. Thanks to 'new' instanciation, HardwareTimer is not destructed when setup() function is finished.
      _timer = new HardwareTimer(Instance);

      _timer->setOverflow(1000, MICROSEC_FORMAT);
      _timer->attachInterrupt(update);
      _timer->resume();
    }
};

extern Stm32MillisClass Stm32Millis;

#endif // STM32
#endif
