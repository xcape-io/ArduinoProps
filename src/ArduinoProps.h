/*
  Name:    ArduinoProps.h
  Created: 29/10/2019 09:20:31
  Author:  Marie Faure <dev at faure dot systems>
  Editor:  https://github.com/fauresystems
  License: MIT License (c) Marie Faure <dev at faure dot systems>

  Library to make prop sketch for Escape Room 2.0 (connected prop).
*/

#ifndef _ArduinoProps_h
#define _ArduinoProps_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "BridgeProp.h"
#include "EthernetProp.h"
#include "WifiProp.h"
#include "PropAction.h"
#include "PropData.h"

#endif
