/*
  Name:    Prop.h
  Author:  Faure Systems <dev at faure dot systems>
  Editor:  https://github.com/fauresystems
  License: MIT License (c) Faure Systems <dev at faure dot systems>

  Base class to make prop sketch for Escape Room 2.0 (connected).
*/
#ifndef PROP_H
#define PROP_H

#if defined(ARDUINO_ARCH_STM32)
// tested with STM32F767ZI
#else

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <BridgeClient.h>

#endif

#include <PubSubClient.h>
#include <ListLib.h>
#include "PropAction.h"
#include "PropData.h"

#ifndef MQTT_MAX_PACKET_SIZE
#define MQTT_MAX_PACKET_SIZE 128 // Bridge tranfer stuff (see PubSubClient)
#endif

class PropCallback
{
  public:
    static PubSubClient *client;
    static const char *outbox;
    static void run(char* topic, byte* payload, unsigned int len);
};

class PropMessageReceived
{
  public:
    static PubSubClient *client;
    static const char *outbox;
    static void run(String);
};

class InboxMessage : public PropMessageReceived
{
  public:
    static void run(String); // to be implemented in .ino sketch
};

class Prop
{
  public:
    Prop(const char*, const char*, const char*, const char*, const int=1883);
    virtual void begin(void(*)(String) = NULL) = 0;
    void addData(PropData*);
    void loop();
    void resetIntervals(const int changes, const int silent);
    void resetMcu();
    void sendAllData();
    void sendDataChanges();
    void sendData(String); // only in outbox
    void sendDone(String);
    void sendMesg(String);
    void sendMesg(String, char*);
    void sendOmit(String);
    void sendOver(String);
    void sendProg(String);
    void sendRequ(String);
    void sendRequ(String, char*);

    static void (*onInboxMessageReceived)(String);

    const char *clientId;
    const char *inbox;
    const char *outbox;

  protected:
    void checkDataChanges();
    void send(String*, String*);

    IPAddress _brokerIpAddress;
    PubSubClient _client;

    uint16_t _dataSentCount;
    uint8_t _maximumSilentPeriod;
    unsigned long _nextReconAttempt;
    uint8_t _payloadMax;
    PropAction _sendDataAction;
    List<PropData*> _dataTable;
};

#endif
