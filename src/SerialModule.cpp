
#include "SerialModule.hpp"


String readSerialMonitor ()
{

    const int bufferSize = 128;
    char inputBuffer[bufferSize];
    int bufferPointer = 0;
    char inByte;
    
    do{
        if (Serial.available())
        {
            inByte = Serial.read();

            if (inByte == '\n') 
                {
                // 'newline' character
                inputBuffer[bufferPointer++] = '\0';
                bufferPointer = 0;
                }
            else
            {
            // not a 'newline' character
            if (bufferPointer < bufferSize - 1)  // Leave room for a null terminator
                inputBuffer[bufferPointer++] = inByte;                
            }
        }
    }while(inByte != '\n');
    
    String output(inputBuffer);
    return output;
}

bool is_number(const String& s)
{
    int resultado = s.toInt();
 
    if (resultado != 0) return true;

    return false;
}