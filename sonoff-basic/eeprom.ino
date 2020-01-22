
#define DEFAULT_SSID      ("SONONFF")
#define DEFAULT_PASS      ("123465")

eeprom_t eepromData;

void eeprom_default()
{
  memset(&eepromData, 0, sizeof(eepromData));

  WiFi.macAddress(eepromData.mac);
  eepromData.flags = FLAG_MULTICAST_MODE;
  eepromData.e131_universe = 1;
  eepromData.e131_channel = 1;
  strncpy(eepromData.host, WiFi.hostname().c_str(), sizeof(eepromData.host) - 1);
  strncpy(eepromData.ssid, DEFAULT_SSID, sizeof(eepromData.ssid) - 1);
  strncpy(eepromData.passphrase, DEFAULT_PASS, sizeof(eepromData.passphrase) - 1);
  EEPROM.put(0, eepromData);
  EEPROM.commit();
}

void eeprom_commit()
{   
  EEPROM.put(0, eepromData);
  EEPROM.commit();
}

void setup_eeprom()
{
  byte mac[6];
  EEPROM.begin(sizeof(eepromData));
  EEPROM.get(0, eepromData);

  s_PtrEeprom = &eepromData;

  WiFi.macAddress(mac);
  if(memcmp(mac, eepromData.mac, sizeof(mac)))
  {
    eeprom_default();
  }  
}
