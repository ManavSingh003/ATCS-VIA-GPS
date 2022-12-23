#define PubNub_BASE_CLIENT WiFiClient
#include <PubNub.h>
const static char ssid[] = "Sensor Network";
const static char pass[] = "sens0rpassw0rd";
int sendTimer;   // Timer used to control how often messages are published. Sends a new message once every 10 minutes.
int sensorTimer; // Timer used to control how often sensor is checked. Checked every 60 seconds.
float distance; // total distance travlled for the time period.
float amount; // calculated amount from the host

void setup() {
  float disttravel = distance;
  int amount = amount;
  Serial.begin(9600);
  WiFi.begin(ssid, pass);
  if (WiFi.waitForConnectResult() == WL_CONNECTED) {
    Serial.println("WiFi connected.");
    PubNub.begin("pub-c-cb7fea85-05a7-4810-b74b-92fe216d84b5", "sub-c-9844c293-0f64-4c63-9876-916bf1e7a0d0");
  } else {
    Serial.println("Couldn't get a wifi connection.");
    while (1) delay(100);
  }
}

void loop() {
  sensorTimer = (sensorTimer + 1); // Add a second to the sensor timer.
  if (sensorTimer == 60){ // Check sensor.
    sensorTimer = 0; // Reset timer.
    sendTimer = (sendTimer + 1); // Add a minute to sendTimer.
    if (sendTimer == 43800 ) { // Reset timer after a month.
      sendTimer = 0; // Reset timer.
    }

    if ((distance != 0) || (sendTimer == 0)) { // Send a new message if sendTimer was reset or if UV index has changed.
      PubNub_BASE_CLIENT *client;
      
      client = PubNub.publish("Message");
      if (!client) {
        Serial.println("publishing error");
        delay(1000);
        return;
      }
      while (client->connected()) {
        while (client->connected() && !client->available());
        Serial.println("publishing a message");
        Serial.print("Respected Client, the");
        Serial.print(amount);
        Serial.print(" amount has been deducted from your given bank account on your travel journey of distance")
        Serial.print(disttravel);
      }
      client->stop();
      Serial.println();
    }
  }
  delay(1000);
}
}