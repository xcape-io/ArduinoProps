/* BlinkOnWifiProps.ino
   MIT License (c) Marie Faure <dev at faure dot systems>

   Adapt the Blink example (https://www.arduino.cc/en/tutorial/blink) as a
   simple MQTT props. Avoid delay() calls (except short ones) in loop() to
   ensure CPU for MQTT protocol. Use PropsAction checks instead.

   Copy and change it to build your first Arduino connected props, you will
   only be limited by your imagination.

   Requirements: install ArduinoProps.zip library.
*/
#include "ArduinoProps.h"

// Setup your WiFi network
const char* ssid = "";
const char *passphrase = "";

// Builtin led is not available with the shield
#undef LED_BUILTIN
#define LED_BUILTIN 8

// If you're running our Escape Room control software (Room 2.0) you have to respect
// prpos inbox/outbox syntax Room/[escape room name]/Props/[propsname]/inbox|outbox
// https://live-escape.net/go/room

WifiProps props(u8"Arduino Contrôleur", // as MQTT client id, should be unique per client for given broker
                u8"Room/Demoniak/Props/Arduino Contrôleur/inbox",
                u8"Room/Demoniak/Props/Arduino Contrôleur/outbox",
                "192.168.1.42", // your MQTT server IP address
                1883); // your MQTT server port;

PropsDataLogical clignoter(u8"clignote", u8"oui", u8"non", true);
PropsDataLogical led(u8"led");

void clignote(); // forward
PropsAction clignoteAction = PropsAction(1000, clignote);

bool wifiBegun(false);

void setup()
{
  Serial.begin(9600);

  props.addData(&clignoter);
  props.addData(&led);

  props.begin(InboxMessage::run);

  pinMode(LED_BUILTIN, OUTPUT); // initialize digital pin LED_BUILTIN as an output

  // At this point, the broker is not connected yet
}

void loop()
{
  if (!wifiBegun) {
    WiFi.begin(ssid, passphrase);
    Serial.println(WiFi.firmwareVersion());
    delay(250); // acceptable freeze for this props (otherwise use PropsAction for async-like behavior)
    // do static IP configuration disabling the dhcp client, must be called after every WiFi.begin()
    String fv = WiFi.firmwareVersion();
    if (fv.startsWith("1.0")) {
      Serial.println("Please upgrade the firmware for static IP");
      // see https://github.com/fauresystems/ArduinoProps/blob/master/WifiNinaFirmware.md
    }
    else {
      //WiFi.config(IPAddress(192, 168, 1, 21), // local_ip
      //	IPAddress(192, 168, 1, 1),  // dns_server
      //	IPAddress(192, 168, 1, 1),  // gateway
      //	IPAddress(255, 255, 255, 0)); // subnet
    }
    if (WiFi.status() == WL_CONNECTED) {
      wifiBegun = true;
      Serial.println(WiFi.localIP());
      Serial.println(WiFi.subnetMask());
      Serial.println(WiFi.gatewayIP());
    } else {
      WiFi.end();
    }
  } else if (wifiBegun && WiFi.status() != WL_CONNECTED) {
    WiFi.end();
    wifiBegun = false;
  }

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
