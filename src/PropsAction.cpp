/*
  Name:    PropsAction.h
  Created: 29/10/2019 09:20:31
  Author:  Marie Faure <dev at faure dot systems>
  Editor:  https://github.com/fauresystems
  License: MIT License (c) Marie Faure <dev at faure dot systems>

  Provide an easy way of triggering functions at a set _interval.
*/

#include "PropsAction.h"

PropsAction::PropsAction() 
{
  _active = true;
  _previous = 0;
  _interval = 400;
  _execute = NULL;
}

PropsAction::PropsAction(unsigned long intervl, void (*function)()) 
{
  _active = true;
  _previous = 0;
  _interval = intervl;
  _execute = function;
}

void PropsAction::reset(unsigned long intervl) 
{
  _active = true;
  _previous = 0;
  _interval = intervl;
}

void PropsAction::disable()
{
  _active = false;
}

void PropsAction::enable()
{
  _active = true;
}

void PropsAction::check() {
  if (_active && (millis() - _previous >= _interval))
  {
    _previous = millis();
    if (_execute) _execute();
  }
}

bool PropsAction::tick() {
  if (_active && (millis() - _previous >= _interval))
  {
    _previous = millis();
    return true;
  }
  return false;
}

unsigned long PropsAction::getInterval() 
{
  return _interval;
}
