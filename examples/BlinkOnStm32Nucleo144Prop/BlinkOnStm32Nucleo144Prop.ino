/* Stm32Nucleo144Prop.ino
   MIT License (c) Faure Systems <dev at faure dot systems>

   Adapt the Blink example (https://www.arduino.cc/en/tutorial/blink) as a
   simple MQTT prop. Avoid delay() calls (except short ones) in loop() to
   ensure CPU for MQTT protocol. Use PropAction checks instead.

   Copy and change it to build your first Arduino connected prop, you will
   only be limited by your imagination.

   Requirements:
   - install ArduinoProps.zip library and dependencies (https://github.com/fauresystems/ArduinoProps)
   - help: https://github.com/xcape-io/ArduinoProps/blob/master/help/ArduinoProps_sketch.md
   - STM32 Nucleo-144 borads require: STM32duino LwIP and STM32duino STM32Ethernet libraries

*/
#if defined(ARDUINO_ARCH_STM32)
#include <LwIP.h>
#include <STM32Ethernet.h>
#include "ArduinoProps.h"
#include "Stm32Millis.h"
extern Stm32MillisClass Stm32Millis;

// If you're running xcape.io Room software you have to respect prop inbox/outbox
// topicw syntax: Room/[escape room name]/Props/[propsname]/inbox|outbox
// https://xcape.io/go/room

Stm32Nucleo144Prop prop(u8"Nucleo Blink", // as MQTT client id, should be unique per client for given broker
                        u8"Room/My room/Props/Nucleo Blink/inbox",
                        u8"Room/My room/Props/Nucleo Blink/outbox",
                        "192.168.1.42", // your MQTT server IP address
                        1883); // your MQTT server port;

byte mac[] = { 0x46, 0x4F, 0xEA, 0x10, 0x20, 0x04 }; //<<< MAKE SURE IT'S UNIQUE IN YOUR NETWORK!!! and not a reserved MAC
IPAddress ip(192, 168, 1, 177); //<<< Set to (0,0,0,0) to get an address from DHCP
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
IPAddress server(192, 168, 1, 42); // numeric IP for Google (no DNS)
//char server[] = "broker.mqtt-dashboard.com";    // name address for mqtt broker (using DNS)

PropDataLogical blinking(u8"blink", u8"yes", u8"no", true);
PropDataLogical led(u8"led");

void blink(); // forward
PropAction blinkingAction = PropAction(1000, blink);

void setup()
{
  if (ip == IPAddress(0,0,0,0)) {
    if (!Ethernet.begin(mac)) {
      // if DHCP fails, start with a hard-coded address:
      Ethernet.begin(mac, IPAddress(10, 90, 90, 239));
    }
  }
  else {
    Ethernet.begin(mac, ip);
  }

  delay(1500); // allow the hardware to sort itself out

  // millis() missing in STM32duino
  Stm32Millis.begin();

  prop.addData(&blinking);
  prop.addData(&led);

  prop.begin(InboxMessage::run);

  pinMode(LED_BUILTIN, OUTPUT); // initialize digital pin LED_BUILTIN as an output

  // At this point, the broker is not connected yet
}

void loop()
{
  prop.loop();

  led.setValue(digitalRead(LED_BUILTIN)); // read I/O

  blinkingAction.check(); // do your stuff, don't freeze the loop with delay() calls
}

void blink()
{
  if (blinking.value()) {
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
  else if (a == "blink:1")
  {
    blinking.setValue(true);

    prop.sendAllData(); // all data change, we don't have to be selctive then
    prop.sendDone(a); // acknowledge prop command action
  }
  else if (a == "blink:0")
  {
    blinking.setValue(false);

    prop.sendAllData(); // all data change, we don't have to be selctive then
    prop.sendDone(a); // acknowledge prop command action
  }
  else
  {
    // acknowledge omition of the prop command
    prop.sendOmit(a);
  }
}
#endif
