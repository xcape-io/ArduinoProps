/*
  Name:		PropData.cpp
  Author:  Marie Faure <dev at faure dot systems>
  Editor:  https://github.com/fauresystems
  License: MIT License (c) Marie Faure <dev at faure dot systems>

  Variable wrapper for connected prop.
*/

#include "PropData.h"
#include <String.h>

// ---- Decimal
PropDataDecimal::PropDataDecimal(const char * id, uint8_t digits, double precision, double initial) {
  _id = id;
  _current = initial;
  _previous = _current;
  _precision = precision;
  _digits = digits;
}

String PropDataDecimal::fetch() {
  _previous = _current;
  return String(_id) + "=" + String(_current, _digits) + " ";
}

String PropDataDecimal::fetchChange() {
  if (_current != _previous && _current && abs(_current - _previous) / _current >= _precision) {
    return fetch();
  }
  else if (_current != _previous && !_current && abs(_current - _previous) >= _precision) {
    return fetch();
  }
  return String();
}

void PropDataDecimal::setValue(const double v) {
  _current = v;
}

double PropDataDecimal::value() const {
  return _current;
}

// ---- Integer
PropDataInteger::PropDataInteger(const char * id, double precision, long initial) {
  _id = id;
  _current = initial;
  _previous = _current;
  _precision = precision;
}

String PropDataInteger::fetch() {
  _previous = _current;
  return String(_id) + "=" + String(_current) + " ";
}

String PropDataInteger::fetchChange() {
  if (_current != _previous && _current && abs(_current - _previous) / _current >= _precision) {
    return fetch();
  }
  else if (_current != _previous && !_current && abs(_current - _previous) >= _precision) {
    return fetch();
  }
  return String();
}

void PropDataInteger::setValue(const long v) {
  _current = v;
}

long PropDataInteger::value() const {
  return _current;
}

// ---- Logical
PropDataLogical::PropDataLogical(const char * id, const char *trueval, const char *falseval, bool initial) {
  _id = id;
  _current = initial;
  _previous = _current;
  _trueString = trueval;
  _falseString = falseval;
}

String PropDataLogical::fetch() {
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

String PropDataLogical::fetchChange() {
  if (_current != _previous) {
    return fetch();
  }
  return String();
}

void PropDataLogical::setValue(const bool v) {
  _current = v;
}

bool PropDataLogical::value() const {
  return _current;
}

// ---- Text
PropDataText::PropDataText(const char * id) {
  _id = id;
}

String PropDataText::fetch() {
  _previous = _current;
  return String(_id) + "=" + _current + " ";
}

String PropDataText::fetchChange() {
  if (_current != _previous) {
    return fetch();
  }
  return String();
}

void PropDataText::setValue(String v) {
  _current = v;
}

String PropDataText::value() const {
  return _current;
}
