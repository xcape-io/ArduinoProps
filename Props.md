# *Props* class reference

*Props* is a base class for 3 connected props, depending on their network interface:
* ***BridgeProps*** for Yun board and Yun shield
* ***EthernetProps*** for Ethernet shield
* ***WifiProps*** for Wifi shield

*Props* class reference:
1. Define the *Props*
    * Defining a *BridgeProps*
    * Defining an *EthernetProps*
    * Defining a *WifiProps*
2. Setup the *Props*
    * Change timings
3. Call in sketch *loop()*
4. Implement `InboxMessage::run()` callback
5. Update broker address from SSH (*BridgeProps*)
6. Examples
7. Compatible hardware


## 1. Defines the *Props*
Defining a Bridge (Yun), Ethernet or Wifi *Props* is slighlty different.

### Defining a *BridgeProps*
```csharp
#include <Bridge.h>
#include "ArduinoProps.h"

// If you're running our Escape Room control software (Room 2.0) you have to respect
// prpos inbox/outbox syntax Room/[escape room name]/Props/[propsname]/inbox|outbox
// https://live-escape.net/go/room

BridgeProps props(u8"Arduino Contrôleur", // as MQTT client id, should be unique per client for given broker
                  u8"Room/Demoniak/Props/Arduino Contrôleur/inbox",
                  u8"Room/Demoniak/Props/Arduino Contrôleur/outbox",
                  "192.168.1.42", // your MQTT server IP address
                  1883); // your MQTT server port;

PropsDataLogical clignoter(u8"clignote", u8"oui", u8"non", true);
PropsDataLogical led(u8"led");

void clignote(); // forward
PropsAction clignoteAction = PropsAction(1000, clignote);
```
Create *BridgeProps* instance with MQTT parameters, then create *PropsData* instances and  *PropsActions*.

The *Bridge* instance is created by `#include <Bridge.h>`.

### Defining an *EthernetProps*
```csharp
#include <Ethernet.h>
#include <IPAddress.h>
#include "ArduinoProps.h"

byte mac[] = { 0xFC, 0xFA, 0xEA, 0x00, 0x00, 0x02 }; //<<< MAKE SURE IT'S UNIQUE IN YOUR NETWORK!!!
String ip = "192.168.1.19"; //<<< ENTER YOUR IP ADDRESS HERE ("" for DHCP)

// Builtin led is not available with the shield
#undef LED_BUILTIN
#define LED_BUILTIN 8

// If you're running our Escape Room control software (Room 2.0) you have to respect
// prpos inbox/outbox syntax Room/[escape room name]/Props/[propsname]/inbox|outbox
// https://live-escape.net/go/room

EthernetProps props(u8"Arduino Contrôleur", // as MQTT client id, should be unique per client for given broker
                    u8"Room/Demoniak/Props/Arduino Contrôleur/inbox",
                    u8"Room/Demoniak/Props/Arduino Contrôleur/outbox",
                    "192.168.1.42", // your MQTT server IP address
                    1883); // your MQTT server port;

PropsDataLogical clignoter(u8"clignote", u8"oui", u8"non", true);
PropsDataLogical led(u8"led");

void clignote(); // forward
PropsAction clignoteAction = PropsAction(1000, clignote);
```
Create MAC address (which must be unique, it's the hardware ID in the netwprk).

Create *EthernetProps* instance with MQTT parameters, then create *PropsData* instances and  *PropsActions*.

The *Ethernet* instance is created by `#include <Ethernet.h>`.

### Defining a *WifiProps*
```csharp
#include "ArduinoProps.h"

// Setup your WiFi network
const char* ssid = "";
const char *passphrase = "";

// Builtin led is not available with the shield
#undef LED_BUILTIN
#define LED_BUILTIN 8

// If you're running our Escape Room control software (Room 2.0) you have to respect
// prpos inbox/outbox syntax Room/[escape room name]/Props/[propsname]/inbox|outbox
// https://live-escape.net/go/room

WifiProps props(u8"Arduino Contrôleur", // as MQTT client id, should be unique per client for given broker
                u8"Room/Demoniak/Props/Arduino Contrôleur/inbox",
                u8"Room/Demoniak/Props/Arduino Contrôleur/outbox",
                "192.168.1.42", // your MQTT server IP address
                1883); // your MQTT server port;

PropsDataLogical clignoter(u8"clignote", u8"oui", u8"non", true);
PropsDataLogical led(u8"led");

void clignote(); // forward
PropsAction clignoteAction = PropsAction(1000, clignote);
```
The *AduinoProps* library uses **<a href="https://github.com/arduino-libraries/WiFiNINA" target="_blank">WiFiNINA</a>** WiFi library for **Arduino Uno WiFi Rev 2**, **Arduino NANO 33 IoT**, **Arduino MKR 1010** and **Arduino MKR VIDOR 4000**. 

Create *WifiProps* instance with MQTT parameters, then create *PropsData* instances and  *PropsActions*.

#### MQTT topics
The props listens to the *inbox* topic for command messages and send data and messages into the *outbox* topic. 

If you're running Live Escape Room 2.0 control software, you have to respect *inbox*/*outbox* syntax:

Room 2.0 *inbox* topic:
```csharp
format:  Room/[escape room name]/Props/[props name]/inbox
example: u8"Room/Demoniak/Props/Arduino Contrôleur/inbox"
```
Room 2.0 *outbox* topic:
```csharp
format:  Room/[escape room name]/Props/[props name]/outbox
example: u8"Room/Demoniak/Props/Arduino Contrôleur/outbox"
```
See [README.md](README.md): **4. Application protocol for escape room 2.0 props**.

#### *PropsData* instances
*PropsData* provides a facility to monitor data sent into MQTT outbox.

See [PropsData.md](PropsData.md) for *PropsData* reference.

#### *PropsAction* instances
To maintain a non-blocking asynchronous behavior of your props, use *PropsAction* objects to trigger action periodically.

Reading I/O and simple computing (arithmetics) can be done at every sketch loop but long computing have to run periodically.

For example, to blink a led, don't block the skecth with a `delay(1000)` call to switch the output: instead, ***check if 1 second is elapsed to switch the output***.

See [PropsAction.md](PropsData.md) for *PropsAction* reference.


## 2. Setup the *Props*
`InboxMessage::run` is the callback executed when a message is received in the MQTT inbox.
```csharp
void setup()
{
  // CODE TO BEGIN THE INTERNET CONNECTION
  ... see below the different code for BridgeProps, EthenetProps and WifiProps

  // SAME CODE FOR ALL PROPS
  props.addData(&clignoter);
  props.addData(&led);

  props.begin(InboxMessage::run);

  pinMode(LED_BUILTIN, OUTPUT); // initialize digital pin LED_BUILTIN as an output

  // At this point, the broker is not connected yet
}

```

*BridgeProps*, *EthenetProps* and *WifiProps* has the exactly same setup:

- add *PropsData* references to the props
- call `props.begin(InboxMessage::run)`

The setup code is slighlty different to start the network client (bridge, ethernet of wifi):
```csharp
void setup()
{
#if defined(BRIDGEPROPS_H)
  Bridge.begin();
  updateBrokerAdressFromFile("/root/broker", &props); // if you're running our Escape Room control software (Room 2.0)

#elif defined(ETHERNETPROPS_H)
  // can do more static IP configuration
  //Ethernet.setSubnetMask(IPAddress());
  //Ethernet.setGatewayIP(IPAddress());
  //Ethernet.setDnsServerIP(IPAddress());

  IPAddress ipa;
  if (!ipa.fromString(ip)) {
    // attempt a DHCP connection:
    if (!Ethernet.begin(mac)) {
      // if DHCP fails, start with a hard-coded address:
      Ethernet.begin(mac, IPAddress(10, 90, 90, 239));
    }
  }
  else {
    Ethernet.begin(mac, ipa);
  }
  delay(1500); // time for shield stuff

#elif defined(WIFIPROPS_H)  
    // can do more static IP configuration
  //WiFi.config(IPAddress(), IPAddress(), IPAddress());
  //WiFi.config(IPAddress(), IPAddress());
  //WiFi.config(IPAddress());

  WiFi.begin(ssid, passphrase);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
#endif
  ...
}

```

### Change timings
Default timings are:
* check data changes every 400 milliseconds
* *Props* remains silent for up to 30 seconds

Timings can be changed anywhere in the code with `resetIntervals()`:
```csharp
  props.resetIntervals(250, 15); // check data changes every 250 milliseconds, silent up to 15 seconds
```


## 3. Call in sketch *loop()*
```csharp
void loop()
{
  props.loop();

  led.setValue(digitalRead(LED_BUILTIN)); // read I/O

  clignoteAction.check(); // do your stuff, don't freeze the loop with delay() calls
}

```
Couldn't be more simple!

Just call `props.loop()` when entering the sketch `loop()`, it will maintain MQTT broker connection and run callback when receiving MQTT messages.


## 4. Implement `InboxMessage::run()` callback
`InboxMessage::run` is the callback executed when a message is received in the MQTT inbox, you have to implement it.

Example for Room 2.0 commands:
```csharp
void InboxMessage::run(String a) {

  if (a == u8"app:startup")
  {
    props.sendAllData();
    props.sendDone(a);
  }
  else if (a == "clignoter:1")
  {
    clignoter.setValue(true);

    props.sendAllData(); // all data change, we don't have to be selctive then
    props.sendDone(a); // acknowledge props command action
  }
  else if (a == "clignoter:0")
  {
    clignoter.setValue(false);

    props.sendAllData(); // all data change, we don't have to be selctive then
    props.sendDone(a); // acknowledge props command action
  }
  else
  {
    // acknowledge omition of the props command
    props.sendOmit(a);
  }
}
```


## 5. Update broker address from SSH (*BridgeProps*)
Room 2.0 use a SSH command to change remotely the broker IP address (in /root/broker file) and to relaunch to sketch:
```bash
ssh 'echo 192.168.1.14> /root/broker && reset-mcu'
```
On the *BridgeProps* you can read the broker IP address from Linino filesystem with this code:
```csharp
void updateBrokerAdressFromFile(const char* broker_file, BridgeProps* props)
{
  // broker IP address is stored in Linino file systems and updated with ssh command by Room 2.0
  IPAddress ip;

  Process _process;
  _process.begin("cat");
  _process.addParameter(broker_file); // for ssh remotely set broker address
  _process.run(); // run the process and wait for its termination
  String b;
  while (_process.available() > 0) {
    char c = _process.read();
    b += c;
  }
  b.trim();

  if (ip.fromString(b.c_str())) props->setBrokerIpAddress(ip);
}
```


## 6. Examples
The library comes with a number of example sketches. See **File > Examples > ArduinoProps** within the Arduino IDE application.

See [EXAMPLES.md](EXAMPLES.md) for an adaptation of the Blink example (https://www.arduino.cc/en/tutorial/blink) as a simple MQTT props:

1. **BlinkOnBridgeProps**: the Blink example on a Yun props with *ArduinoProps library*
2. **BlinkOnEthernetProps**: the Blink example on an Ethernet props with *ArduinoProps library*
3. **BlinkOnWifiProps**: the Blink example on a Wifi props with *ArduinoProps library*
4. **BlinkOnBridgePubSub**: the Blink example on props using *PubSubClient* directly


## 7. Compatible hardware
*Props library* supported harware:
 - Arduino Yún and Arduino Yún Rev 2
 - Any Arduino + Dragino Yún Shield (tested with Elegoo Mega 2560 R3)
 - Arduino Uno WiFi Rev 2
 - Arduino NANO 33 IoT
 - Arduino MKR 1010
 - Arduino MKR VIDOR 4000
 - Arduino Ethernet
 - Arduino Ethernet Shield

To use other WiFi hardware compatible with any *<a href="https://github.com/knolleary/pubsubclient" target="_blank">Nick O'Leary PubSubClient</a>* library you have to fork `WifiProps.h` and `WifiProps.cpp` and to replace `WiFiNINA.h` whih appropriate WiFi library (`ESP8266WiFi.h` for example):
 - Arduino WiFi Shield
 - Sparkfun WiFly Shield – [library](https://github.com/dpslwk/WiFly)
 - TI CC3000 WiFi - [library](https://github.com/sparkfun/SFE_CC3000_Library)
 - Intel Galileo/Edison
 - ESP8266
 - ESP32


## Author

**Marie FAURE** (Oct 18th, 2019)
* company: FAURE SYSTEMS SAS
* mail: *dev at faure dot systems*
* github: <a href="https://github.com/fauresystems?tab=repositories" target="_blank">fauresystems</a>
* web: <a href="https://www.live-escape.net/" target="_blank">Live Escape Grenoble</a>