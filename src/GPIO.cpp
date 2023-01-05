
#include <Arduino.h>
#include "GPIO.hpp"
#include "SerialWiFiModule.hpp"


void InitGPIO()
{
    pinMode(relay1, OUTPUT);
    pinMode(relay2, OUTPUT);
    digitalWrite(relay2, LOW);
}


void SwitchRealytatus(int relay)
{
    /* TODO: check input is a relay */
    Serialprint("voy a apagar el releay ");
    Serialprintln(relay);
    if (digitalRead(relay) == HIGH)
        digitalWrite(relay, LOW);
    else
        digitalWrite(relay, HIGH);
    delay(500);
}

int GetReralyStatus(int relay)
{
    /* TODO: check the input is a relay*/

    return digitalRead(relay);
}