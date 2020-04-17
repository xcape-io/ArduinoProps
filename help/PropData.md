# *PropData* reference

*PropData* tracks data changes and provides escape room 2.0 propocol `DATA` ready string format.

See [**4. Application protocol for escape room 2.0 prop**](../README.md#4-application-protocol-for-escape-room-20-prop) in [README.md](../README.md).

*PropData* is an interface (base class) implemented by 4 classes:
* ***PropDataDecimal*** 
* ***PropDataInteger*** 
* ***PropDataLogical*** 
* ***PropDataText*** 

## Constructors
* `PropDataDecimal(const char *, uint8_t digits = 2, double precision = 0.1, double initial = 0)`
* `PropDataInteger(const char *, double precision = 0.1, long initial = 0)`
* `PropDataLogical(const char *, const char *trueval = NULL, const char *falseval = NULL, bool initial = false)`
* `PropDataText(const char *)`

By default for *PropDataLogical*, `trueval` is `"1"` band `falseval` is `"0"`.

For *PropDataDecimal*, `digits` is the number of decimal digits in the display string.

For *PropDataDecimal* and *PropDataInteger*, `precision` is the percentage threshold for deciding whether a change has occurred or not.

## Interface
* `String fetch()`
* `String fetchChange()`

*fetch()* always returns the `"var=value "` string ready to be sent in a `DATA` protocol message and update the `_previous` value.

*fetchChange()* checks whether a change has occurred or not. If a change occured, returns the `"var=value "` string ready to be sent in a `DATA` protocol message and update the `_previous` value. If no change occured, returns and empty string.

## Accessors and Mutators
* `void setValue(const double|long|bool|String)`
* `double|long|bool|String value() const`

Accessors and Mutators are typed according to *PropData* type.

## Usage

### Create a *PropData* instance
```csharp
PropDataLogical clignoter(u8"clignote", u8"oui", u8"non", true);
```

### Add the *PropData* instance to managed data of the *Props*
```csharp
void setup()
{
  ...

  Props::addManagedLogicalData(&clignoter);

  ...
}
```

### Use the *PropData* instance anywhere
```csharp
clignoter.setValue(true);

...

if (clignoter.value()) {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    led.setValue(digitalRead(LED_BUILTIN));
}
```

## Author

**Marie FAURE** (Oct 18th, 2019)
* company: FAURE SYSTEMS SAS
* mail: *dev at faure dot systems*
* github: <a href="https://github.com/fauresystems?tab=repositories" target="_blank">fauresystems</a>
* web: <a href="https://faure.systems/" target="_blank">Faure Systems</a>