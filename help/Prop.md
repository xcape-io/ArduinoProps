# *Prop* class reference
See also:
* <a href="https://github.com/xcape-io/ArduinoProps/blob/master/help/ArduinoProps_sketch.md#arduinoprops-sketch" target="_blank">ArduinoProps sketch</a>
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
The *Prop* class interface is consistent with the <a href="https://github.com/xcape-io/PyProps/blob/master/help/Prop.md" target="_blank">Prop class</a> of the <a href="https://github.com/xcape-io/PyProps#pyprops-library" target="_blank">PyProps library</a> for Raspberry boards.

* `virtual void begin(void(*)(String) = NULL) = 0`
    - the pure virtual `begin()` sets the network client
* `void addData(PropData*)`
    -  registers a <a href="PropData.md" target="_blank">PropData</a> instance to be treated by `sendAllData()` and `sendDataChanges()` methods
* `void loop()`
    - is the method to be called in the sketch *loop()*
* `void resetIntervals(const int changes, const int silent)`
* `void resetMcu()`
    - restarts the sketch (like the reset button)
* `void sendAllData()`
    - sends the `DATA` message for all registered data
* `void sendDataChanges()`
    - sends the `DATA` message for all registered data that value has changed since last call
* `void sendData(String variables)`
    - send the `data` string in a `DATA` message 
* `void sendDone(String action)`
    - send the `action` string in a `DONE` message  
* `void sendMesg(String message)`
    - send the `message` string in a `MESG` message to the *outbox* 
* `void sendMesg(String topic, char* message)`
    - send the `message` string in a `MESG` message to the `topic` parameter
* `void sendOmit(String action)`
    - send the `action` string in a `OMIT` message  
* `void sendOver(String challenge)`
    - send the `challenge` string in a `OVER` message  
* `void sendProg(String program)`
    - send the `program` string in a `PROG` message  
* `void sendRequ(String request)`
    - send the `request` string in a `REQU` message to the *outbox*
* `void sendRequ(String topic, char*)`
    - send the `request` string in a `REQU` message to the `topic` parameter

For `send----()` methods see *<a href="https://github.com/xcape-io/ArduinoProps#4-application-protocol-for-escape-room-20-prop" target="_blank">Application protocol for escape room 2.0 prop</a>*


## Author

**Marie FAURE** (Oct 18th, 2019)
* company: FAURE SYSTEMS SAS
* mail: *dev at faure dot systems*
* github: <a href="https://github.com/fauresystems?tab=repositories" target="_blank">fauresystems</a>
* web: <a href="https://faure.systems/" target="_blank">Faure Systems</a>