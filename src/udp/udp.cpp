#include "module.h"
#include <WiFi.h>
#include <WiFiUdp.h>
#include <ESPmDNS.h>


// Wi-Fi connection info.
const char* ssid     = "your ssid";
const char* password = "your pw";
const int port = 3002;

//String string_buf = "message from M5stack";

//ntp server and time info
const char* ntpServer =  "ntp.nict.jp";
const long  gmtOffset_sec = 9 * 3600;
const int   daylightOffset_sec = 0;
struct tm timeinfo;

// The udp library class
WiFiUDP udp;

// mDNS target & M5stack
const String target_dev = "raspberrypi";
IPAddress ip;         // IPAdress of the target device
String ip_addr = "";  // 4 octet style target address
const char *mdns_name = "m5stick";  //M5stack mDNS name

// convert IP address to 4 octet style
String ipToString(uint32_t ip){
  String result = "";

  result += String((ip & 0xFF), 10);    // base number : 10
  result += ".";
  result += String((ip & 0xFF00) >> 8, 10);
  result += ".";
  result += String((ip & 0xFF0000) >> 16, 10);
  result += ".";
  result += String((ip & 0xFF000000) >> 24, 10);

  return result;
}

void mdns(){
  mdns_init();
  ip = MDNS.queryHost(target_dev);
  // target dev IP
  Serial.println("");
  Serial.print(target_dev + " : ");
  Serial.println(ip);
  // M5 self IP
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  ip_addr = ipToString(ip);
  // register mDNS name
  if (MDNS.begin(mdns_name)) {
    Serial.println("MDNS responder started");
  }
}

void print_wifi_state(){
	M5.Lcd.clear(BLACK);  // clear LCD
  M5.Lcd.setTextColor(YELLOW);
  M5.Lcd.setCursor(3, 3);
	M5.Lcd.println("");
	M5.Lcd.println("WiFi connected.");
	M5.Lcd.print("IP address: ");
	M5.Lcd.println(WiFi.localIP());
	M5.Lcd.print("Port: ");
	M5.Lcd.println(port);
}

void setup_wifi(){
	M5.Lcd.setTextColor(RED);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(3, 10);
  M5.Lcd.print("Connecting to ");
  M5.Lcd.println(ssid);

	// setup wifi
  WiFi.mode(WIFI_STA);  // WIFI_AP, WIFI_STA, WIFI_AP_STA or WIFI_OFF
  WiFi.begin(ssid, password);
	// WiFi.begin();

	// Connecting ..
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    M5.Lcd.print(".");
  }

	// print state
  print_wifi_state();
  udp.begin(port);
  
  // mDNS
  mdns();
}

void udp_setup() {
	M5.begin();
  Serial.begin(115200);
	// setup wifi
	setup_wifi();
    //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

void udp_loop(String val){
    // get current rime
    if (!getLocalTime(&timeinfo)) {
      Serial.println("Failed to obtain time");
    return;
    }
    String cur_time = String(timeinfo.tm_year + 1900) 
      + "/" + String(timeinfo.tm_mon) 
      + "/" + String(timeinfo.tm_mday) 
      + " " + String(timeinfo.tm_hour) 
      + ":" + String(timeinfo.tm_min) 
      + " " + String(timeinfo.tm_sec) + " ";
    Serial.println(cur_time);

    //udp send
    const char *ipadr = ip_addr.c_str();
    udp.beginPacket(ipadr, port);
    // current time
    int i = 0;
    while( cur_time.length() > i){
      char tmp = cur_time.charAt(i);
      udp.write(tmp);
      i++;
    }
    // measured value
    i = 0;
    while( val.length() > i){
      char tmp = val.charAt(i);
      udp.write(tmp);
      i++;
    }
    udp.endPacket();
}