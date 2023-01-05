
#include "UtilsModule.hpp"
#include "SerialWiFiModule.hpp"

String messagehttp= "message=";

void InitIFS()
{
    // We initialise the LittleFS library
    if (!LittleFS.begin()) {
        // Serial.println("Error al inicializar LittleFS");
        Serialprintln("Error al inicializar LittleFS");
    return;
  }
}

File OpenFile (String fileName)
{
    File file = LittleFS.open(fileName , "w");
    if (!file) {
        // Serial.println("Error opening CSV file");
        Serialprintln("Error opening CSV file");
    }
    return file;
}

void CloseFile(File fileName)
{
    fileName.close();
}


String ReadFile (String fileName)
{
    File file = LittleFS.open(fileName , "r");
    if (!file) {
        // Serial.println("Error opening CSV file");
        Serialprintln("Error opening CSV file");
    }
    return file.readStringUntil('\n');
    CloseFile(file);
}

String GetSSIDFromFile()
{
    String connectionData = ReadFile("SSID.csv");
    
    int SeparatorPos = connectionData.indexOf(',');

   return connectionData.substring(0,SeparatorPos);
}

String GetPasswordFromFile()
{
    String connectionData = ReadFile("SSID.csv");
    
    int SeparatorPos = connectionData.indexOf(',');

   return connectionData.substring(SeparatorPos + 1, connectionData.length());
}

String SearchString(String str, String search)
{
    // TODO: manage own errors
    int position = str.indexOf(search);

    if (position == -1)
    {
        return "ERROR";
    }

    return str.substring(position);
   
}

String GetMessage(String str)
{
    String str2;

    str2 = SearchString(str, messagehttp);

    return str2.substring(messagehttp.length());
}

