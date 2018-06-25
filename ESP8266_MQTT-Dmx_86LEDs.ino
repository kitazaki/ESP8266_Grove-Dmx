#include <ESP8266WiFi.h>
#include <Milkcocoa.h>
#include <Arduino.h>
#include <ESPDMX.h>

#define WLAN_SSID       ""  // Wi-Fi SSID
#define WLAN_PASS       ""  // Wi-Fi Password

WiFiClient client;
DMXESPSerial dmx;

#define MILKCOCOA_APP_ID      ""  // MilkCocoa Application ID
#define MILKCOCOA_DATASTORE   ""  // MilkCocoa Datastore Name
#define MILKCOCOA_SERVERPORT  1883

const char MQTT_SERVER[] PROGMEM    = MILKCOCOA_APP_ID ".mlkcca.com";
const char MQTT_CLIENTID[] PROGMEM  = __TIME__ MILKCOCOA_APP_ID;

Milkcocoa milkcocoa = Milkcocoa(&client, MQTT_SERVER, MILKCOCOA_SERVERPORT, MILKCOCOA_APP_ID, MQTT_CLIENTID);

int red = 0;
int green = 0;
int blue = 0;
int dimmer = 0;
int address = 0;

void onpush(DataElement *elem) {
  Serial.println("onpush");
  Serial.print("r:");
  Serial.print(elem->getInt("r"));
  Serial.print(",g:");
  Serial.print(elem->getInt("g"));
  Serial.print(",b:");
  Serial.print(elem->getInt("b"));
  Serial.print(",s:");
  Serial.print(elem->getInt("s"));
  Serial.print(",t:");
  Serial.print(elem->getInt("t"));
  Serial.println();

  red = elem->getInt("r");  // R [0-255]
  green = elem->getInt("g");  // G [0-255]
  blue = elem->getInt("b");  // B [0-255]
  dimmer = elem->getInt("s");  // Dimmer Strength [0-127]
  address = elem->getInt("t");  // DMX Address [1-512]

  //write to DMX bus
  dmx.write(address, dimmer);
  dmx.write(address+1, red);
  dmx.write(address+2, green);
  dmx.write(address+3, blue);
  dmx.update();
}

void setupWiFi() {
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void setup() {
  Serial.begin(115200);
  setupWiFi();
  Serial.println( milkcocoa.on(MILKCOCOA_DATASTORE, "push", onpush) );
}

void loop() {
  milkcocoa.loop();
}

