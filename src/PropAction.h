/*
  Name:		 PropAction.h
  Author:  Marie Faure <dev at faure dot systems>
  Editor:  https://github.com/fauresystems
  License: MIT License (c) Marie Faure <dev at faure dot systems>

  Provide an easy way of triggering functions at a set interval.
*/

#ifndef PROPACTION_H
#define PROPACTION_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

class Prop;

class PropAction {

  public:
    PropAction();
    PropAction(unsigned long interval, void (*function)());

    void reset(unsigned long interval);
    void disable();
    void enable();
    void check();
    bool tick();

    unsigned long getInterval();

  private:
    bool _active;
    unsigned long _previous;
    unsigned long _interval;
    void (*_execute)();
};

#endif
