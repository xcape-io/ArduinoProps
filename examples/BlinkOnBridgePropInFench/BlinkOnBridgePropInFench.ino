/* BlinkOnBridgeProp.ino
   MIT License (c) Faure Systems <dev at faure dot systems>

   Adapt the Blink example (https://www.arduino.cc/en/tutorial/blink) as a
   simple MQTT prop. Avoid delay() calls (except short ones) in loop() to
   ensure CPU for MQTT protocol. Use PropAction checks instead.

   Copy and change it to build your first Arduino connected prop, you will
   only be limited by your imagination.

   Requirements:
   - install ArduinoProps.zip library and dependencies (https://github.com/fauresystems/ArduinoProps)
   - help: https://github.com/xcape-io/ArduinoProps/blob/master/help/ArduinoProps_sketch.md
*/
#include <Bridge.h>
#include "ArduinoProps.h"

// If you're running xcape.io Room software you have to respect prop inbox/outbox
// topicw syntax: Room/[escape room name]/Props/[propname]/inbox|outbox
// https://xcape.io/go/room

BridgeProp prop(u8"Arduino Blink", // as MQTT client id, should be unique per client for given broker
                  u8"Room/My room/Props/Arduino Blink/inbox",
                  u8"Room/My room/Props/Arduino Blink/outbox",
                  "192.168.1.53", // your MQTT server IP address
                  1883); // your MQTT server port;

PropDataLogical clignoter(u8"clignote", u8"oui", u8"non", true);
PropDataLogical led(u8"led");
PropDataText rssi(u8"rssi");

void clignote(); // forward
PropAction clignoteAction = PropAction(1000, clignote);

void lireRssi(); // forward
PropAction lireRssiAction = PropAction(30000, lireRssi);

void setup()
{
  Bridge.begin();
  //updateBrokerAdressFromFile("/root/broker", &prop); // if you're running our Escape Room control software (Room 2.0)

  prop.addData(&clignoter);
  prop.addData(&led);
  prop.addData(&rssi);

  prop.begin(InboxMessage::run);

  pinMode(LED_BUILTIN, OUTPUT); // initialize digital pin LED_BUILTIN as an output

  lireRssi();

  // At this point, the broker is not connected yet
}

void loop()
{
  prop.loop();

  led.setValue(digitalRead(LED_BUILTIN)); // read I/O

  clignoteAction.check(); // do your stuff, don't freeze the loop with delay() calls
  lireRssiAction.check();
}

void clignote()
{
  if (clignoter.value()) {
    led.setValue(!led.value());
    digitalWrite(LED_BUILTIN, led.value() ? HIGH : LOW);
  }
}

void lireRssi()
{
  Process _process; // a process call takes about 50 milliseconds
  _process.runShellCommand("cat /proc/net/wireless | awk 'NR==3 {print $4}'");
  while (_process.running());
  String b;
  while (_process.available() > 0) {
    char c = _process.read();
    b += c;
  }
  b.trim();
  rssi.setValue(b + " dBm");
}

void InboxMessage::run(String a) {

  if (a == u8"app:startup")
  {
    prop.sendAllData();
    prop.sendDone(a);
  }
  else if (a == u8"reset-mcu")
  {
    prop.resetMcu(); // we prefer SSH command: echo %BROKER%> /root/broker && reset-mcu
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
