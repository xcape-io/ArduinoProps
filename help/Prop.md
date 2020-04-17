# *Prop* class reference

*Prop* is a base class for 3 connected prop, depending on their network interface:
* ***BridgeProp*** for Yun board and Yun shield
* ***EthernetProp*** for Ethernet shield
* ***WifiProp*** for WiFiNINA boards

*Prop* class reference:
1. [Define the *Prop*](#1-defines-the-prop)
    * Defining a *BridgeProp*
    * Defining an *EthernetProp*
    * Defining a *WifiProp*
2. [Setup the *Prop*](#2-setup-the-prop)
    * Change timings
3. [Call in *loop()*](#3-call-in-loop)
4. [Implement `InboxMessage::run()` callback](#4-implement-inboxmessagerun-callback)
5. [Update broker address from SSH (*BridgeProp*)](#5-update-broker-address-from-ssh-bridgeprop)
6. [Examples](#6-examples)
7. [Compatible](#7-compatible-hardware) hardware


## 1. Defines the *Prop*
Defining a Bridge (Yun), Ethernet or Wifi *Prop* is slighlty different.

### Defining a *BridgeProp*
```csharp
#include <Bridge.h>
#include "ArduinoProps.h"

// If you're running our Escape Room control software (Room 2.0) you have to respect
// prpos inbox/outbox syntax Room/[escape room name]/Props/[propsname]/inbox|outbox
// https://live-escape.net/go/room

BridgeProp prop(u8"Arduino Contrôleur", // as MQTT client id, should be unique per client for given broker
                  u8"Room/Demoniak/Props/Arduino Contrôleur/inbox",
                  u8"Room/Demoniak/Props/Arduino Contrôleur/outbox",
                  "192.168.1.42", // your MQTT server IP address
                  1883); // your MQTT server port;

PropDataLogical clignoter(u8"clignote", u8"oui", u8"non", true);
PropDataLogical led(u8"led");

void clignote(); // forward
PropAction clignoteAction = PropAction(1000, clignote);
```
Create *BridgeProp* instance with MQTT parameters, then create *PropData* instances and  *PropsActions*.

The *Bridge* instance is created by `#include <Bridge.h>`.

### Defining an *EthernetProp*
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

EthernetProp prop(u8"Arduino Contrôleur", // as MQTT client id, should be unique per client for given broker
                    u8"Room/Demoniak/Props/Arduino Contrôleur/inbox",
                    u8"Room/Demoniak/Props/Arduino Contrôleur/outbox",
                    "192.168.1.42", // your MQTT server IP address
                    1883); // your MQTT server port;

PropDataLogical clignoter(u8"clignote", u8"oui", u8"non", true);
PropDataLogical led(u8"led");

void clignote(); // forward
PropAction clignoteAction = PropAction(1000, clignote);
```
Create MAC address (which must be unique, it's the hardware ID in the netwprk).

Create *EthernetProp* instance with MQTT parameters, then create *PropData* instances and  *PropsActions*.

The *Ethernet* instance is created by `#include <Ethernet.h>`.

### Defining a *WifiProp*
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

WifiProp prop(u8"Arduino Contrôleur", // as MQTT client id, should be unique per client for given broker
                u8"Room/Demoniak/Props/Arduino Contrôleur/inbox",
                u8"Room/Demoniak/Props/Arduino Contrôleur/outbox",
                "192.168.1.42", // your MQTT server IP address
                1883); // your MQTT server port;

PropDataLogical clignoter(u8"clignote", u8"oui", u8"non", true);
PropDataLogical led(u8"led");

void clignote(); // forward
PropAction clignoteAction = PropAction(1000, clignote);
```
The *AduinoProps* library uses **<a href="https://github.com/arduino-libraries/WiFiNINA" target="_blank">WiFiNINA</a>** WiFi library for **Arduino Uno WiFi Rev 2**, **Arduino NANO 33 IoT**, **Arduino MKR WiFi 1010** and **Arduino MKR VIDOR 4000**. 

Please update the WiFiNINA firmware: [WiFiNINA firmware update](WifiNinaFirmware.md).

Create *WifiProp* instance with MQTT parameters, then create *PropData* instances and  *PropsActions*.

#### MQTT topics
The prop listens to the *inbox* topic for command messages and send data and messages into the *outbox* topic. 

If you're running Live Escape Room 2.0 control software, you have to respect *inbox*/*outbox* syntax:

Room 2.0 *inbox* topic:
```csharp
format:  Room/[escape room name]/Props/[prop name]/inbox
example: u8"Room/Demoniak/Props/Arduino Contrôleur/inbox"
```
Room 2.0 *outbox* topic:
```csharp
format:  Room/[escape room name]/Props/[prop name]/outbox
example: u8"Room/Demoniak/Props/Arduino Contrôleur/outbox"
```
See [README.md](../README.md): **4. Application protocol for escape room 2.0 prop**.

#### *PropData* instances
*PropData* provides a facility to monitor data sent into MQTT outbox.

See [PropData.md](PropData.md) for *PropData* reference.

#### *PropAction* instances
To maintain a non-blocking asynchronous behavior of your prop, use *PropAction* objects to trigger action periodically.

Reading I/O and simple computing (arithmetics) can be done at every sketch loop but long computing have to run periodically.

For example, to blink a led, don't block the skecth with a `delay(1000)` call to switch the output: instead, ***check if 1 second is elapsed to switch the output***.

See [PropAction.md](PropData.md) for *PropAction* reference.


## 2. Setup the *Prop*
`InboxMessage::run` is the callback executed when a message is received in the MQTT inbox.
```csharp
void setup()
{
  // CODE TO BEGIN THE INTERNET CONNECTION
  ... see below the different code for BridgeProp, EthenetProps and WifiProp

  // SAME CODE FOR ALL PROPS
  prop.addData(&clignoter);
  prop.addData(&led);

  prop.begin(InboxMessage::run);

  pinMode(LED_BUILTIN, OUTPUT); // initialize digital pin LED_BUILTIN as an output

  // At this point, the broker is not connected yet
}

```

*BridgeProp*, *EthenetProps* and *WifiProp* has the exactly same setup:

- add *PropData* references to the prop
- call `prop.begin(InboxMessage::run)`

The setup code is slighlty different to start the network client (bridge, ethernet of wifi):
```csharp
void setup()
{
#if defined(BRIDGEPROP_H)
  Bridge.begin();
  updateBrokerAdressFromFile("/root/broker", &prop); // if you're running our Escape Room control software (Room 2.0)

#elif defined(ETHERNETPROP_H)
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

#elif defined(WIFIPROP_H)  
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
* *Prop* remains silent for up to 30 seconds

Timings can be changed anywhere in the code with `resetIntervals()`:
```csharp
  prop.resetIntervals(250, 15); // check data changes every 250 milliseconds, silent up to 15 seconds
```


## 3. Call in *loop()*
```csharp
void loop()
{
  prop.loop();

  led.setValue(digitalRead(LED_BUILTIN)); // read I/O

  clignoteAction.check(); // do your stuff, don't freeze the loop with delay() calls
}

```
Couldn't be more simple!

Just call `prop.loop()` when entering the sketch `loop()`, it will maintain MQTT broker connection and run callback when receiving MQTT messages.


## 4. Implement `InboxMessage::run()` callback
`InboxMessage::run` is the callback executed when a message is received in the MQTT inbox, you have to implement it.

Example for Room 2.0 commands:
```csharp
void InboxMessage::run(String a) {

  if (a == u8"app:startup")
  {
    prop.sendAllData();
    prop.sendDone(a);
  }
  else if (a == "clignoter:1")
  {
    clignoter.setValue(true);

    prop.sendAllData(); // all data change, we don't have to be selctive then
    prop.sendDone(a); // acknowledge prop command action
  }
  else if (a == "clignoter:0")
  {
    clignoter.setValue(false);

    prop.sendAllData(); // all data change, we don't have to be selctive then
    prop.sendDone(a); // acknowledge prop command action
  }
  else
  {
    // acknowledge omition of the prop command
    prop.sendOmit(a);
  }
}
```


## 5. Update broker address from SSH (*BridgeProp*)
Room 2.0 use a SSH command to change remotely the broker IP address (in /root/broker file) and to relaunch to sketch:
```bash
ssh 'echo 192.168.1.14> /root/broker && reset-mcu'
```
On the *BridgeProp* you can read the broker IP address from Linino filesystem with this code:
```csharp
void updateBrokerAdressFromFile(const char* broker_file, BridgeProp* prop)
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

  if (ip.fromString(b.c_str())) prop->setBrokerIpAddress(ip);
}
```


## 6. Examples
The library comes with a number of example sketches. See **File > Examples > ArduinoProps** within the Arduino IDE application.

See [EXAMPLES.md](../EXAMPLES.md) for an adaptation of the Blink example (https://www.arduino.cc/en/tutorial/blink) as a simple MQTT prop:

1. **BlinkOnBridgeProps**: the Blink example on a Yun prop with *ArduinoProps library*
2. **BlinkOnEthernetProps**: the Blink example on an Ethernet prop with *ArduinoProps library*
3. **BlinkOnWifiProps**: the Blink example on a Wifi prop with *ArduinoProps library*
4. **BlinkOnBridgePubSub**: the Blink example on prop using *PubSubClient* directly


## 7. Compatible hardware
*Props library* supported harware:
 - Arduino Yún and Arduino Yún Rev 2
 - Any Arduino + Dragino Yún Shield (tested with Elegoo Mega 2560 R3)
 - Arduino Uno WiFi Rev 2
 - Arduino NANO 33 IoT
 - Arduino MKR WiFi 1010
 - Arduino MKR VIDOR 4000
 - Arduino Ethernet
 - Arduino Ethernet Shield

For WiFi prop, please update the WiFiNINA firmware: [WiFiNINA firmware update](WifiNinaFirmware.md).

To use other WiFi hardware compatible with any *<a href="https://github.com/knolleary/pubsubclient" target="_blank">Nick O'Leary PubSubClient</a>* library you have to fork `WifiProp.h` and `WifiProp.cpp` and to replace `WiFiNINA.h` whih appropriate WiFi library (`ESP8266WiFi.h` for example):
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
* web: <a href="https://faure.systems/" target="_blank">Faure Systems</a>