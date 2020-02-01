//requires ESPAsyncUDP:         https://github.com/me-no-dev/ESPAsyncUDP
#include <ESP8266WiFi.h>        //If you get an error here you need to install the ESP8266 board manager
//#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESPAsyncE131.h>       //https://github.com/forkineye/ESPAsyncE131
#include <ArduinoOTA.h>         //https://github.com/esp8266/Arduino/tree/master/libraries/ArduinoOTA
#include <EEPROM.h>

#define BAUD_SERIAL           (115200)

#define FLAG_MULTICAST_MODE       (0x00000001)

#define MULTICAST_ON               ((FLAG_MULTICAST_MODE & g_PtrEeprom->flags) == FLAG_MULTICAST_MODE)

typedef struct {
  byte mac[6];         // simply used to detect that this data belongs to this module
  byte pad[2];
  uint32_t flags;
  uint16_t e131_channel;                  // channels max at 255 but alignments better with this
  uint16_t e131_universe;
  uint8_t ip[4];
  uint8_t subnet[4];
  uint8_t gateway[4];
  char host[32];
  char ssid[20];
  char passphrase[32];
} eeprom_t;

eeprom_t *g_PtrEeprom;
unsigned long previousMillisLed = 0;     // for GPIO time check

void setup_ota()
{
  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  
  ArduinoOTA.begin(); 
}

void setup() 
{
  Serial.begin(BAUD_SERIAL);

  delay(500); // give the serial a chance...
    
  setup_eeprom();
  setup_wifi();
  setup_e131();
  setup_web();
  setup_ota();
}

void loop() 
{
  loop_e131();
  loop_web();
  loop_wifi();
  
  ArduinoOTA.handle();
}
