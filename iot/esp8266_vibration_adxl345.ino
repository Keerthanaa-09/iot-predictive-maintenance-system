#include <Wire.h>
#include <ESP8266WiFi.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

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

// Feeds
Adafruit_MQTT_Publish xFeed =
  Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/vibration-x");
Adafruit_MQTT_Publish yFeed =
  Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/vibration-y");
Adafruit_MQTT_Publish zFeed =
  Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/vibration-z");

// ADXL345
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

void connectWiFi() {
  Serial.print("Connecting to WiFi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
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
  Wire.begin();

  if (!accel.begin()) {
    Serial.println("ADXL345 not detected");
    while (1);
  }

  accel.setRange(ADXL345_RANGE_16_G);

  connectWiFi();
  connectMQTT();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) connectWiFi();
  if (!mqtt.connected()) connectMQTT();

  sensors_event_t event;
  accel.getEvent(&event);

  Serial.print("X: "); Serial.print(event.acceleration.x);
  Serial.print(" Y: "); Serial.print(event.acceleration.y);
  Serial.print(" Z: "); Serial.println(event.acceleration.z);

  xFeed.publish(event.acceleration.x);
  yFeed.publish(event.acceleration.y);
  zFeed.publish(event.acceleration.z);

  delay(2000);
}
