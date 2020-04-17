/* BlinkOnEthernetProp.ino
   MIT License (c) Marie Faure <dev at faure dot systems>

   Adapt the Blink example (https://www.arduino.cc/en/tutorial/blink) as a
   simple MQTT prop. Avoid delay() calls (except short ones) in loop() to
   ensure CPU for MQTT protocol. Use PropAction checks instead.

   Copy and change it to build your first Arduino connected prop, you will
   only be limited by your imagination.

   Requirements: 
   - install ArduinoProps.zip library and dependencies (https://github.com/fauresystems/ArduinoProps)
   - help: https://github.com/xcape-io/ArduinoProps/blob/master/help/ArduinoProps_sketch.md
*/
#include <Ethernet.h>
#include <IPAddress.h>
#include "ArduinoProps.h"

// If you're running xcape.io Room software you have to respect prop inbox/outbox
// topicw syntax: Room/[escape room name]/Props/[propsname]/inbox|outbox
// https://xcape.io/go/room

EthernetProp prop(u8"Arduino Blink", // as MQTT client id, should be unique per client for given broker
                  u8"Room/My room/Props/Arduino Blink/inbox",
                  u8"Room/My room/Props/Arduino Blink/outbox",
                  "192.168.1.53", // your MQTT server IP address
                  1883); // your MQTT server port;

byte mac[] = { 0x46, 0x4F, 0xEA, 0x10, 0x20, 0x02 }; //<<< MAKE SURE IT'S UNIQUE IN YOUR NETWORK!!! and not a reserved MAC
String ip = "192.168.1.19"; //<<< ENTER YOUR IP ADDRESS HERE ("" for DHCP)

// Builtin led is not available with the shield
#undef LED_BUILTIN
#define LED_BUILTIN 8

PropDataLogical clignoter(u8"clignote", u8"oui", u8"non", true);
PropDataLogical led(u8"led");

void clignote(); // forward
PropAction clignoteAction = PropAction(1000, clignote);

void setup()
{
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

  // can do more static IP configuration, must be called after Ethernet.begin()
  //Ethernet.setSubnetMask(IPAddress(255, 255, 255, 0));
  //Ethernet.setGatewayIP(IPAddress(192, 168, 1, 1));
  //Ethernet.setDnsServerIP(IPAddress(192, 168, 1, 1));

  prop.addData(&clignoter);
  prop.addData(&led);

  prop.begin(InboxMessage::run);

  pinMode(LED_BUILTIN, OUTPUT); // initialize digital pin LED_BUILTIN as an output

  // At this point, the broker is not connected yet
}

void loop()
{
  prop.loop();

  led.setValue(digitalRead(LED_BUILTIN)); // read I/O

  clignoteAction.check(); // do your stuff, don't freeze the loop with delay() calls
}

void clignote()
{
  if (clignoter.value()) {
    led.setValue(!led.value());
    digitalWrite(LED_BUILTIN, led.value() ? HIGH : LOW);
  }
}

void InboxMessage::run(String a) {

  if (a == u8"app:startup")
  {
    prop.sendAllData();
    prop.sendDone(a);
  }
  else if (a == u8"reset-mcu")
  {
    prop.resetMcu();
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
