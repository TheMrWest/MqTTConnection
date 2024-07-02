#ifndef WIFICONFIG_H
#define WIFICONFIG_H

#include <WiFi.h>
#include <Arduino.h>
#include <PubSubClient.h>

WiFiClient espClient;
PubSubClient client(espClient);

class WiFIConnection {
    public:
       WiFIConnection() {}
       WiFIConnection(const char* ssid, const char* passw) {this->_ssid = ssid; this->_passw = passw;}
       void setCredencials(const char* ssid, const char* passw) {this->_ssid = ssid; this->_passw = passw;}

       bool connect(short tempConnection) {
        if (abs(tempConnection) > max_time) {Serial.printf("Temp Connection defined for %d sec\n", default_time); tempConnection = default_time;}
        unsigned long startTime = millis();
        tempPrevConnection = 0;
        
        WiFi.begin(_ssid, _passw);
        while (WiFi.status() != WL_CONNECTED && millis() - startTime <= abs(tempConnection) * 1000) {
            if (millis() - tempPrevConnection > 500) {tempPrevConnection = millis();Serial.print(".");}
        }
        if(WiFi.status() == WL_CONNECTED) {return true;} else {return false;}}

    private:
       const char* _ssid;
       const char* _passw;
       unsigned long tempPrevConnection;
       short max_time = 60;
       short default_time = 5;
};

class MqTT {
    public:
       MqTT() {}
       MqTT(const char* server, const int port) {this->_mqttserver = server; this->_mqtt_port = port;}
       void setServer(const char* server, const int port) {this->_mqttserver = server; this->_mqtt_port = port;}
       
       void begin() {client.setServer(_mqttserver, _mqtt_port);}
       void loop() {if(!client.connected()) {reconnect();}client.loop();}
       bool sendMessage(const char* topic, const char* message) {return client.publish(topic, message);}

    private:
       const char* _mqttserver;
       int _mqtt_port;
       unsigned long int timePrev_connection_mqtt = 0; 
       
       void reconnect() {
        if (!client.connected()) {
            unsigned long currentMillis = millis();
            if (currentMillis - timePrev_connection_mqtt >= 5000) {
                timePrev_connection_mqtt = currentMillis;
                String clientId = "ESP32Client-";
                clientId += String(random(0xffff), HEX);
                if (client.connect(clientId.c_str())) {
                    Serial.println("Conectado");
                } else {
                    Serial.print("failed, rc=");
                    Serial.print(client.state());
                    Serial.println(" try again in 5 seconds");
                }
            }
        }
    }
};

#endif