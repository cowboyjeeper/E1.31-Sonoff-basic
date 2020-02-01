
const char* PARAM_SSID = "ssid";
const char* PARAM_KEY = "key";
const char* PARAM_MULTICAST = "MULTI";
const char* PARAM_UNIVERSE = "universe";
const char* PARAM_CHANNEL = "channel";
const char* PARAM_DHCP = "DHCP";
const char* PARAM_IP_A = "ip_a";
const char* PARAM_IP_B = "ip_b";
const char* PARAM_IP_C = "ip_c";
const char* PARAM_IP_D = "ip_d";
const char* PARAM_SUBNET_A = "sub_a";
const char* PARAM_SUBNET_B = "sub_b";
const char* PARAM_SUBNET_C = "sub_c";
const char* PARAM_SUBNET_D = "sub_d";
const char* PARAM_GATEWAY_A = "gate_a";
const char* PARAM_GATEWAY_B = "gate_b";
const char* PARAM_GATEWAY_C = "gate_c";
const char* PARAM_GATEWAY_D = "gate_d";

char htmlBuffer[3000];  // large buffer for my dynamic page builds

ESP8266WebServer server(80);

void updateMainPage()
{
  IPAddress ipOld = WiFi.localIP();
  IPAddress subnetOld = WiFi.subnetMask();
  IPAddress gatewayOld = WiFi.gatewayIP();
  
  // a IP of 0.0.0.0 causes the WiFi module to do DHCP
  bool checked = g_PtrEeprom->ip[0] == g_PtrEeprom->ip[1] && g_PtrEeprom->ip[1] == g_PtrEeprom->ip[2] && g_PtrEeprom->ip[2] == g_PtrEeprom->ip[3] && g_PtrEeprom->ip[0] == 0;
  
  memset(htmlBuffer, 0, sizeof(htmlBuffer));
  
    snprintf(htmlBuffer, sizeof(htmlBuffer) - 1,
  
"<html>\
<script>\
function toggleMulticast(_checked) {var cells = document.getElementsByClassName('multi');for (var i = 0; i < cells.length; i++) {cells[i].disabled = _checked ? false:true;}}\
function toggleDisabled(_checked) {var cells = document.getElementsByClassName('dhcp');for (var i = 0; i < cells.length; i++) {cells[i].disabled = _checked ? true:false;}}\
</script><style>\
input[type=number]{width:32px;padding:0px}\
input::-webkit-outer-spin-button, input::-webkit-inner-spin-button {-webkit-appearance:none;margin:0;}\
div.max_width {max-width:400px;margin:auto;padding:0px;border:3px solid #73AD21;}\
td:nth-child(1) {width: 120px; text-align: right;}\
</style><body>\
<div class=\"max_width\" align=\"center\">\
<h1>Sonoff settings</h1>\
<form action=\"/action_page.php\" method=\"post\">\
<br>%s<br><br>\
<table>\
<tr><td>Multicast enabled:</td><td><input type=\"checkbox\" name=\"%s\" onchange=\"toggleMulticast(this.checked)\" value=\"%s\" %s></td></tr>\
<tr><td>Universe:</td><td><input type=\"number\" name=\"%s\" min=\"1\" max=\"999\" value=\"%u\"></td></tr>\
<tr><td>Channel:</td><td><input type=\"number\" name=\"%s\" min=\"1\" max=\"255\" value=\"%u\"class=\"multi\" %s></td></tr>\
<tr><td colspan=\"2\"></td></tr>\
<tr><td>SSID:</td><td><input type=\"text\" name=\"%s\" value=\"%s\"></td><tr>\
<tr><td>Password:</td><td><input type=\"text\" name=\"%s\" value=\"%s\"></td><tr>\
<tr><td colspan=\"2\"></td></tr>\
<tr><td>DHCP Enabled:</td><td><input type=\"checkbox\" name=\"%s\" onchange=\"toggleDisabled(this.checked)\"  value=\"%s\" %s></td></tr>\
<tr><td>IP Address:</td><td>\
<input type=\"number\" name=\"%s\" min=\"0\" max=\"256\" value=\"%u\" class=\"dhcp\" %s>\
<input type=\"number\" name=\"%s\" min=\"0\" max=\"256\" value=\"%u\" class=\"dhcp\" %s>\
<input type=\"number\" name=\"%s\" min=\"0\" max=\"256\" value=\"%u\" class=\"dhcp\" %s>\
<input type=\"number\" name=\"%s\" min=\"0\" max=\"256\" value=\"%u\" class=\"dhcp\" %s>\
</td></tr>\
<tr><td>Subnet mask:</td><td>\
<input type=\"number\" name=\"%s\" min=\"0\" max=\"256\" value=\"%u\" class=\"dhcp\" %s>\
<input type=\"number\" name=\"%s\" min=\"0\" max=\"256\" value=\"%u\" class=\"dhcp\" %s>\
<input type=\"number\" name=\"%s\" min=\"0\" max=\"256\" value=\"%u\" class=\"dhcp\" %s>\
<input type=\"number\" name=\"%s\" min=\"0\" max=\"256\" value=\"%u\" class=\"dhcp\" %s>\
</td></tr>\
<tr><td>Gateway:</td><td>\
<input type=\"number\" name=\"%s\" min=\"0\" max=\"256\" value=\"%u\" class=\"dhcp\" %s>\
<input type=\"number\" name=\"%s\" min=\"0\" max=\"256\" value=\"%u\" class=\"dhcp\" %s>\
<input type=\"number\" name=\"%s\" min=\"0\" max=\"256\" value=\"%u\" class=\"dhcp\" %s>\
<input type=\"number\" name=\"%s\" min=\"0\" max=\"256\" value=\"%u\" class=\"dhcp\" %s>\
</td></tr>\
</table>\
<br><br><INPUT type=\"submit\" value=\"Update Settings\">\
</form>\
</div></body>\
</html>",

            MULTICAST_ON ? "Running in multicast mode":"Running in unicast mode",
            PARAM_MULTICAST, PARAM_MULTICAST, MULTICAST_ON ? "checked":"",
            PARAM_UNIVERSE, MULTICAST_ON ? g_PtrEeprom->e131_universe:1,
            PARAM_CHANNEL, MULTICAST_ON ? g_PtrEeprom->e131_channel:1, MULTICAST_ON ? "":"disabled",
            PARAM_SSID, g_PtrEeprom->ssid,
            PARAM_KEY, g_PtrEeprom->passphrase,
            PARAM_DHCP, PARAM_DHCP, checked ? "checked":"",
            PARAM_IP_A, ipOld[0], checked ? "disabled":"",
            PARAM_IP_B, ipOld[1], checked ? "disabled":"",
            PARAM_IP_C, ipOld[2], checked ? "disabled":"",
            PARAM_IP_D, ipOld[3], checked ? "disabled":"",
            PARAM_SUBNET_A, subnetOld[0], checked ? "disabled":"", 
            PARAM_SUBNET_B, subnetOld[1], checked ? "disabled":"", 
            PARAM_SUBNET_C, subnetOld[2], checked ? "disabled":"", 
            PARAM_SUBNET_D, subnetOld[3], checked ? "disabled":"",
            PARAM_GATEWAY_A, gatewayOld[0], checked ? "disabled":"", 
            PARAM_GATEWAY_B, gatewayOld[1], checked ? "disabled":"", 
            PARAM_GATEWAY_C, gatewayOld[2], checked ? "disabled":"", 
            PARAM_GATEWAY_D, gatewayOld[3], checked ? "disabled":"");
}

uint32_t getServerArgNum(const char* PtrArgName, uint32_t CurrentValue)
{
  String serverArg;
  
  if (server.hasArg(PtrArgName))
  {
    serverArg = server.arg(PtrArgName);
    CurrentValue = serverArg.toInt();
  }
  else
  {
    Serial.printf("arg not found: %s", PtrArgName);
    Serial.println("");
  }

  return CurrentValue;
}

void getServerArgString(const char* PtrArgName, char* PtrDestination, size_t DestSize)
{
  String serverArg;
  
  if (server.hasArg(PtrArgName))
  {
    serverArg = server.arg(PtrArgName);
    strncpy(PtrDestination, serverArg.c_str(), DestSize);
  }
  else
  {
    Serial.printf("arg not found: %s", PtrArgName);
    Serial.println("");
  }
}

void handleRoot()
{
  updateMainPage();
  server.send(200, "text/html", htmlBuffer);
}

void handlePhp()
{  
  bool oldMulticastMode = MULTICAST_ON;
  bool usedToBeDhcp = g_PtrEeprom->ip[0] == g_PtrEeprom->ip[1] && g_PtrEeprom->ip[1] == g_PtrEeprom->ip[2] && g_PtrEeprom->ip[2] == g_PtrEeprom->ip[3] && g_PtrEeprom->ip[0] == 0;
  bool dhcpChecked = false;
    
  g_PtrEeprom->flags = server.hasArg(PARAM_MULTICAST) ? (g_PtrEeprom->flags | FLAG_MULTICAST_MODE):(g_PtrEeprom->flags & ~FLAG_MULTICAST_MODE);
  dhcpChecked = server.hasArg(PARAM_DHCP);
  g_PtrEeprom->e131_universe = getServerArgNum(PARAM_UNIVERSE, g_PtrEeprom->e131_universe);
  g_PtrEeprom->e131_channel = getServerArgNum(PARAM_CHANNEL, g_PtrEeprom->e131_channel);
  getServerArgString(PARAM_SSID, g_PtrEeprom->ssid, sizeof(g_PtrEeprom->ssid));
  getServerArgString(PARAM_KEY, g_PtrEeprom->passphrase, sizeof(g_PtrEeprom->passphrase));
  if(dhcpChecked)
  {
    memset(g_PtrEeprom->ip, 0, sizeof(g_PtrEeprom->ip));
    memset(g_PtrEeprom->subnet, 0, sizeof(g_PtrEeprom->subnet));
    memset(g_PtrEeprom->gateway, 0, sizeof(g_PtrEeprom->gateway));
  }
  else
  {
    g_PtrEeprom->ip[0] = getServerArgNum(PARAM_IP_A, g_PtrEeprom->ip[0]);
    g_PtrEeprom->ip[1] = getServerArgNum(PARAM_IP_B, g_PtrEeprom->ip[1]);
    g_PtrEeprom->ip[2] = getServerArgNum(PARAM_IP_C, g_PtrEeprom->ip[2]);
    g_PtrEeprom->ip[3] = getServerArgNum(PARAM_IP_D, g_PtrEeprom->ip[3]);
    g_PtrEeprom->subnet[0] = getServerArgNum(PARAM_SUBNET_A, g_PtrEeprom->subnet[0]);
    g_PtrEeprom->subnet[1] = getServerArgNum(PARAM_SUBNET_B, g_PtrEeprom->subnet[1]);
    g_PtrEeprom->subnet[2] = getServerArgNum(PARAM_SUBNET_C, g_PtrEeprom->subnet[2]);
    g_PtrEeprom->subnet[3] = getServerArgNum(PARAM_SUBNET_D, g_PtrEeprom->subnet[3]);
    g_PtrEeprom->gateway[0] = getServerArgNum(PARAM_GATEWAY_A, g_PtrEeprom->gateway[0]);
    g_PtrEeprom->gateway[1] = getServerArgNum(PARAM_GATEWAY_B, g_PtrEeprom->gateway[1]);
    g_PtrEeprom->gateway[2] = getServerArgNum(PARAM_GATEWAY_C, g_PtrEeprom->gateway[2]);
    g_PtrEeprom->gateway[3] = getServerArgNum(PARAM_GATEWAY_D, g_PtrEeprom->gateway[3]);
  }

  eeprom_commit();
  
  IPAddress ip(g_PtrEeprom->ip[0], g_PtrEeprom->ip[1], g_PtrEeprom->ip[2], g_PtrEeprom->ip[3]);
  IPAddress subnet(g_PtrEeprom->subnet[0], g_PtrEeprom->subnet[1], g_PtrEeprom->subnet[2], g_PtrEeprom->subnet[3]);
  if(oldMulticastMode != MULTICAST_ON)
  {
    if(subnet != WiFi.subnetMask() && !dhcpChecked)
      snprintf(htmlBuffer, sizeof(htmlBuffer) - 1, "<h2><center>Please connect to your new IP address %s and Subnet %s</center></h2>", ip.toString().c_str(), subnet.toString().c_str());
    else if(usedToBeDhcp != dhcpChecked && dhcpChecked)
      snprintf(htmlBuffer, sizeof(htmlBuffer) - 1, "<h2><center>You've changed to DHCP, please connect to the new address the DHCP server provides.</center></h2>");
    else
      snprintf(htmlBuffer, sizeof(htmlBuffer) - 1, "<html><meta http-equiv=\"refresh\" content=\"15;url=http://%s/\" /><h1>Reboot required, attempting to reconnect to %s</h1></html>", ip.toString().c_str(), ip.toString().c_str());
    delay(1000);
    ESP.reset();  // the E131 isn't changing correctly, so we have to reboot to change mode!
  }
  else
  {
    if(ip != WiFi.localIP() || subnet != WiFi.subnetMask() || usedToBeDhcp != dhcpChecked) // compare current and old IP in non-dhcp scenerio
    {
      Serial.print("IP update in progress: ");
      Serial.println(ip);
      if(subnet != WiFi.subnetMask() && !dhcpChecked)
        snprintf(htmlBuffer, sizeof(htmlBuffer) - 1, "<h2><center>Please connect to your new IP address %s and Subnet %s</center></h2>", ip.toString().c_str(), subnet.toString().c_str());
      else if(usedToBeDhcp != dhcpChecked && dhcpChecked)
        snprintf(htmlBuffer, sizeof(htmlBuffer) - 1, "<h2><center>You've changed to DHCP, please connect to the new address the DHCP server provides.</center></h2>");
      else
        snprintf(htmlBuffer, sizeof(htmlBuffer) - 1, "<meta http-equiv=\"refresh\" content=\"5;url=http://%s/\" /><h2><center>Attempting redirect to %s</center></h2>", ip.toString().c_str(), ip.toString().c_str());
      server.send(200, "text/html", htmlBuffer);
      delay(500);
      init_wifi();
    }
    
    snprintf(htmlBuffer, sizeof(htmlBuffer) - 1, "<meta http-equiv=\"refresh\" content=\"0;url=http://%s/\" />", ip.toString().c_str());
    server.send(200, "text/html", htmlBuffer);
  }
}

void handleNotFound()
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup_web()
{
  server.on("/", handleRoot);
  server.on("/action_page.php", handlePhp);
  server.onNotFound(handleNotFound);
  server.begin();
}

void loop_web()
{
  server.handleClient();
}
