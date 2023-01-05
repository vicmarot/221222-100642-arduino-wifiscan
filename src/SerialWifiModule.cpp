

#include "SerialWifiModule.hpp"
#include "Define.hpp"



bool SerialWiFiConnected = false;



void SendMessageSerialWifi(String Message)
{
    WiFiClient SerialClient;

    if(!SerialClient.connect("192.168.3.23", 8080))
    {
        Serial.println("Error al conectar al servidor");
        return;
    }

    SerialClient.println("POST /enviar_mensaje HTTP/1.1");
    SerialClient.println("Content-Type: text/plain");
    SerialClient.print("Content-Length: ");
    SerialClient.println(Message.length() + 1);
    SerialClient.println();
    SerialClient.println(Message);

    // We read the server's response
    while (SerialClient.connected()) {
      String line = SerialClient.readStringUntil('\n');
      if (line == "\r") {
        break;
      }
    }

    delay(1000);
    SerialClient.stop();
}

void SendMessageSerial(String Message)
{
    #ifdef DEBUG
    Serial.print(Message);
    SendMessageSerialWifi(Message);
    #endif
}

void SendMessageSerialln(String Message)
{
    #ifdef DEBUG
    Message = Message + "\n";
    SendMessageSerial(Message);
    #endif
}