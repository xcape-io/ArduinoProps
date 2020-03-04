/* BlinkOnBridgeProps.ino
   MIT License (c) Marie Faure <dev at faure dot systems>

   Adapt the Blink example (https://www.arduino.cc/en/tutorial/blink) as a
   simple MQTT props. Avoid delay() calls (except short ones) in loop() to
   ensure CPU for MQTT protocol. Use PropsAction checks instead.

   Copy and change it to build your first Arduino connected props, you will
   only be limited by your imagination.

   Requirements:
   - install ArduinoProps.zip library and dependencies (https://github.com/fauresystems/ArduinoProps)
*/
#include <Bridge.h>
#include "ArduinoProps.h"

// If you're running xcape.io Room software you have to respect props inbox/outbox
// topicw syntax: Room/[escape room name]/Props/[propsname]/inbox|outbox
// https://xcape.io/go/room

BridgeProps props(u8"Arduino Blink", // as MQTT client id, should be unique per client for given broker
                  u8"Room/My room/Props/Arduino Blink/inbox",
                  u8"Room/My room/Props/Arduino Blink/outbox",
                  "192.168.1.53", // your MQTT server IP address
                  1883); // your MQTT server port;

PropsDataLogical blinking(u8"blink", u8"yes", u8"no", true);
PropsDataLogical led(u8"led");
PropsDataText rssi(u8"rssi");

void clignote(); // forward
PropsAction blinkingAction = PropsAction(1000, clignote);

void lireRssi(); // forward
PropsAction lireRssiAction = PropsAction(30000, lireRssi);

void setup()
{
  Bridge.begin();
  //updateBrokerAdressFromFile("/root/broker", &props); // if you're running our Escape Room control software (Room 2.0)

  props.addData(&blinking);
  props.addData(&led);
  props.addData(&rssi);

  props.begin(InboxMessage::run);

  pinMode(LED_BUILTIN, OUTPUT); // initialize digital pin LED_BUILTIN as an output

  lireRssi();

  // At this point, the broker is not connected yet
}

void loop()
{
  props.loop();

  led.setValue(digitalRead(LED_BUILTIN)); // read I/O

  blinkingAction.check(); // do your stuff, don't freeze the loop with delay() calls
  lireRssiAction.check();
}

void clignote()
{
  if (blinking.value()) {
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
    props.sendAllData();
    props.sendDone(a);
  }
  else if (a == u8"reset-mcu")
  {
    props.resetMcu(); // we prefer SSH command: echo %BROKER%> /root/broker && reset-mcu
  }
  else if (a == "blink:1")
  {
    blinking.setValue(true);

    props.sendAllData(); // all data change, we don't have to be selctive then
    props.sendDone(a); // acknowledge props command action
  }
  else if (a == "blink:0")
  {
    blinking.setValue(false);

    props.sendAllData(); // all data change, we don't have to be selctive then
    props.sendDone(a); // acknowledge props command action
  }
  else
  {
    // acknowledge omition of the props command
    props.sendOmit(a);
  }
}

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
