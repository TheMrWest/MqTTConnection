/* 
    - Connection MqTT
*/

#include <Connection.h>
#include <Arduino.h>
#include <System.h>

WiFIConnection wifi;
MqTT mqtt;

bool connectedwifi;

void setup() {
    Serial.begin(System::BAUND_RATE);

    wifi.setCredencials("SSID", "PASSOWRD"); // Set SSID and PASSWORD
    mqtt.setServer("test.mosquitto.org", 1883); // Set SERVER and PORT

    Serial.println("--");
    connectedwifi = wifi.connect(5); // 5 Second
    if(connectedwifi) {
        Serial.println("\nPerfect");
        mqtt.begin();
    }
}

unsigned long int timeprev = 0;
void loop() {
    if (connectedwifi) {
        mqtt.loop();
        if (millis() - timeprev >= 1000) {
            timeprev = millis();
            mqtt.sendMessage("topic", "message");

        }
    } else {
        // 
    }
}