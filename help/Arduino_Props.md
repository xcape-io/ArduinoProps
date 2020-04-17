# Introduction to Arduino Props

* [Arduino Props for Escape Room 2.0](#arduino-props-for-escape-room-20)
* [Escape room 2.0 constraints on prop](#escape-room-20-constraints-on-prop)
* [ArduinoProps library for escape room 2.0](#arduinoprops-library-for-escape-room-20)
* [Arduino YUN and YUN shields](#arduino-yun-and-yun-shields)
* [Arduino Ethernet and Wifi](#arduino-ethernet-and-wifi)
* [Compatible hardware](#compatible-hardware)


## Arduino Props for Escape Room 2.0

The *ArduinoProps library* for Arduino has been designed to build *escape room 2.0 prop*.

Specificities of prop for escape rooms:
- very high uptime (near 100%)
- fast problem fixing
- real-time (human scale) notification and control 
- fallback controls
- low Total Cost of Ownership

Benefits and specificities of the *ArduinoProps library*
- rely on standard MQTT messaging protocol for IoT
- simplify and ease Arduino coding
- constant sustaining of Arduino code (YUN)
- real-time notification, monitoring and control (human scale)

## Escape room 2.0 constraints on prop
Escape game are opened 7 days a week and particularly booked during weekends and usually until midnight. Therefore their uptime must be close to 100% and incidents must be worked-around immediately by the game master (manual commands).

Escape Room Props operation and performance impact directly escape game customer experience. Therefore notifications must be processed in human-scale real-time, automatically or manually by a non-technological user (the game master).

Escape Room Props TCO (Total Cost of Ownership) must be affordable by escape games (SOHO costs for small businesses), processing is usually supported by Arduino or Raspberry boards.

![TCO](tco.jpg)

IT professional 24/7 support must be avoided for cost reasons. Therefore prop code must be the most reliable.


## *ArduinoProps library* for escape room 2.0
[MQTT](http://mqtt.org/) is a machine-to-machine (M2M)/"Internet of Things" connectivity protocol. It was designed as an extremely lightweight publish/subscribe messaging transport.

*ArduinoProps library* relies on *[Nick O'Leary PubSubClient](https://github.com/knolleary/pubsubclient)*, a simple publish/subscribe messaging Arduino library. [PubSubClient](https://github.com/knolleary/pubsubclient) is a MQTT client with a few limitations:

- It can only publish QoS 0 messages. It can subscribe at QoS 0 or QoS 1.
- The maximum message size, including header, is 128 bytes by default. This is configurable via MQTT_MAX_PACKET_SIZE in PubSubClient.h.
- The keepalive interval is set to 15 seconds by default. This is configurable via MQTT_KEEPALIVE in PubSubClient.h.
- The client uses MQTT 3.1.1 by default. It can be changed to use MQTT 3.1 by changing value of MQTT_VERSION in PubSubClient.h.

How does *ArduinoProps library* use MQTT efficiently in escape rooms:

- Every prop has two MQTT topics assigned as inbox and outbox.
- More MQTT topics can be used to share game countdown, scenario or any common useful dynamic data for the escape game.
- Notifying the prop is done by publishing a message on its inbox topic.
- Other prop or software controllers can subscribe to the prop outbox topic to receive all its notifications.
- *ArduinoProps library* includes a ping process at application level, in LAN network the round-trip-delay is around 50 milliseconds.
- *ArduinoProps library* is defaulted to publish QoS 0 messages and to subscribe at QoS 1.

*ArduinoProps library* wraps [PubSubClient](https://github.com/knolleary/pubsubclient) to make Arduino code the most simple and readable for better sustainability.

## Arduino YUN and YUN shields
Arduino YUN is a Arduino with Leonardo MCU/GPIO and Linux SoC.

Arduino YUN shield and Dragino YUN shield are YUN Linux SoC to be plugged onto Arduino Uno, Mega or Due.

**Arduino YUN advantage is that you can upload AVR program from the Arduino IDE at any moment during the Props lifetime**:
- change sensor settings when mechanics is undermined
- tune whole challenge processing

Arduino YUN can be rebooted or its MCU resetted remotely with simple SSH commands. 

Moreover, SSH command can be used to remotely change the MQTT server IP address, *ArduinoProps library* read this address at sketch start.

Therefore, **Arduino YUN or YUN shield is the preferred solution for prop with Arduino boards** because it will be very easy to sustain over time and greatly facilitates fallback plans to ensure maximum uptime:
- **Arduino Yún**
- **Arduino Yún Rev 2**
- **Arduino Mega 2560 Rev3 + Dragino Yún Shield**
- **Elegoo Mega 2560 R3 + Dragino Yún Shield**
- **Arduino Uno R3 + Dragino Yún Shield**
- **Arduino Due + Dragino Yún Shield**


## Arduino Ethernet and Wifi
Arduino with Ethernet or Wifi shield is suitable to build an Escape Room Props, however the sketch must be uploaded with an USB cable, so it is not convenient for prop design and worse for prop in production, which can lead to an unexpected significant increase in TCO.


## Compatible hardware
*ArduinoProps library* supported harware:
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