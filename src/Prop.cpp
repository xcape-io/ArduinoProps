/*
  Name:    Prop.cpp
  Author:  Faure Systems <dev at faure dot systems>
  Editor:  https://github.com/fauresystems
  License: MIT License (c) Faure Systems <dev at faure dot systems>

  Base class to make prop sketch for Escape Room 2.0 (connected).
*/

#include "Prop.h"
//#include <Process.h>
#if defined(__AVR__)
#include <avr/wdt.h>
#endif

void PropCallback::run(char* topic, byte* payload, unsigned int len)
{
  if (len)
  {
    char* p = (char*)malloc(len + 1);
    memcpy(p, payload, len);
    p[len] = '\0';
    if (String(p) == "@PING")
      client->publish(outbox, "PONG");
    else
      Prop::onInboxMessageReceived(p);
    free(p);
  }
}

void PropMessageReceived::run(String a)
{
  client->publish(outbox, String("OMIT " + a).c_str());
}

Prop::Prop(const char* client_id, const char* in_box, const char* out_box, const char* broker, const int port)
{
  clientId = client_id;
  inbox = in_box;
  outbox = out_box;

  _nextReconAttempt = 0;
  _dataSentCount = 0;
  _maximumSilentPeriod = 30; // 30 seconds
  _payloadMax = MQTT_MAX_PACKET_SIZE - (1 + 2 + strlen(Prop::outbox));

  _brokerIpAddress.fromString(broker);
  _client.setServer(_brokerIpAddress, port);

  PropCallback::client = &_client;
  PropCallback::outbox = outbox;
  _client.setCallback(PropCallback::run);

  PropMessageReceived::client = &_client;
  PropMessageReceived::outbox = outbox;

  _sendDataAction.reset(400); // check data changes every 400 milliseconds
}

void Prop::addData(PropData* d)
{
  _dataTable.Add(d);
}

void Prop::loop()
{
  if (_client.connected())
  {
    _client.loop();
  }
  else if (millis() > _nextReconAttempt)
  {
    _nextReconAttempt += 5000L;

    if (_client.connect(clientId, outbox, 2, true, "DISCONNECTED"))
    {
      _client.publish(outbox, "CONNECTED", true);
      _client.subscribe(inbox, 1);  // max QoS is 1 for PubSubClient subsciption
      _nextReconAttempt = 0L;
    }
  }

  if (_sendDataAction.tick()) checkDataChanges(); // send data changes if any
}

void Prop::checkDataChanges()
{
  if (_dataSentCount) {
    sendDataChanges();
  } else {
    sendAllData();
  }

  ++_dataSentCount;

  // don't be silent to long
  if (_dataSentCount > (_maximumSilentPeriod * 1000 / _sendDataAction.getInterval())) _dataSentCount = 0;
}

void Prop::resetMcu()
{
#if defined(ARDUINO_ARCH_SAMD) || defined(STM32F4xx) || defined(STM32F7xx)
    NVIC_SystemReset();
#elif defined(ARDUINO_ARCH_AVR) || defined(__AVR__)
    wdt_enable(WDTO_15MS);
    while (true);
#endif
}

void Prop::sendAllData()
{
  String data("DATA "), str;
  for (int i = 0; i < _dataTable.Count(); i++) {
      str = _dataTable[i]->fetch();
      send(&data, &str);
  }

  if (data.length() > 5) {
    _client.publish(outbox, data.c_str());
  }
}

void Prop::sendDataChanges()
{
  String data("DATA "), str;
  for (int i = 0; i < _dataTable.Count(); i++) {
      str = _dataTable[i]->fetchChange();
      send(&data, &str);
  }

  if (data.length() > 5) {
    _client.publish(outbox, data.c_str());
  }
}

void Prop::send(String* d, String* s)
{
  if (d->length() + s->length() <= _payloadMax) {
    *d += *s;
  } else if (s->length() > _payloadMax + 5) {
    sendMesg(u8"Data ignored, loo larged to be sent");
  } else if (d->length() + s->length() > _payloadMax) {
    _client.publish(outbox, d->c_str());
    *d = "DATA " + *s;
  }
}

void Prop::sendData(String data) {
  if (data.length() > 5) {
    _client.publish(outbox, String("DATA " + data).c_str());
  }
}

void Prop::sendDone(String action) {
  _client.publish(outbox, String("DONE " + action).c_str());
}

void Prop::sendMesg(String mesg) {
  if (mesg.length() > 5) {
    _client.publish(outbox, String("MESG " + mesg).c_str());
  }
}

void Prop::sendMesg(String mesg, char *topic) {
  if (mesg.length() > 5) {
    _client.publish(topic, String("MESG " + mesg).c_str());
  }
}

void Prop::sendOmit(String action) {
  _client.publish(outbox, String("OMIT " + action).c_str());
}

void Prop::sendOver(String challenge) {
  _client.publish(outbox, String("OVER " + challenge).c_str());
}

void Prop::sendProg(String program) {
  _client.publish(outbox, String("PROG " + program).c_str());
}

void Prop::sendRequ(String request) {
  _client.publish(outbox, String("REQU " + request).c_str());
}

void Prop::sendRequ(String action, char* topic) {
  _client.publish(topic, String("REQU " + action).c_str());
}

void Prop::resetIntervals(const int changes, const int silent)
{
    _nextReconAttempt = 0;
    _dataSentCount = 0;
    _maximumSilentPeriod = silent; // seconds
    _sendDataAction.reset(changes); // milliseconds
}


PubSubClient *PropCallback::client;
const char *PropCallback::outbox;
PubSubClient *PropMessageReceived::client;
const char *PropMessageReceived::outbox;
void (*Prop::onInboxMessageReceived)(String) = PropMessageReceived::run;
