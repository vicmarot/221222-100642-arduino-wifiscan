/*

WiFiModule.cpp

*/

#include "WifiModule.hpp"
#include "SerialModule.hpp"
#include "UtilsModule.hpp"
#include "GPIO.hpp"
#include "SerialWifiModule.hpp"



void InCommingClients(WiFiClient client)
{
    
    if (client) {  // if a client connects
        Serialprintln("New client connected!");
        while (client.connected()) {  // while the client is connected
            if (client.available()) {  // if there's data available to read
                String request = client.readString();
                Serial.println(request);
                client.flush();  // flush the request from the buffer

                // send a response to the client
                client.println("HTTP/1.1 200 OK");
                client.println("Content-Type: text/plain");
                client.println("Connection: close");
                client.println();

                String task = GetMessage(request);
                Serialprintln(task);

                
                if (task.compareTo("ERROR") == 0)
                {
                    client.println("Task ERROR");
                }
                else if(task.compareTo("Switch 1") == 0 || task.compareTo("switch 1") == 0)
                {   
                    client.print("The realy 1 will be turn ");
                    if (GetReralyStatus(relay1) == HIGH)
                    client.println("on");
                    else
                    client.println("off");
                    SwitchRealytatus(relay1);
                }
                else if(task.compareTo("Switch 2") == 0 || task.compareTo("switch 2") == 0)
                {   
                    client.print("The realy 2 will be turn ");
                    if (GetReralyStatus(relay2) == HIGH)
                    client.println("on");
                    else
                    client.println("off");
                    SwitchRealytatus(relay2);
                }
                else if (task.compareTo("Status 1") == 0 || task.compareTo("status 1") == 0)
                {
                    client.print("The realy 1 currently is in ");
                    if (GetReralyStatus(relay1) == HIGH)
                    client.println("on");
                    else
                    client.println("off");
                }
                else if (task.compareTo("Status 2") == 0 || task.compareTo("status 2") == 0)
                {
                    client.print("The realy 2 currently is in ");
                    if (GetReralyStatus(relay2) == HIGH)
                    client.println("on");
                    else
                    client.println("off");
                }
                else
                {
                    client.println("Task not found");
                }
                        
                
                break;
            }
        }
    
   }

}


void InCommingSecureClients(WiFiClientSecure client)
{
    
    if (client) {  // if a client connects
        Serialprintln("New secure client connected!");
        while (client.connected()) {  // while the client is connected
            if (client.available()) {  // if there's data available to read
                String request = client.readString();
                Serial.println(request);
                client.flush();  // flush the request from the buffer

                // send a response to the client
                client.println("HTTP/1.1 200 OK");
                client.println("Content-Type: text/plain");
                client.println("Connection: close");
                client.println();

                String task = GetMessage(request);
                Serialprintln(task);

                
                if (task.compareTo("ERROR") == 0)
                {
                    client.println("Task ERROR");
                }
                else if(task.compareTo("Switch 1") == 0 || task.compareTo("switch 1") == 0)
                {   
                    client.print("The realy 1 will be turn ");
                    if (GetReralyStatus(relay1) == HIGH)
                    client.println("on");
                    else
                    client.println("off");
                    SwitchRealytatus(relay1);
                }
                else if(task.compareTo("Switch 2") == 0 || task.compareTo("switch 2") == 0)
                {   
                    client.print("The realy 2 will be turn ");
                    if (GetReralyStatus(relay2) == HIGH)
                    client.println("on");
                    else
                    client.println("off");
                    SwitchRealytatus(relay2);
                }
                else if (task.compareTo("Status 1") == 0 || task.compareTo("status 1") == 0)
                {
                    client.print("The realy 1 currently is in ");
                    if (GetReralyStatus(relay1) == HIGH)
                    client.println("on");
                    else
                    client.println("off");
                }
                else if (task.compareTo("Status 2") == 0 || task.compareTo("status 2") == 0)
                {
                    client.print("The realy 2 currently is in ");
                    if (GetReralyStatus(relay2) == HIGH)
                    client.println("on");
                    else
                    client.println("off");
                }
                else
                {
                    client.println("Task not found");
                }
            
                break;
            }
        }
    }

}


int ScanSSID(int ssidSelected)
{
    // WiFi.scanNetworks will return the number of networks found
    int n = WiFi.scanNetworks();
    if (n == 0)
    Serialprintln("no networks found");
    else
    {
        
        Serialprint(n);
        Serialprintln(" networks found");
        Serialprintln("0: Rescan SSID");
        for (int i = 0; i < n; ++i)
        {


            Serialprint(i + 1);
            Serialprint(": ");
            Serialprint(WiFi.SSID(i));
            Serialprint(" (");
            Serialprint(WiFi.RSSI(i));
            Serialprint(")");
            Serialprintln((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");
            delay(10);
        }
        // Serial.print("99: Exit");
        Serialprint("99: Exit");
    }
    // Serial.println("");
    Serialprintln("");

    // Wait a bit before scanning again
    delay(500);

    return 0;
}

String SelectSSID()
{
    int ssidSelected = 0;
    String ssidNumber;

    ScanSSID(ssidSelected);
    Serialprint("Please select by number a SSID: ");

    ssidNumber = readSerialMonitor();
    Serial.println(ssidNumber);

    while (!is_number(ssidNumber) || ssidNumber.toInt() == 0)
    {
        if (ssidNumber.toInt() == 0)
            Serialprintln("Wrong Selection");

        ScanSSID(ssidSelected);
        Serialprint("Please select by number a SSID: ");
        ssidNumber = readSerialMonitor();
        Serialprintln(ssidNumber);
    }
    
    if (ssidNumber.toInt()== 99)
        return "EXIT";
    
    return WiFi.SSID(ssidNumber.toInt());
}


bool ConnectSSID(String ssid, String password)
{
    if (password.isEmpty() == true)
    {
        Serialprint("Enter Password ");
        password = readSerialMonitor();
        password = "CASAD1MAR0T0";
    }
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serialprint(".");
    }
    if(WiFi.status() == WL_CONNECTED)
    {
        File ssidFile = OpenFile("SSID.csv");
        ssidFile.print(ssid);
        ssidFile.print(",");
        ssidFile.print(password);
        CloseFile(ssidFile);
    }

    Serialprintln();
    Serialprint("Connect to WiFi ");
    Serialprintln(ssid);
    Serialprint("IP Addres ");
    
    IPAddress ip_address = WiFi.localIP();
    String ip_address_str = String(ip_address[0]) + "." + String(ip_address[1]) + "." + String(ip_address[2]) + "." + String(ip_address[3]);
    Serialprintln(ip_address_str);

    return true;
}