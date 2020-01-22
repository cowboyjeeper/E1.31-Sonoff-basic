
const char* PARAM_SSID = "ssid";
const char* PARAM_KEY = "key";
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

char htmlBuffer[2500];  // large buffer for my dynamic page builds

ESP8266WebServer server(80);

void updateMainPage()
{
  IPAddress ipOld = WiFi.localIP();
  IPAddress subnetOld = WiFi.subnetMask();
  IPAddress gatewayOld = WiFi.gatewayIP();
  
  // a IP of 0.0.0.0 causes the WiFi module to do DHCP
  bool checked = s_PtrEeprom->ip[0] == s_PtrEeprom->ip[1] && s_PtrEeprom->ip[1] == s_PtrEeprom->ip[2] && s_PtrEeprom->ip[2] == s_PtrEeprom->ip[3] && s_PtrEeprom->ip[0] == 0;
  
  memset(htmlBuffer, 0, sizeof(htmlBuffer));

  if(MULTICAST_ON)
  {
    snprintf(htmlBuffer, sizeof(htmlBuffer) - 1,
  
"<html>\
<head>\
<title>Sonoff settings</title>\
<script>\
function toggleDisabled(_checked) {var cells = document.getElementsByClassName('dhcp');for (var i = 0; i < cells.length; i++) {cells[i].disabled = _checked ? true:false;}}\
</script>\
<style>input[type=number]{width:32px;padding:0px} input::-webkit-outer-spin-button, input::-webkit-inner-spin-button {-webkit-appearance:none;margin:0; } div.max_width {max-width:375px;margin:auto;padding:0px;border:3px solid #73AD21;}</style>\
</head>\
<body><div class=\"max_width\" align=\"center\">\
<h1>Sonoff settings</h1>\
<form action=\"/\" method=\"post\">\
<br>Running in multicast mode<br><br>\
<table>\
<tr><td align=\"right\" width=95px>Universe:</td><td><input type=\"number\" name=\"%s\" min=\"1\" max=\"999\" value=\"%u\">  Channel: <input type=\"number\" name=\"%s\" min=\"1\" max=\"512\" value=\"%u\"></td></tr>\
<tr><td align=\"right\">SSID:<br>Pass Phrase:</td>\
<td>\
<input type=\"test\" name=\"%s\" value=\"%s\">\
<br><input type=\"test\" name=\"%s\" value=\"%s\">\
</td></tr>\
<tr><td align=\"right\">DHCP enabled:<br>IP address:<br>Subnet mask:<br>Gateway:</td>\
<td>\
<input type=\"checkbox\" name=\"%s\" onchange=\"toggleDisabled(this.checked)\" %s>\
<br><input type=\"number\" name=\"%s\" min=\"0\" max=\"256\" value=\"%u\" class=\"dhcp\" %s>\
.<input type=\"number\" name=\"%s\" min=\"0\" max=\"256\" value=\"%u\" class=\"dhcp\" %s>\
.<input type=\"number\" name=\"%s\" min=\"0\" max=\"256\" value=\"%u\" class=\"dhcp\" %s>\
.<input type=\"number\" name=\"%s\" min=\"0\" max=\"256\" value=\"%u\" class=\"dhcp\" %s>\
<br><input type=\"number\" name=\"%s\" min=\"0\" max=\"256\" value=\"%u\" class=\"dhcp\" %s>\
.<input type=\"number\" name=\"%s\" min=\"0\" max=\"256\" value=\"%u\" class=\"dhcp\" %s>\
.<input type=\"number\" name=\"%s\" min=\"0\" max=\"256\" value=\"%u\" class=\"dhcp\" %s>\
.<input type=\"number\" name=\"%s\" min=\"0\" max=\"256\" value=\"%u\" class=\"dhcp\" %s>\
<br><input type=\"number\" name=\"%s\" min=\"0\" max=\"256\" value=\"%u\" class=\"dhcp\" %s>\
.<input type=\"number\" name=\"%s\" min=\"0\" max=\"256\" value=\"%u\" class=\"dhcp\" %s>\
.<input type=\"number\" name=\"%s\" min=\"0\" max=\"256\" value=\"%u\" class=\"dhcp\" %s>\
.<input type=\"number\" name=\"%s\" min=\"0\" max=\"256\" value=\"%u\" class=\"dhcp\" %s>\
</td></tr>\
</table>\
<br><br><INPUT type=\"submit\" value=\"Send\"> <button type=\"submit\" formaction=\"/mode\">Change Mode</button>\
</form>\
</div></body>\
</html>",
  
            PARAM_UNIVERSE, s_PtrEeprom->e131_universe, PARAM_CHANNEL, s_PtrEeprom->e131_channel,
            PARAM_SSID, s_PtrEeprom->ssid, PARAM_KEY, s_PtrEeprom->passphrase,
            PARAM_DHCP, checked ? "checked":"",
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
  else
  {
        snprintf(htmlBuffer, sizeof(htmlBuffer) - 1,
  
"<html>\
<head>\
<title>Sonoff settings</title>\
<script>\
function toggleDisabled(_checked) {var cells = document.getElementsByClassName('dhcp');for (var i = 0; i < cells.length; i++) {cells[i].disabled = _checked ? true:false;}}\
</script>\
<style>input[type=number]{width:32px;padding:0px} input::-webkit-outer-spin-button, input::-webkit-inner-spin-button {-webkit-appearance:none;margin:0; } div.max_width {max-width:375px;margin:auto;padding:0px;border:3px solid #73AD21;}</style>\
</head>\
<body><div class=\"max_width\" align=\"center\">\
<h1>Sonoff settings</h1>\
<form action=\"/\" method=\"post\">\
<br>Running in unicast mode<br>(Universe & Channel are fixed)<br><br>\
<table>\
<tr><td align=\"right\" width=100px>Universe:</td><td>1  Channel:  1</td></tr>\
<tr><td align=\"right\">SSID:<br>Pass Phrase:</td>\
<td>\
<input type=\"test\" name=\"%s\" value=\"%s\">\
<br><input type=\"test\" name=\"%s\" value=\"%s\">\
</td></tr>\
<tr><td align=\"right\">IP address:<br>Subnet mask:<br>Gateway:</td>\
<td>\
<input type=\"checkbox\" name=\"%s\" onchange=\"toggleDisabled(this.checked)\" %s>\
<br><input type=\"number\" name=\"%s\" min=\"0\" max=\"256\" value=\"%u\" class=\"dhcp\" %s>\
.<input type=\"number\" name=\"%s\" min=\"0\" max=\"256\" value=\"%u\" class=\"dhcp\" %s>\
.<input type=\"number\" name=\"%s\" min=\"0\" max=\"256\" value=\"%u\" class=\"dhcp\" %s>\
.<input type=\"number\" name=\"%s\" min=\"0\" max=\"256\" value=\"%u\" class=\"dhcp\" %s>\
<br><input type=\"number\" name=\"%s\" min=\"0\" max=\"256\" value=\"%u\" class=\"dhcp\" %s>\
.<input type=\"number\" name=\"%s\" min=\"0\" max=\"256\" value=\"%u\" class=\"dhcp\" %s>\
.<input type=\"number\" name=\"%s\" min=\"0\" max=\"256\" value=\"%u\" class=\"dhcp\" %s>\
.<input type=\"number\" name=\"%s\" min=\"0\" max=\"256\" value=\"%u\" class=\"dhcp\" %s>\
<br><input type=\"number\" name=\"%s\" min=\"0\" max=\"256\" value=\"%u\" class=\"dhcp\" %s>\
.<input type=\"number\" name=\"%s\" min=\"0\" max=\"256\" value=\"%u\" class=\"dhcp\" %s>\
.<input type=\"number\" name=\"%s\" min=\"0\" max=\"256\" value=\"%u\" class=\"dhcp\" %s>\
.<input type=\"number\" name=\"%s\" min=\"0\" max=\"256\" value=\"%u\" class=\"dhcp\" %s>\
</td></tr>\
</table>\
<br><br><INPUT type=\"submit\" value=\"Send\"> <button type=\"submit\" formaction=\"/mode\">Change Mode</button>\
</form>\
</div></body>\
</html>",
  
            PARAM_SSID, s_PtrEeprom->ssid, PARAM_KEY, s_PtrEeprom->passphrase,
            PARAM_DHCP, checked ? "checked":"",
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
}

void updateModePage()
{
  memset(htmlBuffer, 0, sizeof(htmlBuffer));

  snprintf(htmlBuffer, sizeof(htmlBuffer) - 1,

    "<html>\
<head>\
<title>Sonoff mode confirmation</title>\
<style>input[type=number]{width:32px;padding:0px} input::-webkit-outer-spin-button, input::-webkit-inner-spin-button {-webkit-appearance:none;margin:0; } div.max_width {max-width:375px;margin:auto;padding:0px;border:3px solid #73AD21;}</style>\
</head>\
<body><div class=\"max_width\" align=\"center\">\
<h1>Sonoff mode change</h1>\
<form action=\"/\" method=\"post\">\
<p>Current mode: %s</p><p>Please confirm change to <b>%s</b>!<br>\
<b><font color=\"red\">(This will reboot the sonoff controller)</font></b><br></p>\
<br><br><button type=\"submit\" formaction=\"/\">Cancel</button><button type=\"submit\" formaction=\"/confirmed\">Confirm</button>\
</form>\
</div></body>\
</html>",
  
            MULTICAST_ON ? "multicast":"unicast",
            MULTICAST_ON ? "unicast":"multicast");
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
    Serial.printf("arg not found: %s\n", PtrArgName);
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
    Serial.printf("arg not found: %s\n", PtrArgName);
  }
}

void handleRoot()
{  
  bool dhcp = false;

  Serial.println(__FUNCTION__);
  
  if(server.args() > 0)
  {
    s_PtrEeprom->e131_universe = getServerArgNum(PARAM_UNIVERSE, s_PtrEeprom->e131_universe);
    s_PtrEeprom->e131_channel = getServerArgNum(PARAM_CHANNEL, s_PtrEeprom->e131_channel);
    getServerArgString(PARAM_SSID, s_PtrEeprom->ssid, sizeof(s_PtrEeprom->ssid));
    getServerArgString(PARAM_KEY, s_PtrEeprom->passphrase, sizeof(s_PtrEeprom->passphrase));
    if(server.arg(PARAM_DHCP) == "on")
    {
      dhcp = true;
      memset(s_PtrEeprom->ip, 0, sizeof(s_PtrEeprom->ip));
      memset(s_PtrEeprom->subnet, 0, sizeof(s_PtrEeprom->subnet));
      memset(s_PtrEeprom->gateway, 0, sizeof(s_PtrEeprom->gateway));
    }
    else
    {
      s_PtrEeprom->ip[0] = getServerArgNum(PARAM_IP_A, s_PtrEeprom->ip[0]);
      s_PtrEeprom->ip[1] = getServerArgNum(PARAM_IP_B, s_PtrEeprom->ip[1]);
      s_PtrEeprom->ip[2] = getServerArgNum(PARAM_IP_C, s_PtrEeprom->ip[2]);
      s_PtrEeprom->ip[3] = getServerArgNum(PARAM_IP_D, s_PtrEeprom->ip[3]);
      s_PtrEeprom->subnet[0] = getServerArgNum(PARAM_SUBNET_A, s_PtrEeprom->subnet[0]);
      s_PtrEeprom->subnet[1] = getServerArgNum(PARAM_SUBNET_B, s_PtrEeprom->subnet[1]);
      s_PtrEeprom->subnet[2] = getServerArgNum(PARAM_SUBNET_C, s_PtrEeprom->subnet[2]);
      s_PtrEeprom->subnet[3] = getServerArgNum(PARAM_SUBNET_D, s_PtrEeprom->subnet[3]);
      s_PtrEeprom->gateway[0] = getServerArgNum(PARAM_GATEWAY_A, s_PtrEeprom->gateway[0]);
      s_PtrEeprom->gateway[1] = getServerArgNum(PARAM_GATEWAY_B, s_PtrEeprom->gateway[1]);
      s_PtrEeprom->gateway[2] = getServerArgNum(PARAM_GATEWAY_C, s_PtrEeprom->gateway[2]);
      s_PtrEeprom->gateway[3] = getServerArgNum(PARAM_GATEWAY_D, s_PtrEeprom->gateway[3]);
    }

    eeprom_commit();
      
    IPAddress dhcpIP(0,0,0,0);
    IPAddress ip(s_PtrEeprom->ip[0], s_PtrEeprom->ip[1], s_PtrEeprom->ip[2], s_PtrEeprom->ip[3]);
    IPAddress subnet(s_PtrEeprom->subnet[0], s_PtrEeprom->subnet[1], s_PtrEeprom->subnet[2], s_PtrEeprom->subnet[3]);
    if(ip != WiFi.localIP() || subnet != WiFi.subnetMask() || DHCP_ON != dhcp) // compare current and old IP in non-dhcp scenerio
    {
      Serial.print("IP update in progress: ");
      Serial.println(ip);
      server.send(200, "text", "Network restarted, please reconnect with new IP address");
      delay(1000);
      setup_wifi();
    }
  }
  updateMainPage();  
  server.send(200, "text/html", htmlBuffer);
}

void handleMode()
{
  Serial.println ("handleMode");
  updateModePage();  
  server.send(200, "text/html", htmlBuffer);
}

void handleConfirmMode()
{
 String html = F("<html><head><style>\
div.a {font-size:50px}\
</style></head>\
<body>\
<div class=\"a\" id=\"counter\" align=\"center\" >15</div>\
<script>\
setInterval(function() {\
var div = document.querySelector(\"#counter\");\
var count = div.textContent * 1 - 1;\
div.textContent = count;\
if (count <= 0) {\
window.location.replace(\"/\");\
}\
}, 1000);\
</script>\
</body>\
</html>");

  Serial.println ("handleConfirmMode");
  s_PtrEeprom->flags ^= FLAG_MULTICAST_MODE;
  eeprom_commit();
  server.send(200, "text/html", html);
  delay(1000);
  ESP.restart();
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
  server.on("/mode", handleMode);
  server.on("/confirmed", handleConfirmMode);
  server.onNotFound(handleNotFound);
  server.begin();
}

void loop_web()
{
  server.handleClient();
}
