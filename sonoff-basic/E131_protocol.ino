
#define UNIVERSE              (g_PtrEeprom->e131_universe)
#define CHANNEL               (g_PtrEeprom->e131_channel)
#define RELAY_GPIO            (12)                        // GPIO pin used to toggle the relay state

bool s_multicast_on = false;

ESPAsyncE131 e131(1);       // this is a very basic controller, only one channel so it only needs one universe!

uint32_t s_relay = 0;
uint32_t s_last_relay = s_relay;

void setup_e131()
{
  int retVal;
  char line[81] = {0};

  s_multicast_on = MULTICAST_ON;
  pinMode(RELAY_GPIO, OUTPUT);
  digitalWrite(RELAY_GPIO, LOW);  // we are off until we are told to be on!
  
  // Choose one to begin listening for E1.31 data
  if(s_multicast_on)
  {
      snprintf(line, sizeof(line) - 1, "Starting multicast listener:  Universe %d count %d", UNIVERSE, 1);
      Serial.println(line);
      retVal = e131.begin(E131_MULTICAST, UNIVERSE, 1);
  }
  else
  {
      Serial.println("Starting unicast listener");
      retVal = e131.begin(E131_UNICAST);
  }

  if (retVal)
      Serial.println(F("Listening for data..."));
  else 
      Serial.println(F("*** FAILED TO START E1.31 LISTENER ***"));
}

void loop_e131() 
{
  e131_packet_t packet;
  uint32_t relay_universe = UNIVERSE;
  uint32_t relay_channel = s_multicast_on ? CHANNEL:1;
  uint32_t e131_universe;
  
  if (!e131.isEmpty()) 
  {
    e131.pull(&packet);     // Pull packet from ring buffer

    e131_universe = htons(packet.universe);
    
    if(e131_universe == relay_universe)
    {
      s_relay = packet.property_values[relay_channel];
             
      if(s_relay != s_last_relay) 
      {
        digitalWrite(RELAY_GPIO, s_relay ? HIGH:LOW);
        s_last_relay = s_relay;
      
        Serial.printf("%s (%u / %u) Packet#: %u / Errors: %u/ Using Channel: %u / value: %u\r\n",
                s_multicast_on ? "Multicast":"Unicast",
                e131_universe,                          // The Universe for this packet
                htons(packet.property_value_count) - 1, // Start code is ignored, we're interested in dimmer data
                e131.stats.num_packets,                 // Packet counter
                e131.stats.packet_errors,               // Packet error counter
                relay_channel,
                s_relay);                               // Dimmer data for Channel
      }
    }
  }
}
