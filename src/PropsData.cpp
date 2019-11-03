/*
  Name:		PropsData.cpp
  Created: 29/10/2019 09:20:31
  Author:  Marie Faure <dev at faure dot systems>
  Editor:  https://github.com/fauresystems
  License: MIT License (c) Marie Faure <dev at faure dot systems>

  Variable wrapper for connected props.
*/

#include "PropsData.h"
#include <String.h>

// ---- Decimal
PropsDataDecimal::PropsDataDecimal(const char * id, uint8_t digits, double precision, double initial) {
  _id = id;
  _current = initial;
  _previous = _current;
  _precision = precision;
  _digits = digits;
}

String PropsDataDecimal::fetch() {
  _previous = _current;
  return String(_id) + "=" + String(_current, _digits) + " ";
}

String PropsDataDecimal::fetchChange() {
  if (_current != _previous && _current && abs(_current - _previous) / _current >= _precision) {
    return fetch();
  }
  else if (_current != _previous && !_current && abs(_current - _previous) >= _precision) {
    return fetch();
  }
  return String();
}

void PropsDataDecimal::setValue(const double v) {
  _current = v;
}

double PropsDataDecimal::value() const {
  return _current;
}

// ---- Integer
PropsDataInteger::PropsDataInteger(const char * id, double precision, long initial) {
  _id = id;
  _current = initial;
  _previous = _current;
  _precision = precision;
}

String PropsDataInteger::fetch() {
  _previous = _current;
  return String(_id) + "=" + String(_current) + " ";
}

String PropsDataInteger::fetchChange() {
  if (_current != _previous && _current && abs(_current - _previous) / _current >= _precision) {
    return fetch();
  }
  else if (_current != _previous && !_current && abs(_current - _previous) >= _precision) {
    return fetch();
  }
  return String();
}

void PropsDataInteger::setValue(const long v) {
  _current = v;
}

long PropsDataInteger::value() const {
  return _current;
}

// ---- Logical
PropsDataLogical::PropsDataLogical(const char * id, const char *trueval, const char *falseval, bool initial) {
  _id = id;
  _current = initial;
  _previous = _current;
  _trueString = trueval;
  _falseString = falseval;
}

String PropsDataLogical::fetch() {
  String str(String(_id) + "=");
  if (_current) {
    str += _trueString ? _trueString : "1";
  } else {
    str += _falseString ? _falseString : "0";
  }
  str += " ";
  _previous = _current;
  return str;
}

String PropsDataLogical::fetchChange() {
  if (_current != _previous) {
    return fetch();
  }
  return String();
}

void PropsDataLogical::setValue(const bool v) {
  _current = v;
}

bool PropsDataLogical::value() const {
  return _current;
}

// ---- Text
PropsDataText::PropsDataText(const char * id) {
  _id = id;
}

String PropsDataText::fetch() {
  _previous = _current;
  return String(_id) + "=" + _current + " ";
}

String PropsDataText::fetchChange() {
  if (_current != _previous) {
    return fetch();
  }
  return String();
}

void PropsDataText::setValue(String v) {
  _current = v;
}

String PropsDataText::value() const {
  return _current;
}
