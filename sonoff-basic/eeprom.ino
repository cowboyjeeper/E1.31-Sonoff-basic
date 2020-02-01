
#define DEFAULT_SSID      ("SONONFF")
#define DEFAULT_PASS      ("123465")

eeprom_t s_EepromData;

void eeprom_default()
{
  memset(&s_EepromData, 0, sizeof(s_EepromData));

  WiFi.macAddress(s_EepromData.mac);
  s_EepromData.flags = FLAG_MULTICAST_MODE;
  s_EepromData.e131_universe = 1;
  s_EepromData.e131_channel = 1;
  strncpy(s_EepromData.host, WiFi.hostname().c_str(), sizeof(s_EepromData.host) - 1);
  strncpy(s_EepromData.ssid, DEFAULT_SSID, sizeof(s_EepromData.ssid) - 1);
  strncpy(s_EepromData.passphrase, DEFAULT_PASS, sizeof(s_EepromData.passphrase) - 1);
  EEPROM.put(0, s_EepromData);
  EEPROM.commit();
}

void eeprom_commit()
{     
  EEPROM.put(0, s_EepromData);
  EEPROM.commit();
}

void setup_eeprom()
{
  byte mac[6];
  EEPROM.begin(sizeof(s_EepromData));
  EEPROM.get(0, s_EepromData);

  g_PtrEeprom = &s_EepromData;
  
  WiFi.macAddress(mac);
  if(memcmp(mac, s_EepromData.mac, sizeof(mac)))
  {
    eeprom_default();
  }  
}
