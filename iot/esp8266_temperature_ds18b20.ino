#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <OneWire.h>
#include <DallasTemperature.h>

/* ----------- USER CONFIG ----------- */
#define WIFI_SSID     "YOUR_WIFI_NAME"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"

#define AIO_SERVER     "io.adafruit.com"
#define AIO_SERVERPORT 1883
#define AIO_USERNAME   "YOUR_ADAFRUIT_USERNAME"
#define AIO_KEY        "YOUR_ADAFRUIT_IO_KEY"
/* ----------------------------------- */

// WiFi & MQTT client
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT,
                          AIO_USERNAME, AIO_KEY);

// Adafruit IO feed
Adafruit_MQTT_Publish temperatureFeed =
  Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/temp-sensor");

// DS18B20 setup
#define ONE_WIRE_BUS D4   // GPIO2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void connectWiFi() {
  Serial.print("Connecting to WiFi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println(" Connected");
}

void connectMQTT() {
  while (!mqtt.connected()) {
    Serial.print("Connecting to Adafruit IO...");
    if (mqtt.connect()) {
      Serial.println(" Connected");
    } else {
      Serial.println(" Failed, retrying...");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  sensors.begin();
  connectWiFi();
  connectMQTT();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) connectWiFi();
  if (!mqtt.connected()) connectMQTT();

  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);

  if (tempC == DEVICE_DISCONNECTED_C) {
    Serial.println("DS18B20 not detected");
    delay(2000);
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(tempC);
  Serial.println(" °C");

  temperatureFeed.publish(tempC);
  delay(5000);
}
