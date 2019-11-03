/*
  Name:		PropsData.h
  Created: 29/10/2019 09:20:31
  Author:  Marie Faure <dev at faure dot systems>
  Editor:  https://github.com/fauresystems
  License: MIT License (c) Marie Faure <dev at faure dot systems>

  Variable wrapper for connected props.
*/
#ifndef PROPSDATA_H
#define PROPSDATA_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <inttypes.h>
#include <WString.h>

class PropsData
{
  public:
    virtual String fetch() = 0;
    virtual String fetchChange() = 0;
};

class PropsDataDecimal : public PropsData
{
  public:
    PropsDataDecimal(const char *, uint8_t digits = 2, double precision = 0.1, double initial = 0);
    String fetch();
    String fetchChange();
    void setValue(const double);
    double value() const;
  private:
    const char *_id;
    double _current;
    double _previous;
    double _precision;
    uint8_t _digits;
};

class PropsDataInteger : public PropsData
{
  public:
    PropsDataInteger(const char *, double precision = 0.1, long initial = 0);
    String fetch();
    String fetchChange();
    void setValue(const long);
    long value() const;
  private:
    const char *_id;
    long _current;
    long _previous;
    double _precision;
};

class PropsDataLogical : public PropsData
{
  public:
    PropsDataLogical(const char *, const char *trueval = NULL, const char *falseval = NULL, bool initial = false);
    String fetch();
    String fetchChange();
    void setValue(const bool);
    bool value() const;
  private:
    const char *_id;
    bool _current;
    bool _previous;
    const char *_trueString;
    const char *_falseString;
};

class PropsDataText : public PropsData
{
  public:
    PropsDataText(const char *);
    String fetch();
    String fetchChange();
    void setValue(String);
    String value() const;
  private:
    const char *_id;
    String _current;
    String _previous;
};

#endif
