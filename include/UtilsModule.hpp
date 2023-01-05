

// #include <string.h>
#include <LittleFS.h>

// bool is_number(const String& s);



void InitIFS();
File OpenFile (String fileName);
void CloseFile(File fileName);
String ReadFile (String fileName);
String GetSSIDFromFile();
String GetPasswordFromFile();
String SearchString(String str, String search);
String GetMessage(String str);