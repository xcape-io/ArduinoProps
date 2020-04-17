# *Prop* class reference
See also:
* <a href="PropData.md" target="_blank">PropData</a> class reference
* <a href="PropAction.md" target="_blank">PropAction</a> class reference

*Prop* (see <a href="https://github.com/xcape-io/ArduinoProps/blob/master/src/Prop.h" target="_blank">`Prop.h`</a>) is an interface (base class) implemented by 3 classes:
* ***BridgeProp*** for Yun board and Yun shield
* ***EthernetProp*** for Ethernet shield
* ***WifiProp*** for WiFiNINA boards

## Constructors
* `BridgeProp(const char*, const char*, const char*, const char*, const int)`
* `EthernetProp(const char*, const char*, const char*, const char*, const int)`
* `WifiProp(const char*, const char*, const char*, const char*, const int)`

Each constructor implement the ***Prop*** interface:
* `Props(const char* client_id, const char* in_box, const char* out_box, const char* broker, const int port)`
```csharp
BridgeProp prop(u8"Arduino Blink", // as MQTT client id, should be unique per client for given broker
                u8"Room/My room/Props/Arduino Blink/inbox",
                u8"Room/My room/Props/Arduino Blink/outbox",
                "192.168.1.53", // your MQTT server IP address
                1883); // your MQTT server port;
```


## Interface
* `virtual void begin(void(*)(String) = NULL) = 0`
* `void addData(PropData*)`
* `void loop()`
* `void resetIntervals(const int changes, const int silent)`
* `void resetMcu()`
* `void sendAllData()`
* `void sendDataChanges()`
* `void sendData(String variables)`
* `void sendDone(String action)`
* `void sendMesg(String message)`
* `void sendMesg(String topic, char* message)`
* `void sendOmit(String action)`
* `void sendOver(String challenge)`
* `void sendProg(String program)`
* `void sendRequ(String request)`
* `void sendRequ(String topic, char*)`

For `send----()` methods see <a href="../ArduinoProps#4-application-protocol-for-escape-room-20-prop" target="_blank">Application protocol for escape room 2.0 prop</a>
    
*fetch()* always returns the `"var=value "` string ready to be sent in a `DATA` protocol message and update the `_previous` value.

*fetchChange()* checks whether a change has occurred or not. If a change occured, returns the `"var=value "` string ready to be sent in a `DATA` protocol message and update the `_previous` value. If no change occured, returns and empty string.

## Public properties
* `const char *clientId`
* `const char *inbox`
* `const char *outbox`

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