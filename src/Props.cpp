/*
  Name:    Props.cpp
  Created: 29/10/2019 09:20:31
  Author:  Marie Faure <dev at faure dot systems>
  Editor:  https://github.com/fauresystems
  License: MIT License (c) Marie Faure <dev at faure dot systems>

  Base class to make props sketch for Escape Room 2.0 (connected).
*/

#include "Props.h"
#include <Process.h>
#if defined(__AVR__)
#include <avr/wdt.h>
#endif

void PropsCallback::run(char* topic, byte* payload, unsigned int len)
{
  if (len)
  {
    char* p = (char*)malloc(len + 1);
    memcpy(p, payload, len);
    p[len] = '\0';
    if (String(p) == "@PING")
      client->publish(outbox, "PONG");
    else
      Props::onInboxMessageReceived(p);
    free(p);
  }
}

void PropsMessageReceived::run(String a)
{
  client->publish(outbox, String("OMIT " + a).c_str());
}

Props::Props(const char* client_id, const char* in_box, const char* out_box, const char* broker, const int port)
{
  clientId = client_id;
  inbox = in_box;
  outbox = out_box;

  _nextReconAttempt = 0;
  _dataSentCount = 0;
  _maximumSilentPeriod = 30; // 30 seconds
  _payloadMax = MQTT_MAX_PACKET_SIZE - (1 + 2 + strlen(Props::outbox));

  _brokerIpAddress.fromString(broker);
  _client.setServer(_brokerIpAddress, port);

  PropsCallback::client = &_client;
  PropsCallback::outbox = outbox;
  _client.setCallback(PropsCallback::run);

  PropsMessageReceived::client = &_client;
  PropsMessageReceived::outbox = outbox;

  _sendDataAction.reset(400); // check data changes every 400 milliseconds
}

void Props::addData(PropsData* d)
{
  _dataTable.Add(d);
}

void Props::loop()
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

void Props::checkDataChanges()
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

void Props::resetMcu()
{
#if defined(ARDUINO_ARCH_SAMD)
	NVIC_SystemReset();
#elif defined(ARDUINO_ARCH_AVR)
	wdt_enable(WDTO_15MS);
	while (true);
#endif
}

void Props::sendAllData()
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

void Props::sendDataChanges()
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

void Props::send(String* d, String* s)
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

void Props::sendData(String data) {
  if (data.length() > 5) {
    _client.publish(outbox, String("DATA " + data).c_str());
  }
}

void Props::sendDone(String action) {
  _client.publish(outbox, String("DONE " + action).c_str());
}

void Props::sendMesg(String mesg) {
  if (mesg.length() > 5) {
    _client.publish(outbox, String("MESG " + mesg).c_str());
  }
}

void Props::sendMesg(String mesg, char *topic) {
  if (mesg.length() > 5) {
    _client.publish(topic, String("MESG " + mesg).c_str());
  }
}

void Props::sendOmit(String action) {
  _client.publish(outbox, String("OMIT " + action).c_str());
}

void Props::sendOver(String challenge) {
  _client.publish(outbox, String("OMIT " + challenge).c_str());
}

void Props::sendProg(String program) {
  _client.publish(outbox, String("PROGRAM " + program).c_str());
}

void Props::sendRequ(String request) {
  _client.publish(outbox, String("REQU " + request).c_str());
}

void Props::sendRequ(String action, char* topic) {
  _client.publish(topic, String("OMIT " + action).c_str());
}

void Props::resetIntervals(const int changes, const int silent)
{
	_nextReconAttempt = 0;
	_dataSentCount = 0;
	_maximumSilentPeriod = silent; // seconds
	_sendDataAction.reset(changes); // milliseconds
}


PubSubClient *PropsCallback::client;
const char *PropsCallback::outbox;
PubSubClient *PropsMessageReceived::client;
const char *PropsMessageReceived::outbox;
void (*Props::onInboxMessageReceived)(String) = PropsMessageReceived::run;
