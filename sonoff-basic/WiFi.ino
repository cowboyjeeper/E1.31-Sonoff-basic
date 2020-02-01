/*
 * This is a module to control and connect to a WiFi network.
 * 
 * It attempts to connect to the specified STA_SSID, if it cannot after WIFI_TIMEOUT_SEC seconds
 * it will create a wifi network of its own by the name of SOFT_AP_SSID while continuing to 
 * attempt to connect to STA_SSID.
 * 
 * If a connection is establisted to the STA_SSID after the SOFT_AP_SSID creation it will 
 * disconnect the SOFT_AP_SSID.
 * 
 * The temprorary SOFT_AP_SSID may be used to setup configurations as needed.  However the 
 * reconnect is allowed to handle possiblity of power outages on the wifi network.
 * 
 */

#define STA_SSID                      g_PtrEeprom->ssid
#define STA_PSK                       g_PtrEeprom->passphrase
#define STA_HOST                      g_PtrEeprom->host
#define SOFT_AP_SSID                  "SONOFF"

#define WIFI_AP_BLINK_RATE            (500)
#define WIFI_TIMEOUT_SEC              (15)
#define WIFI_ACTIVE_GPIO              (13)          // if you don't have a GPIO, then simply undefine this
#define WIFI_ACTIVE_GPIO_ON           (LOW)         // this is an inverted GPIO, so on = LOW
#define WIFI_ACTIVE_GPIO_OFF          (HIGH)        // this is an inverted GPIO, so of = HIGH

WiFiEventHandler s_ConnectHandler;
WiFiEventHandler s_DisConnectHandler;
bool s_apMode = false;
unsigned long s_ledTimer = 0;

void wifi_hdlr_connect(const WiFiEventStationModeGotIP &event) 
{
  WiFi.softAPdisconnect(true);  // if the network was started we don't need the soft AP anymore!
  WiFi.mode(WIFI_STA);  
  
  WiFi.hostname(STA_HOST);
  
  Serial.println("\n");
  Serial.printf("%20s: %s\r\n", "SSID", WiFi.SSID().c_str());
  Serial.printf("%20s: %s\r\n", "Hostname", WiFi.hostname().c_str());
  Serial.printf("%20s: ", "IP address");
  Serial.println(event.ip);
  Serial.printf("%20s: ", "Subnet mask");
  Serial.println( event.mask);
  Serial.printf("%20s: ", "Gateway");
  Serial.println( event.gw);
  Serial.println("");

  #ifdef WIFI_ACTIVE_GPIO
    digitalWrite(WIFI_ACTIVE_GPIO, WIFI_ACTIVE_GPIO_ON);
  #endif

  s_apMode = false;
}

void wifi_hdlr_disconnect(const WiFiEventStationModeDisconnected &event) 
{  
  #ifdef WIFI_ACTIVE_GPIO
    digitalWrite(WIFI_ACTIVE_GPIO, WIFI_ACTIVE_GPIO_OFF);
  #endif
}

void init_wifi()
{
  IPAddress ip(g_PtrEeprom->ip[0], g_PtrEeprom->ip[1], g_PtrEeprom->ip[2], g_PtrEeprom->ip[3]);
  IPAddress subnet(g_PtrEeprom->subnet[0], g_PtrEeprom->subnet[1], g_PtrEeprom->subnet[2], g_PtrEeprom->subnet[3]);
  IPAddress gateway(g_PtrEeprom->gateway[0], g_PtrEeprom->gateway[1], g_PtrEeprom->gateway[2], g_PtrEeprom->gateway[3]);
  
  unsigned long beginTime = 0;
  char ap_ssid[80];
  
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);                      // Make sure you're in station mode   
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
  WiFi.begin(STA_SSID, STA_PSK);
  WiFi.config(ip, subnet, gateway);
  beginTime = millis();
  Serial.printf("\nConnecting to %s\r\n", STA_SSID);
  while (WiFi.status() != WL_CONNECTED && !((millis() - beginTime) > (1000 * WIFI_TIMEOUT_SEC))) 
  {
    Serial.print(".");
    delay(500);
  }

  if(WiFi.status() != WL_CONNECTED)
  {
    Serial.println("");
    Serial.printf("Failed to connect to SSID: %s", STA_SSID);
    Serial.println("");
    Serial.println("Defaulting to AP mode");
    // failed to connect to the WIFI, going into AP mode for reconfiguration
    WiFi.mode(WIFI_AP_STA); 
    snprintf(ap_ssid, sizeof(ap_ssid) - 1, "%s-%02X%02X%02X%02X%02X%02X", SOFT_AP_SSID, g_PtrEeprom->mac[0], g_PtrEeprom->mac[1], g_PtrEeprom->mac[2], g_PtrEeprom->mac[3], g_PtrEeprom->mac[4], g_PtrEeprom->mac[5]);
    WiFi.softAP(ap_ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.softAPIP());
    s_apMode = true;
  }
}

void setup_wifi()
{    
  // Setup WiFi Handlers
  s_ConnectHandler = WiFi.onStationModeGotIP(wifi_hdlr_connect);
  s_DisConnectHandler = WiFi.onStationModeDisconnected(wifi_hdlr_disconnect);
    
#ifdef WIFI_ACTIVE_GPIO
  pinMode(WIFI_ACTIVE_GPIO, OUTPUT);
  digitalWrite(WIFI_ACTIVE_GPIO, WIFI_ACTIVE_GPIO_OFF);
  s_ledTimer = millis();
#endif 
 
  init_wifi();
}

void loop_wifi()
{  
#ifdef WIFI_ACTIVE_GPIO
  if (s_apMode && ((millis() - s_ledTimer) > WIFI_AP_BLINK_RATE))
  {
    s_ledTimer = millis();
    digitalWrite(WIFI_ACTIVE_GPIO, !digitalRead(WIFI_ACTIVE_GPIO));
  }
#endif
}
