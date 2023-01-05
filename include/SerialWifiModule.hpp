
#include <ESP8266WiFi.h>

#define Serialprint(str) (SendMessageSerial(String(str)))
#define Serialprintln(str) (SendMessageSerialln(String(str)))

extern bool SerialWiFiConnected;
extern WiFiClient SerialClient;


void SendMessageSerialWifi(String Message);
void SendMessageSerial(String Message);
void SendMessageSerialln(String Message);
