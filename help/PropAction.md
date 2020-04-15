# *PropAction* class reference

*PropAction* brings asynchronous-like behavior to sketch `loop()`.


## Constructors
* `PropAction()`
* `PropAction(unsigned long interval, void (*function)())`

`function` is the callback executed at every `interval` milliseconds.

## Members
* `void reset(unsigned long interval)`
* `void disable()`
* `void enable()`
* `void check()`
* `bool tick()`
* `unsigned long getInterval()`

## Usage

### Create a callback
```csharp
void clignote()
{
  if (clignoter.value()) {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    led.setValue(digitalRead(LED_BUILTIN));
  }
}
```

### Create a *PropAction* instance
```csharp
PropAction clignoteAction = PropAction(1000, clignote); // interval every 1000 milliseconds
```

### Call the instance `check()` method ine the sketch `loop()`
```csharp
void loop()
{
  ...

  clignoteAction.check(); // do your stuff, don't freeze the loop with delay() calls

  ...
}
```


## Author

**Marie FAURE** (Oct 18th, 2019)
* company: FAURE SYSTEMS SAS
* mail: *dev at faure dot systems*
* github: <a href="https://github.com/fauresystems?tab=repositories" target="_blank">fauresystems</a>
* web: <a href="https://faure.systems/" target="_blank">Faure Systems</a>