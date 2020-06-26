/*
  Name:    ArduinoProps.h
  Author:  Faure Systems <dev at faure dot systems>
  Editor:  https://github.com/fauresystems
  License: MIT License (c) Faure Systems <dev at faure dot systems>

  Library to make prop sketch for Escape Room 2.0 (connected prop).
*/

#ifndef _ArduinoProps_h
#define _ArduinoProps_h

#if defined(ARDUINO_ARCH_STM32)
// tested with STM32F767ZI
#include <Arduino.h>
#include "Stm32Nucleo144Prop.h"

#else

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "BridgeProp.h"
#include "EthernetProp.h"
#include "WifiProp.h"

#endif

#include "PropAction.h"
#include "PropData.h"

#endif
