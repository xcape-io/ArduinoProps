/* BlinkOnEthernetProps.ino
   MIT License (c) Marie Faure <dev at faure dot systems>

   Adapt the Blink example (https://www.arduino.cc/en/tutorial/blink) as a
   simple MQTT props. Avoid delay() calls (except short ones) in loop() to
   ensure CPU for MQTT protocol. Use PropsAction checks instead.

   Copy and change it to build your fist Arduino connected props, you will
   only be limited by your imagination.

   Requirements: install ArduinoProps.zip library.
*/
#include <Ethernet.h>
#include <IPAddress.h>
#include "ArduinoProps.h"

byte mac[] = { 0x46, 0x4F, 0xEA, 0x10, 0x20, 0x02 }; //<<< MAKE SURE IT'S UNIQUE IN YOUR NETWORK!!! and not a reserved MAC
String ip = ""; //<<< ENTER YOUR IP ADDRESS HERE ("" for DHCP)

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

  props.addData(&clignoter);
  props.addData(&led);

  props.begin(InboxMessage::run);

  pinMode(LED_BUILTIN, OUTPUT); // initialize digital pin LED_BUILTIN as an output

  // At this point, the broker is not connected yet
}

void loop()
{
  props.loop();

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
