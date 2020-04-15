/* BlinkOnBridgePubSub.ino
   MIT License (c) Marie Faure <dev at faure dot systems>

   Adapt the Blink example (https://www.arduino.cc/en/tutorial/blink) as a
   simple MQTT prop with PubSubClient.
*/
#include <Bridge.h>
#include <BridgeClient.h>
#include <Process.h>
#include <PubSubClient.h>
#include <VariableTimedAction.h>

// If you're running xcape.io Room software you have to respect prop inbox/outbox
// topicw syntax: Room/[escape room name]/Props/[propsname]/inbox|outbox
// https://xcape.io/go/room

#define BROKER          "192.168.1.53" // your MQTT server IP address
#define PROP_NAME      u8"Arduino Blink" // as MQTT client id, should be unique per client for given broker
#define PROP_INBOX     u8"Room/My room/Props/Arduino Blink/inbox"
#define PROP_OUTBOX    u8"Room/My room/Props/Arduino Blink/outbox"

// Yun can store broker IP address in Linino file systems, and updatred with ssh command
#define YUN_BROKER_FILE "/root/broker"

void publishAll(); // forward
void publishChanges(); // forward

class Blinking : public VariableTimedAction {
private:
	//this method will be called at your specified interval
	unsigned long run() {
		//swicth LED if blinking mode
		if (blink) {
			digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
		}

		//return code of 0 indicates no change to the interval
		//if the interval must be changed, then return the new interval
		return 0;
	}

public:
	bool blink;
};

class PublishAllData : public VariableTimedAction {
private:
	unsigned long run() {
		publishAll();
		return 0;
	}
};

class PublishChangedData : public VariableTimedAction {
private:
	unsigned long run() {
		publishChanges();
		return 0;
	}
};

Blinking blinking;

bool led_ref;
bool blink_ref;

BridgeClient _ethClient;
PubSubClient _client(_ethClient);
IPAddress _brokerAddress;

void callback(char*, byte*, unsigned int); // forward

// MQTT
unsigned long lastReconnection(0L);
PublishAllData publishAllData;
PublishChangedData publishChangedData;

void setup()
{
	Bridge.begin();

	Process p;
	p.begin("cat");
	p.addParameter(YUN_BROKER_FILE);
	p.run(); // run the process and wait for its termination
	String b;
	while (p.available() > 0) {
		char c = p.read();
		b += c;
	}
	b.trim();

	if (!_brokerAddress.fromString(b.c_str())) _brokerAddress.fromString(BROKER);
	_client.setServer(_brokerAddress, 1883);
	_client.setCallback(callback);

	publishAllData.start(30000);
	publishChangedData.start(400);

	blinking.blink = true;
	blinking.start(1000);
	blink_ref = false;

	pinMode(LED_BUILTIN, OUTPUT); // initialize digital pin LED_BUILTIN as an output
	digitalWrite(LED_BUILTIN, HIGH);
	led_ref = LOW;

	// At this point, the broker is not connected yet
}

void loop()
{
	if (_client.connected())
	{
		_client.loop();
	}
	else if (millis() > lastReconnection)
	{
		lastReconnection += 5000L;

		if (_client.connect(PROP_NAME, PROP_OUTBOX, 2, true, "DISCONNECTED"))
		{
			_client.publish(PROP_OUTBOX, "CONNECTED", true);
			_client.subscribe(PROP_INBOX, 1); // max QoS is 1 for PubSubClient subsciption
			lastReconnection = 0L;
		}
	}

	// automation code should be here (nothing to do for this example)

	VariableTimedAction::updateActions();
}

void publishAll()
{
	String buf = "DATA";

	bool led = digitalRead(LED_BUILTIN);

	buf += u8" led=" + (led ? String("1") : String("0"));
	led_ref = led;

	buf += u8" clignote=" + (blinking.blink ? String("oui") : String("non"));
	blink_ref = blinking.blink;

	_client.publish(PROP_OUTBOX, buf.c_str());
}

void publishChanges()
{
	String buf = "DATA";

	bool led = digitalRead(LED_BUILTIN);

	if (led != led_ref)
	{
		buf += u8" led=" + (led ? String("1") : String("0"));
		led_ref = led;
	}

	if (blinking.blink != blink_ref)
	{
		buf += u8" clignote=" + (blinking.blink ? String("oui") : String("non"));
		blink_ref = blinking.blink;
	}

	if (buf.length() > 4)
		_client.publish(PROP_OUTBOX, buf.c_str());
}

void publishDone(String a)
{
	a = "DONE " + a;
	_client.publish(PROP_OUTBOX, a.c_str());
}

void onInboxMessage(String a) {

	if (a == u8"app:startup")
	{
		publishAll();
		publishDone(a);
	}
	else if (a == "clignoter:1")
	{
		digitalWrite(LED_BUILTIN, HIGH);
		blinking.blink = true;

		publishAll(); // all data change, we don't have to be selctive then
		publishDone(a); // acknowledge prop command action
	}
	else if (a == "clignoter:0")
	{
		digitalWrite(LED_BUILTIN, LOW);
		blinking.blink = false;

		publishAll(); // all data change, we don't have to be selctive then
		publishDone(a); // acknowledge prop command action
	}
	else
	{
		// acknowledge omition of the prop command
		_client.publish(PROP_OUTBOX, String("OMIT " + a).c_str());
	}
}

void callback(char* topic, byte* payload, unsigned int len)
{
	if (len)
	{
		char* p = (char*)malloc(len + 1);
		memcpy(p, payload, len);
		p[len] = '\0';
		if (String(p) == "@PING")
			_client.publish(PROP_OUTBOX, "PONG");
		else
			onInboxMessage(p);
		free(p);
	}
}
