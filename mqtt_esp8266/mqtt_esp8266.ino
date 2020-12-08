#include <ESP8266WiFi.h>
#include <MQTTClient.h>

const char* ssid     = "RASPI-NETWORK";
const char* password = "raspinetwork";

WiFiClient WiFiclient;
MQTTClient client;

unsigned long lastMillis = 0;

int sensor_type = 2;
int sensor_id = 1;

char buf[100];

void setup() {
  pinMode(A0, INPUT);
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  delay(2000);

  Serial.print("connecting to MQTT broker...");
  //  client.begin("broker.shiftr.io", WiFiclient);
  client.begin("192.168.10.10", WiFiclient);
  client.onMessage(messageReceived);
  connect();
}

void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);
}

void connect() {
  //  client_id, mqtt broker username, mqtt broker password
  char sid[50];
  sprintf(sid, "sensor%d.%d", sensor_id, sensor_type);
  while (!client.connect(sid, "pi", "raspberry")) {
    Serial.print(".");
  }
  //  Serial.println("\nconnected!");
  //  client.subscribe("wsn/#");
  client.subscribe("wsn/check");
}

int val = 0;

void loop() {
  //  int val = analogRead(A0);
  client.loop();
  if (!client.connected()) {
    connect();
  }

  if (millis() - lastMillis > 1000) {
    lastMillis = millis();

    val += 1;
    if (val >= 60) {
      val = 0;
    }
    int adc = analogRead(A0);
    //    sprintf(buf, "%d,%d", val, adc);
    sprintf(buf, "%d,%d", sensor_id, adc);
    Serial.print("sending: ");
    Serial.println(buf);

    char sid[50];
    sprintf(sid, "wsn/sensor/%d", sensor_type);
    client.publish(sid, buf);
  }
}
