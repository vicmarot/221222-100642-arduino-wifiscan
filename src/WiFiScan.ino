/*
 *  This sketch demonstrates how to scan WiFi networks. 
 *  The API is almost the same as with the WiFi Shield library, 
 *  the most obvious difference being the different file you need to include:
 *  ./python3.exe -I "C:\Users\marotovi\AppData\Local\Arduino15\packages\esp8266\hardware\esp8266\3.0.2/tools/espota.py" -i "192.168.3.32" -p "8266" "--auth=password" -f "C:\Users\marotovi\Documents\PlatformIO\Projects\221222-100642-arduino-wifiscan\.pio\build\espDev\firmware.bin"
 */
#include "WifiModule.hpp"
#include "SerialModule.hpp"
#include "UtilsModule.hpp"
#include "GPIO.hpp"
#include "SerialWifiModule.hpp"
#include "Define.hpp"


#include <ArduinoOTA.h>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiClientSecure.h>
#include <ESP8266WebServerSecure.h>


const int HTTPServerPort = 80;
const int HTTPSServerPort = 443;
const int OTAPort = 8266;

WiFiServer HTTPServer(HTTPServerPort);
WiFiServerSecure HTTPSServer (HTTPSServerPort);



static const char serverCert[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDnzCCAocCFCg7zzfdykdVjoXNaaQDhmBaMZDEMA0GCSqGSIb3DQEBCwUAMIGL
MQswCQYDVQQGEwJFUzEPMA0GA1UEBwwGTWFkcmlkMRUwEwYDVQQKDAxPcmdhbml6
YXRpb24xHDAaBgNVBAsME09yZ2FuaXphdGlvbmFsIFVuaXQxHTAbBgkqhkiG9w0B
CQEWDnlvdXJAZW1haWwuY29tMRcwFQYDVQQDDA55b3VyZG9tYWluLmNvbTAeFw0y
MzAxMDQxMDE0MDNaFw0yNDAxMDQxMDE0MDNaMIGLMQswCQYDVQQGEwJFUzEPMA0G
A1UEBwwGTWFkcmlkMRUwEwYDVQQKDAxPcmdhbml6YXRpb24xHDAaBgNVBAsME09y
Z2FuaXphdGlvbmFsIFVuaXQxHTAbBgkqhkiG9w0BCQEWDnlvdXJAZW1haWwuY29t
MRcwFQYDVQQDDA55b3VyZG9tYWluLmNvbTCCASIwDQYJKoZIhvcNAQEBBQADggEP
ADCCAQoCggEBAK3veOo8mrpfuA32udfBdeT2HGjRPL4nC2nhmm3GQFoGci6VBSAD
TJT2e3rliO/9TthRiIeO2JVCiG8jw4w5Gqw2/4lTjht+q5mtGrr3l42KDjuyOvBe
JszXQI/3xXxEHzJaWVMpCySD/i4RbzyPxNZZ8NtBWt6ybzbYJL1q4LUpF4B+KwHg
h+Tmb64sdKav1vceooXdZpLZEHk/D0Z2akl7vmYdGtYJsQaYMBFKNVymhQoOCWWk
uxXy3MZFsrogNZ5DZPFZz6L9cz6SZXhLYK4dqPfCYFBT83KsvYH4E/4pH7zUnZBv
1CBVo71/SLA402pERsORU91aCS7YvqX9ZM8CAwEAATANBgkqhkiG9w0BAQsFAAOC
AQEAoErmHb4MAiOS/cjq0/WsDuEz8DZuEGRvEndZRdsGcrK0UyzqusDniQvCJiIF
4LzOkdvZoiiDPjf+u9sy7ki8sEFG3pNpL/Z0yY6KQoHcSiq33yZTYuIszqChQiPi
lTCRGA6ayUjnuirYihy+gi8+0QOL/kfvctFDN1lnLy1EzLYgt8j/a+OFP3y5kwir
XAcaPaaufnjO8BlvshIPQReu2kZeCXju6B6DaawKLiolm7LE/gJkNNl29+dESQqg
MNidJ5oNGDEjv0suzSwsDiCvjdeKNJX+bJQUXv/inKCb2ve8V5dXPQOHF93AsGgC
iJ/XyFFApUoCF2KTqmfTN7OZUg==
-----END CERTIFICATE-----
)EOF";

static const char serverKey[] PROGMEM = R"EOF(
-----BEGIN RSA PRIVATE KEY-----
MIIEowIBAAKCAQEAre946jyaul+4Dfa518F15PYcaNE8vicLaeGabcZAWgZyLpUF
IANMlPZ7euWI7/1O2FGIh47YlUKIbyPDjDkarDb/iVOOG36rma0auveXjYoOO7I6
8F4mzNdAj/fFfEQfMlpZUykLJIP+LhFvPI/E1lnw20Fa3rJvNtgkvWrgtSkXgH4r
AeCH5OZvrix0pq/W9x6ihd1mktkQeT8PRnZqSXu+Zh0a1gmxBpgwEUo1XKaFCg4J
ZaS7FfLcxkWyuiA1nkNk8VnPov1zPpJleEtgrh2o98JgUFPzcqy9gfgT/ikfvNSd
kG/UIFWjvX9IsDjTakRGw5FT3VoJLti+pf1kzwIDAQABAoIBACcA1DRycp/oZvV6
piHjl/RH3AcFJIHfb4fGt0J2eme1mlUnxYZwNvQQcZAVeKeYPd30KpF7TsXprX5+
npMLGxgTHSpGOk+gEArd7miOrMVod9ibFuxfsIOuoSOng/lhrT01VWJ4ia8eihj6
f98svg7zDDeeaCoYCShbP6HaHZmBxrT75HACEdfNf8pQWSoLPhvq8WoFbTdXZjNy
a7+flAxQvF27JabAcf+H6zzjnZSdWnelQsajmlO7b5gRb40t1wZyJPZSLrpBt9Fl
jhvg3u9WoR0JAAYPawwiaah1CtudrnrvVHKxYqWiKuR5RYeU36lmKkv/337yjAaG
qeFU2kECgYEA25W9ZFICaufXZ2SU5tfIwGFVSJs8NsUuvD5XspPCZ7cvdvRItPDs
AB3HSiKalTy2Pnr/dFyoMT7sWpEocLHVzaQXZG7994mfXYLzzrgpvusAKT1qOMbr
2HPNvxCFWJYAbGg7qN4xdXpgat4xZRHQO6KcfTy4da3tj+g+GiU2QNMCgYEAyse6
np+WutkdrAk01AC3SS/CJq+P629hEdilUhI+m2EFeTjXQ/982/irXNwr47WOvcMK
xv8ezbf81cLStU4nflbTCE4dUu9pwTpXYr7BbRTSPOlpEWUu8xPRp28aCvf3eILD
VNFbrmDArNdtfQlktD0r+SY8B5hTDxELPIrVbpUCgYEAxnuwqQi0JLwR6eRx78On
M7YHZ8zh/cP4vbFAhiunsyZTYhvnyJ5pRKWmKW3j+vfY59s4I49JfNIaxLanLk6h
KvOjLWZibClbAyTGitnO6kLzb1OTO2UbdBkhJg6h6fQUx+jj3H2TiVQn6RGU5n3W
ksEm4NFNDxo7PTQ5RlrSkn8CgYArR94JJT7iupyer4hbB5wXPt5Nq4MoWSl6yabf
kY1TVK36ZatslAduW/EXbNbhT0scLZkLSECm3/JTHbYjhH7hdF6ukx2nKfqEh5NH
PDfEtID7B0aps9MFhuFkv03rsWlci2xtmhHbckMtoP15hpu77KKKFyMQ6+4udRDU
5z0O0QKBgAdD3WQpMkj9g66RfF0HAHmmBYn81XMWoUr6rWc77bHcZizaZ1POqdEl
FME6CsjLDACrLzQEP8ASYD/mzbOuANqp+E1RBNF5qiAZgFOySAOPpOzjDsjwOWM0
pIjZegJLDm54j+mLrdCZ+qP3zfbPkb2k/iD7ZiekiPRHXyJ0LO8u
-----END RSA PRIVATE KEY-----
)EOF";

void setup() {

  Serial.begin(115200);

  HTTPServer.begin();
  
  HTTPSServer.setRSACert(new BearSSL::X509List(serverCert), new BearSSL::PrivateKey(serverKey));
  HTTPSServer.begin();

  InitIFS();

  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  /* TODO Put in a function*/
  String connectionData = ReadFile("SSID.csv");
  if (connectionData.isEmpty())
    ConnectSSID(SelectSSID(), "");
  else
  {
    ConnectSSID(GetSSIDFromFile(), GetPasswordFromFile());
  }

  InitGPIO();

  ArduinoOTA.setPort(OTAPort);
  ArduinoOTA.setHostname("my-esp8266");
  ArduinoOTA.setPassword("password");
  ArduinoOTA.begin();
  
  

  // Serial.println("Setup done");
  Serialprintln("Setup done");
  
}


void loop() {
 
  // Serial.println("WAITING FOR CONNECTION");
  Serialprintln("WAITING FOR CONNECTION");
  #ifdef DEBUG
  delay(5000);
  #endif
  WiFiClient client = HTTPServer.available();
  InCommingClients(client);

  WiFiClientSecure clientSecure = HTTPSServer.available();
  InCommingSecureClients(clientSecure);

  ArduinoOTA.handle();
}
