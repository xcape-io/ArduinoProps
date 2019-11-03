/*
  Name:    Props.h
  Created: 29/10/2019 09:20:31
  Author:  Marie Faure <dev at faure dot systems>
  Editor:  https://github.com/fauresystems
  License: MIT License (c) Marie Faure <dev at faure dot systems>

  Base class to make props sketch for Escape Room 2.0 (connected).
*/
#ifndef PROPS_H
#define PROPS_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <BridgeClient.h>
#include <PubSubClient.h>
#include <SimpleList.h>
#include "PropsAction.h"
#include "PropsData.h"

#ifndef MQTT_MAX_PACKET_SIZE
#define MQTT_MAX_PACKET_SIZE 128 // Bridge tranfer stuff (see PubSubClient)
#endif

class PropsCallback
{
  public:
    static PubSubClient *client;
    static const char *outbox;
    static void run(char* topic, byte* payload, unsigned int len);
};

class PropsMessageReceived
{
  public:
    static PubSubClient *client;
    static const char *outbox;
    static void run(String);
};

class InboxMessage : public PropsMessageReceived
{
  public:
    static void run(String); // to be implemented in .ino sketch
};

class Props
{
  public:
    Props(const char*, const char*, const char*, const char*, const int=1883);
	virtual void begin(void(*)(String) = NULL) = 0;
    void addData(PropsData*);
    void loop();
	void resetIntervals(const int changes, const int silent);
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
    PropsAction _sendDataAction;
    SimpleList<PropsData*> _dataTable;
};

#endif
