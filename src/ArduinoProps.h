/*
  Name:    ArduinoProps.h
  Author:  Faure Systems <dev at faure dot systems>
  Editor:  https://github.com/fauresystems
  License: MIT License (c) Faure Systems <dev at faure dot systems>

  Library to make prop sketch for Escape Room 2.0 (connected prop).
*/

#ifndef _ArduinoProps_h
#define _ArduinoProps_h

#if !defined(STM32F4xx) && !defined(STM32F7xx) // tested with STM32F767


#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "BridgeProp.h"
#include "EthernetProp.h"
#include "WifiProp.h"

#else // !STM32F4xx && !STM32F7xx
#include <Arduino.h>
#endif // STM32F4

#include "PropAction.h"
#include "PropData.h"

#endif
