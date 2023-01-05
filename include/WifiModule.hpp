

#include <ESP8266WiFi.h>




int ScanSSID(int ssidSelected);
String SelectSSID();
bool ConnectSSID(String ssid, String password);
void InCommingClients(WiFiClient client);
void InCommingSecureClients(WiFiClientSecure client);
