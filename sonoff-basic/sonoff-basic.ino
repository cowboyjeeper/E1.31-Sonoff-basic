//requires ESPAsyncUDP:         https://github.com/me-no-dev/ESPAsyncUDP
#include <ESP8266WiFi.h>        //If you get an error here you need to install the ESP8266 board manager
//#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESPAsyncE131.h>       //https://github.com/forkineye/ESPAsyncE131
#include <ArduinoOTA.h>         //https://github.com/esp8266/Arduino/tree/master/libraries/ArduinoOTA
#include <EEPROM.h>

#define SOFT_AP_SSID          ("SONOFF")
#define WIFI_TIMEOUT_SEC      (15)
#define BAUD_SERIAL           (115200)
#define VERSION               ("01.00.00")

#define FLAG_MULTICAST_MODE       (0x00000001)
#define FLAG_DHCP                 (0x00000002)

#define DHCP_ON                    (FLAG_DHCP & s_PtrEeprom->flags ? true:false)
#define MULTICAST_ON               (FLAG_MULTICAST_MODE & s_PtrEeprom->flags ? true:false)

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

eeprom_t *s_PtrEeprom;
unsigned long previousMillisLed = 0;     // for GPIO time check
uint32_t secondsForGpio      = 0;        // approximate number of seconds GPIO 0 was pressed

String get_mac()
{
  byte mac[6];
  char buffer[81] = {0};
  
  WiFi.macAddress(mac);
  snprintf(buffer, sizeof(buffer) -1, "%X:%X:%X:%X:%X:%X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  return String(buffer);
}

void setup_wifi()
{
  unsigned long beginTime = 0;
  IPAddress ip(s_PtrEeprom->ip[0], s_PtrEeprom->ip[1], s_PtrEeprom->ip[2], s_PtrEeprom->ip[3]);
  IPAddress subnet(s_PtrEeprom->subnet[0], s_PtrEeprom->subnet[1], s_PtrEeprom->subnet[2], s_PtrEeprom->subnet[3]);
  IPAddress gateway(s_PtrEeprom->gateway[0], s_PtrEeprom->gateway[1], s_PtrEeprom->gateway[2], s_PtrEeprom->gateway[3]);
      
  WiFi.softAPdisconnect(true);  // turn off the soft ap, if on
  WiFi.disconnect();
  
  WiFi.mode(WIFI_STA);                  // Make sure you're in station mode   
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
  WiFi.begin(s_PtrEeprom->ssid, s_PtrEeprom->passphrase);
  WiFi.hostname(s_PtrEeprom->host);
  WiFi.config(ip, subnet, gateway);
  beginTime = millis();
  Serial.printf("\nConnecting to %s\r\n", s_PtrEeprom->ssid);
  while (WiFi.status() != WL_CONNECTED && !((millis() - beginTime) > (1000 * WIFI_TIMEOUT_SEC))) 
  {
    delay(500);
    Serial.print(".");
  }

  if(WiFi.status() == WL_CONNECTED)
  {
    Serial.println("\n");
    Serial.printf("%20s: %s\r\n", "SSID", s_PtrEeprom->ssid);
    Serial.printf("%20s: %s\r\n", "MAC address", get_mac().c_str());
    Serial.printf("%20s: %s\r\n", "Hostname", WiFi.hostname().c_str());
    Serial.printf("%20s: ", "IP address");
    Serial.println( WiFi.localIP());
    Serial.printf("%20s: ", "IP address");
    Serial.println(ip);
    Serial.printf("%20s: ", "Subnet mask");
    Serial.println( WiFi.subnetMask());
    Serial.printf("%20s: ", "Gateway");
    Serial.println( WiFi.gatewayIP());
    Serial.printf("%20s: ", "DNS");
    Serial.println( WiFi.dnsIP());
    Serial.println("");
  }
  else
  {
    Serial.println("");
    Serial.println("Failed to connect wifi, default to AP mode");
    // failed to connect to the WIFI, going into AP mode for reconfiguration
    WiFi.disconnect();
    WiFi.mode(WIFI_AP);                  // Make sure you're in station mode  
    WiFi.softAP(SOFT_AP_SSID);
    Serial.print("IP address: ");
    Serial.println(WiFi.softAPIP());
  }
}

void setup() 
{
  Serial.begin(BAUD_SERIAL);
  
  setup_eeprom();
  setup_wifi();
  setup_e131();
  setup_web();
  
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

void loop() 
{
  loop_e131();
  loop_web();
  
  ArduinoOTA.handle();
}
