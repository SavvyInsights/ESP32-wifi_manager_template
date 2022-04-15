#ifndef FUNCTIONS_H // This is so if it included in mulitple cpp files, the code will not be duplicated
#define FUNCTIONS_H

#include <Arduino.h> // must be here, but does not need to be entered in .cpp file
#include <AsyncElegantOTA.h>
// WEB SERIAL

#define ESP_ASYNC_WIFIMANAGER_VERSION_MIN_TARGET      "ESPAsync_WiFiManager v1.12.2"
#define ESP_ASYNC_WIFIMANAGER_VERSION_MIN             1012002
#define _ESPASYNC_WIFIMGR_LOGLEVEL_    3// Use from 0 to 4. Higher number, more debugging messages and memory usage.

#include <WiFi.h>
#include <WiFiClient.h>
#include <ESPmDNS.h>
#include <WiFiMulti.h>

WiFiMulti wifiMulti;

String password = "password";

//#define FORMAT_FILESYSTEM true// You only need to format the filesystem once
#define FORMAT_FILESYSTEM false

  #define USE_LITTLEFS    false// LittleFS has higher priority than SPIFFS
  #define USE_SPIFFS      true

#if USE_LITTLEFS
  // Use LittleFS
  #ifndef LCD_DISPLAY
    #include "FS.h"
  #endif
  
  #include <LittleFS.h>  

  FS* filesystem =      &LittleFS;
  #define FileFS        LittleFS
  #define FS_Name       "LittleFS"
    
#elif USE_SPIFFS
  #ifndef LCD_DISPLAY // Already delcared in lcd_functions.h
    #include <SPIFFS.h>
  #endif
  FS* filesystem =                &SPIFFS;
  #define FileFS                  SPIFFS
  #define FS_Name                 "SPIFFS"
#else
  // Use FFat
  #include <FFat.h>
  FS* filesystem =                &FFat;
  #define FileFS                  FFat
  #define FS_Name                 "FFat"
#endif

#define LED_BUILTIN       2
#define LED_ON            HIGH
#define LED_OFF           LOW

#include <SPIFFSEditor.h>

// These defines must be put before #include <ESP_DoubleResetDetector.h>
// to select where to store DoubleResetDetector's variable.
// For ESP32, You must select one to be true (EEPROM or SPIFFS)
// Otherwise, library will use default EEPROM storage
#if USE_LITTLEFS
  #define ESP_DRD_USE_LITTLEFS    true
  #define ESP_DRD_USE_SPIFFS      false
  #define ESP_DRD_USE_EEPROM      false
#elif USE_SPIFFS
  #define ESP_DRD_USE_LITTLEFS    false
  #define ESP_DRD_USE_SPIFFS      true
  #define ESP_DRD_USE_EEPROM      false
#else
  #define ESP_DRD_USE_LITTLEFS    false
  #define ESP_DRD_USE_SPIFFS      false
  #define ESP_DRD_USE_EEPROM      true
#endif

#define DOUBLERESETDETECTOR_DEBUG       true  //false

#include <ESP_DoubleResetDetector.h>      //https://github.com/khoih-prog/ESP_DoubleResetDetector

#define DRD_TIMEOUT 20// Number of seconds after reset during which a
#define DRD_ADDRESS 0// RTC Memory Address for the DoubleResetDetector to use

DoubleResetDetector* drd = NULL;//DoubleResetDetector drd(DRD_TIMEOUT, DRD_ADDRESS);

//////

// SSID and PW for Config Portal
String ssid = "TTGO-1_" + String((uint32_t)ESP.getEfuseMac(), HEX);


// SSID and PW for your Router
String Router_SSID;
String Router_Pass;

// From v1.1.0
#define MIN_AP_PASSWORD_SIZE    8

#define SSID_MAX_LEN            32
//From v1.0.10, WPA2 passwords can be up to 63 characters long.
#define PASS_MAX_LEN            64

typedef struct
{
  char wifi_ssid[SSID_MAX_LEN];
  char wifi_pw  [PASS_MAX_LEN];
}  WiFi_Credentials;

typedef struct
{
  String wifi_ssid;
  String wifi_pw;
}  WiFi_Credentials_String;

#define NUM_WIFI_CREDENTIALS      2

// Assuming max 49 chars
#define TZNAME_MAX_LEN            50
#define TIMEZONE_MAX_LEN          50

typedef struct
{
  WiFi_Credentials  WiFi_Creds [NUM_WIFI_CREDENTIALS];
  char TZ_Name[TZNAME_MAX_LEN];     // "America/Toronto"
  char TZ[TIMEZONE_MAX_LEN];        // "EST5EDT,M3.2.0,M11.1.0"
  uint16_t checksum;
} WM_Config;

WM_Config         WM_config;

#define  CONFIG_FILENAME              F("/wifi_cred.dat")
//////

// Indicates whether ESP has WiFi credentials saved from previous session, or double reset detected
bool initialConfig = false;

// Use false if you don't like to display Available Pages in Information Page of Config Portal
// Comment out or use true to display Available Pages in Information Page of Config Portal
// Must be placed before #include <ESP_WiFiManager.h>
#define USE_AVAILABLE_PAGES     true

// From v1.0.10 to permit disable/enable StaticIP configuration in Config Portal from sketch. Valid only if DHCP is used.
// You'll loose the feature of dynamically changing from DHCP to static IP, or vice versa
// You have to explicitly specify false to disable the feature.
#define USE_STATIC_IP_CONFIG_IN_CP          false

// Use false to disable NTP config. Advisable when using Cellphone, Tablet to access Config Portal.
// See Issue 23: On Android phone ConfigPortal is unresponsive (https://github.com/khoih-prog/ESP_WiFiManager/issues/23)
#define USE_ESP_WIFIMANAGER_NTP     false

// Just use enough to save memory. On ESP8266, can cause blank ConfigPortal screen
// if using too much memory
#define USING_AFRICA        false
#define USING_AMERICA       true
#define USING_ANTARCTICA    false
#define USING_ASIA          false
#define USING_ATLANTIC      false
#define USING_AUSTRALIA     false
#define USING_EUROPE        false
#define USING_INDIAN        false
#define USING_PACIFIC       false
#define USING_ETC_GMT       false

// Use true to enable CloudFlare NTP service. System can hang if you don't have Internet access while accessing CloudFlare
// See Issue #21: CloudFlare link in the default portal (https://github.com/khoih-prog/ESP_WiFiManager/issues/21)
#define USE_CLOUDFLARE_NTP          false

#define USING_CORS_FEATURE          true

////////////////////////////////////////////

// Use USE_DHCP_IP == true for dynamic DHCP IP, false to use static IP which you have to change accordingly to your network
#if (defined(USE_STATIC_IP_CONFIG_IN_CP) && !USE_STATIC_IP_CONFIG_IN_CP)
  // Force DHCP to be true
  #if defined(USE_DHCP_IP)
    #undef USE_DHCP_IP
  #endif
  #define USE_DHCP_IP     true
#else
  // You can select DHCP or Static IP here
  #define USE_DHCP_IP     true
  //#define USE_DHCP_IP     false
#endif

#if ( USE_DHCP_IP )
  // Use DHCP
  
  #if (_ESPASYNC_WIFIMGR_LOGLEVEL_ > 3)
    #warning Using DHCP IP
  #endif
  
  IPAddress stationIP   = IPAddress(10, 10, 10, 79);
  IPAddress gatewayIP   = IPAddress(10, 10, 10, 2);
  IPAddress netMask     = IPAddress(255, 255, 255, 0);
  
#else
  // Use static IP
  
  #if (_ESPASYNC_WIFIMGR_LOGLEVEL_ > 3)
    #warning Using static IP
  #endif
  
  #ifdef ESP32
    IPAddress stationIP   = IPAddress(192, 168, 2, 232);
  #else
    IPAddress stationIP   = IPAddress(192, 168, 2, 186);
  #endif
  
  IPAddress gatewayIP   = IPAddress(192, 168, 2, 1);
  IPAddress netMask     = IPAddress(255, 255, 255, 0);
#endif

////////////////////////////////////////////


#define USE_CONFIGURABLE_DNS      true

IPAddress dns1IP      = gatewayIP;
IPAddress dns2IP      = IPAddress(8, 8, 8, 8);

#define USE_CUSTOM_AP_IP          false

IPAddress APStaticIP  = IPAddress(10, 10, 10, 77);
IPAddress APStaticGW  = IPAddress(10, 10, 10, 2);
IPAddress APStaticSN  = IPAddress(255, 255, 255, 0);

#include <ESPAsync_WiFiManager.h>               //https://github.com/khoih-prog/ESPAsync_WiFiManager

// Redundant, for v1.10.0 only
//#include <ESPAsync_WiFiManager-Impl.h>          //https://github.com/khoih-prog/ESPAsync_WiFiManager

String host = "async-esp32fs";

#define HTTP_PORT     80

AsyncWebServer server(HTTP_PORT);
DNSServer dnsServer;
WiFiServer TelnetServer(23); // setup Telenet port
WiFiClient Telnet;

AsyncEventSource events("/events");

String http_username = "admin";
String http_password = "admin";

String separatorLine = "===============================================================";

///////////////////////////////////////////
// New in v1.4.0
/******************************************
 * // Defined in ESPAsync_WiFiManager.h
typedef struct
{
  IPAddress _ap_static_ip;
  IPAddress _ap_static_gw;
  IPAddress _ap_static_sn;

}  WiFi_AP_IPConfig;

typedef struct
{
  IPAddress _sta_static_ip;
  IPAddress _sta_static_gw;
  IPAddress _sta_static_sn;
#if USE_CONFIGURABLE_DNS  
  IPAddress _sta_static_dns1;
  IPAddress _sta_static_dns2;
#endif
}  WiFi_STA_IPConfig;
******************************************/

WiFi_AP_IPConfig  WM_AP_IPconfig;
WiFi_STA_IPConfig WM_STA_IPconfig;







void initSTAIPConfigStruct(WiFi_STA_IPConfig &in_WM_STA_IPconfig)
{
  in_WM_STA_IPconfig._sta_static_ip   = stationIP;
  in_WM_STA_IPconfig._sta_static_gw   = gatewayIP;
  in_WM_STA_IPconfig._sta_static_sn   = netMask;
#if USE_CONFIGURABLE_DNS  
  in_WM_STA_IPconfig._sta_static_dns1 = dns1IP;
  in_WM_STA_IPconfig._sta_static_dns2 = dns2IP;
#endif
}

void initAPIPConfigStruct(WiFi_AP_IPConfig &in_WM_AP_IPconfig)
{
  in_WM_AP_IPconfig._ap_static_ip   = APStaticIP;
  in_WM_AP_IPconfig._ap_static_gw   = APStaticGW;
  in_WM_AP_IPconfig._ap_static_sn   = APStaticSN;
}

void displayIPConfigStruct(WiFi_STA_IPConfig in_WM_STA_IPconfig)
{
  LOGERROR3(F("stationIP ="), in_WM_STA_IPconfig._sta_static_ip, F(", gatewayIP ="), in_WM_STA_IPconfig._sta_static_gw);
  LOGERROR1(F("netMask ="), in_WM_STA_IPconfig._sta_static_sn);
#if USE_CONFIGURABLE_DNS
  LOGERROR3(F("dns1IP ="), in_WM_STA_IPconfig._sta_static_dns1, F(", dns2IP ="), in_WM_STA_IPconfig._sta_static_dns2);
#endif
}

void configWiFi(WiFi_STA_IPConfig in_WM_STA_IPconfig)
{
  #if USE_CONFIGURABLE_DNS  
    // Set static IP, Gateway, Subnetmask, DNS1 and DNS2. New in v1.0.5
    WiFi.config(in_WM_STA_IPconfig._sta_static_ip, in_WM_STA_IPconfig._sta_static_gw, in_WM_STA_IPconfig._sta_static_sn, in_WM_STA_IPconfig._sta_static_dns1, in_WM_STA_IPconfig._sta_static_dns2);  
  #else
    // Set static IP, Gateway, Subnetmask, Use auto DNS1 and DNS2.
    WiFi.config(in_WM_STA_IPconfig._sta_static_ip, in_WM_STA_IPconfig._sta_static_gw, in_WM_STA_IPconfig._sta_static_sn);
  #endif 
}




uint8_t connectMultiWiFi()
{
  // For ESP32 core v1.0.6, must be >= 500
  #define WIFI_MULTI_1ST_CONNECT_WAITING_MS           800L  // For ESP32, this better be 0 to shorten the connect time.
  #define WIFI_MULTI_CONNECT_WAITING_MS                   500L

  uint8_t status;

  //WiFi.mode(WIFI_STA);

  LOGERROR(F("ConnectMultiWiFi with :"));

  if ( (Router_SSID != "") && (Router_Pass != "") )
  {
    LOGERROR3(F("* Flash-stored Router_SSID = "), Router_SSID, F(", Router_Pass = "), Router_Pass );
    LOGERROR3(F("* Add SSID = "), Router_SSID, F(", PW = "), Router_Pass );
    wifiMulti.addAP(Router_SSID.c_str(), Router_Pass.c_str());
  }

  for (uint8_t i = 0; i < NUM_WIFI_CREDENTIALS; i++)
  {
    // Don't permit NULL SSID and password len < MIN_AP_PASSWORD_SIZE (8)
    if ( (String(WM_config.WiFi_Creds[i].wifi_ssid) != "") && (strlen(WM_config.WiFi_Creds[i].wifi_pw) >= MIN_AP_PASSWORD_SIZE) )
    {
      LOGERROR3(F("* Additional SSID = "), WM_config.WiFi_Creds[i].wifi_ssid, F(", PW = "), WM_config.WiFi_Creds[i].wifi_pw );
    }
  }

  LOGERROR(F("Connecting MultiWifi..."));

  //WiFi.mode(WIFI_STA);

#if !USE_DHCP_IP
  // New in v1.4.0
  configWiFi(WM_STA_IPconfig);
  //////
#endif

  int i = 0;
  status = wifiMulti.run();
  delay(WIFI_MULTI_1ST_CONNECT_WAITING_MS);

  while ( ( i++ < 20 ) && ( status != WL_CONNECTED ) )
  {
    status = WiFi.status();

    if ( status == WL_CONNECTED )
      break;
    else
      delay(WIFI_MULTI_CONNECT_WAITING_MS);
  }

  if ( status == WL_CONNECTED )
  {
    LOGERROR1(F("WiFi connected after time: "), i);
    LOGERROR3(F("SSID:"), WiFi.SSID(), F(",RSSI="), WiFi.RSSI());
    LOGERROR3(F("Channel:"), WiFi.channel(), F(",IP address:"), WiFi.localIP() );


    #ifdef LCD_DISPLAY
      lcdClearText();
      lcdoutln("STA MODE");
      tft.setTextSize(1); lcdoutln();
      tft.setTextSize(2); lcdout("   "); lcdoutln(WiFi.localIP());
      tft.setTextSize(1);
      lcdout(" /edit"); lcdout(" ("); lcdout(http_username); lcdout("/"); lcdout(http_password);lcdout(")");
      lcdoutln("  /update  ");  
      lcdout("  ");lcdout(Router_SSID); lcdout(" / "); lcdoutln(Router_Pass);

    #endif







    
  }
  else
  {
    LOGERROR(F("WiFi not connected"));

    // To avoid unnecessary DRD
    drd->loop();
  
    ESP.restart();
  }

  return status;
}





//format bytes
String formatBytes(size_t bytes)
{
  if (bytes < 1024)
  {
    return String(bytes) + "B";
  }
  else if (bytes < (1024 * 1024))
  {
    return String(bytes / 1024.0) + "KB";
  }
  else if (bytes < (1024 * 1024 * 1024))
  {
    return String(bytes / 1024.0 / 1024.0) + "MB";
  }
  else
  {
    return String(bytes / 1024.0 / 1024.0 / 1024.0) + "GB";
  }
}





void toggleLED()
{
  //toggle state
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}


#if USE_ESP_WIFIMANAGER_NTP
void printLocalTime(){
  struct tm timeinfo;
  getLocalTime( &timeinfo );
  // Valid only if year > 2000. 
  // You can get from timeinfo : tm_year, tm_mon, tm_mday, tm_hour, tm_min, tm_sec
  if (timeinfo.tm_year > 100 )
  {
    out("Local Date/Time: ");
    out( asctime( &timeinfo ) );
  }
}
#endif



void heartBeatPrint()
{
#if USE_ESP_WIFIMANAGER_NTP
  printLocalTime();
#else
  static int num = 1;

  if (WiFi.status() == WL_CONNECTED)
    outln(F("H"));        // H means connected to WiFi
  else
    out(F("F"));        // F means not connected to WiFi

  if (num == 80)
  {
    outln();
    num = 1;
  }
  else if (num++ % 10 == 0)
  {
    out(F(" "));
  }
#endif  
}


void check_WiFi()
{
  if ( (WiFi.status() != WL_CONNECTED) )
  {
    outln(F("\nWiFi lost. Call connectMultiWiFi in loop"));
    connectMultiWiFi();
  }
}  


void check_status()
{
  static ulong checkstatus_timeout  = 0;
  static ulong LEDstatus_timeout    = 0;
  static ulong checkwifi_timeout    = 0;

  static ulong current_millis;

#define WIFICHECK_INTERVAL    1000L

#if USE_ESP_WIFIMANAGER_NTP
  #define HEARTBEAT_INTERVAL    60000L
#else
  #define HEARTBEAT_INTERVAL    10000L
#endif

#define LED_INTERVAL          2000L

  current_millis = millis();
  
  // Check WiFi every WIFICHECK_INTERVAL (1) seconds.
  if ((current_millis > checkwifi_timeout) || (checkwifi_timeout == 0))
  {
    check_WiFi();
    checkwifi_timeout = current_millis + WIFICHECK_INTERVAL;
  }

  if ((current_millis > LEDstatus_timeout) || (LEDstatus_timeout == 0))
  {
    // Toggle LED at LED_INTERVAL = 2s
    toggleLED();
    LEDstatus_timeout = current_millis + LED_INTERVAL;
  }

  // Print hearbeat every HEARTBEAT_INTERVAL (10) seconds.
  if ((current_millis > checkstatus_timeout) || (checkstatus_timeout == 0))
  {
    heartBeatPrint();
    checkstatus_timeout = current_millis + HEARTBEAT_INTERVAL;
  }
}


int calcChecksum(uint8_t* address, uint16_t sizeToCalc)
{
  uint16_t checkSum = 0;
  
  for (uint16_t index = 0; index < sizeToCalc; index++)
  {
    checkSum += * ( ( (byte*) address ) + index);
  }

  return checkSum;
}


bool loadConfigData()
{
  File file = FileFS.open(CONFIG_FILENAME, "r");
  LOGERROR(F("LoadWiFiCfgFile "));

  memset((void *) &WM_config,       0, sizeof(WM_config));

  // New in v1.4.0
  memset((void *) &WM_STA_IPconfig, 0, sizeof(WM_STA_IPconfig));
  //////

  if (file)
  {
    file.readBytes((char *) &WM_config,   sizeof(WM_config));

    // New in v1.4.0
    file.readBytes((char *) &WM_STA_IPconfig, sizeof(WM_STA_IPconfig));
    //////

    file.close();
    LOGERROR(F("OK"));

    if ( WM_config.checksum != calcChecksum( (uint8_t*) &WM_config, sizeof(WM_config) - sizeof(WM_config.checksum) ) )
    {
      LOGERROR(F("WM_config checksum wrong"));
      
      return false;
    }
    
    // New in v1.4.0
    displayIPConfigStruct(WM_STA_IPconfig);
    //////

    return true;
  }
  else
  {
    LOGERROR(F("failed"));

    return false;
  }
}

void saveConfigData()
{
  File file = FileFS.open(CONFIG_FILENAME, "w");
  LOGERROR(F("SaveWiFiCfgFile "));

  if (file)
  {
    WM_config.checksum = calcChecksum( (uint8_t*) &WM_config, sizeof(WM_config) - sizeof(WM_config.checksum) );
    
    file.write((uint8_t*) &WM_config, sizeof(WM_config));

    displayIPConfigStruct(WM_STA_IPconfig);

    // New in v1.4.0
    file.write((uint8_t*) &WM_STA_IPconfig, sizeof(WM_STA_IPconfig));
    //////

    file.close();
    LOGERROR(F("OK"));
  }
  else
  {
    LOGERROR(F("failed"));
  }
}




void wifiManagerSetup(){
pinMode(LED_BUILTIN, OUTPUT);  //set led pin as output
  out(F("\nStarting Async_ESP32_FSWebServer_DRD using ")); out(FS_Name);
  out(F(" on ")); outln(ARDUINO_BOARD);
  outln(ESP_ASYNC_WIFIMANAGER_VERSION);
  outln(ESP_DOUBLE_RESET_DETECTOR_VERSION);

#if defined(ESP_ASYNC_WIFIMANAGER_VERSION_INT)
  if (ESP_ASYNC_WIFIMANAGER_VERSION_INT < ESP_ASYNC_WIFIMANAGER_VERSION_MIN)
  {
    out("Warning. Must use this example on Version later than : ");
    outln(ESP_ASYNC_WIFIMANAGER_VERSION_MIN_TARGET);
  }
#endif

  Serial.setDebugOutput(false);

  if (FORMAT_FILESYSTEM) 
    FileFS.format();

  // Format FileFS if not yet
  if (!FileFS.begin(true))
  {
    outln(F("SPIFFS/LittleFS failed! Already tried formatting."));
  
    if (!FileFS.begin())
    {     
      // prevents debug info from the library to hide err message.
      delay(100);
      
#if USE_LITTLEFS
      outln(F("LittleFS failed!. Please use SPIFFS or EEPROM. Stay forever"));
#else
      outln(F("SPIFFS failed!. Please use LittleFS or EEPROM. Stay forever"));
#endif

      while (true)
      {
        delay(1);
      }
    }
  }
  
  File root = FileFS.open("/");
  File file = root.openNextFile();
  
  while (file) 
  {
    String fileName = file.name();
    size_t fileSize = file.size();
    Serial.printf("FS File: %s, size: %s\n", fileName.c_str(), formatBytes(fileSize).c_str());
    file = root.openNextFile();
  }
  
  outln();

  drd = new DoubleResetDetector(DRD_TIMEOUT, DRD_ADDRESS);

  if (!drd)
    outln(F("Can't instantiate. Disable DRD feature"));
    
  unsigned long startedAt = millis();

  // New in v1.4.0
  initAPIPConfigStruct(WM_AP_IPconfig);
  initSTAIPConfigStruct(WM_STA_IPconfig);
  //////

  digitalWrite(LED_BUILTIN, LED_ON);

  //Local intialization. Once its business is done, there is no need to keep it around
  // Use this to default DHCP hostname to ESP8266-XXXXXX or ESP32-XXXXXX
  //ESPAsync_WiFiManager ESPAsync_wifiManager(&webServer, &dnsServer);
  // Use this to personalize DHCP hostname (RFC952 conformed)0
  DNSServer dnsServer;
  
  ESPAsync_WiFiManager ESPAsync_wifiManager(&server, &dnsServer, "AsyncESP32-FSWebServer");

#if USE_CUSTOM_AP_IP 
  //set custom ip for portal
  // New in v1.4.0
  ESPAsync_wifiManager.setAPStaticIPConfig(WM_AP_IPconfig);
  //////
#endif

  ESPAsync_wifiManager.setMinimumSignalQuality(-1);

  // Set config portal channel, default = 1. Use 0 => random channel from 1-13
  ESPAsync_wifiManager.setConfigPortalChannel(0);
  //////

#if !USE_DHCP_IP    
    // Set (static IP, Gateway, Subnetmask, DNS1 and DNS2) or (IP, Gateway, Subnetmask). New in v1.0.5
    // New in v1.4.0
    ESPAsync_wifiManager.setSTAStaticIPConfig(WM_STA_IPconfig);
    //////
#endif

  // New from v1.1.1
#if USING_CORS_FEATURE
  ESPAsync_wifiManager.setCORSHeader("Your Access-Control-Allow-Origin");
#endif

  // We can't use WiFi.SSID() in ESP32as it's only valid after connected.
  // SSID and Password stored in ESP32 wifi_ap_record_t and wifi_config_t are also cleared in reboot
  // Have to create a new function to store in EEPROM/SPIFFS for this purpose
  Router_SSID = ESPAsync_wifiManager.WiFi_SSID();
  Router_Pass = ESPAsync_wifiManager.WiFi_Pass();

  //Remove this line if you do not want to see WiFi password printed
  outln("ESP Self-Stored: SSID = " + Router_SSID + ", Pass = " + Router_Pass);

  // SSID to uppercase
  ssid.toUpperCase();
  //password = "My" + ssid; // moved to top of wifi_manager.h

  bool configDataLoaded = false;

  // From v1.1.0, Don't permit NULL password
  if ( (Router_SSID != "") && (Router_Pass != "") )
  {
    LOGERROR3(F("* Add SSID = "), Router_SSID, F(", PW = "), Router_Pass);
    wifiMulti.addAP(Router_SSID.c_str(), Router_Pass.c_str());
    
    ESPAsync_wifiManager.setConfigPortalTimeout(120); //If no access point name has been previously entered disable timeout.
    outln(F("Got ESP Self-Stored Credentials. Timeout 120s for Config Portal"));
  }
  
  if (loadConfigData())
  {
    configDataLoaded = true;
    
    ESPAsync_wifiManager.setConfigPortalTimeout(120); //If no access point name has been previously entered disable timeout.
    outln(F("Got stored Credentials. Timeout 120s for Config Portal"));

#if USE_ESP_WIFIMANAGER_NTP      
    if ( strlen(WM_config.TZ_Name) > 0 )
    {
      LOGERROR3(F("Saving current TZ_Name ="), WM_config.TZ_Name, F(", TZ = "), WM_config.TZ);

  #if ESP8266
      configTime(WM_config.TZ, "pool.ntp.org"); 
  #else
      //configTzTime(WM_config.TZ, "pool.ntp.org" );
      configTzTime(WM_config.TZ, "time.nist.gov", "0.pool.ntp.org", "1.pool.ntp.org");
  #endif  
    }
    else
    {
      outln(F("Current Timezone is not set. Enter Config Portal to set."));
    } 
#endif 
  }
  else
  {
    // Enter CP only if no stored SSID on flash and file 
    outln(F("Open Config Portal without Timeout: No stored Credentials."));
    initialConfig = true;
    
  }

  if (drd->detectDoubleReset())
  {
    // DRD, disable timeout.
    ESPAsync_wifiManager.setConfigPortalTimeout(0);
    
    outln(F("Open Config Portal without Timeout: Double Reset Detected"));
    initialConfig = true;
  }

  if (initialConfig)
  {
    out(F("Starting configuration portal @ "));



    #ifdef LCD_DISPLAY
      lcdClearText();
    
      lcdoutln(" AP MODE");
      tft.setTextSize(1); lcdoutln();
      tft.setTextSize(2); lcdoutln("   192.168.4.1");
      tft.setTextSize(1); 
      lcdout("  "); lcdout(ssid); lcdout(" / "); lcdoutln(password);
      //digitalWrite(LCD_LIGHT_PIN, 0);
      
      

    #endif





#if USE_CUSTOM_AP_IP    
    out(APStaticIP);
#else
    out(F("192.168.4.1"));
#endif

    out(F(", SSID = "));
    out(ssid);
    
    out(F(", PWD = "));
    outln(password);
    

    // Starts an access point
    if (!ESPAsync_wifiManager.startConfigPortal((const char *) ssid.c_str(), password.c_str()))
      outln(F("Not connected to WiFi but continuing anyway."));
    else
    {
      outln(F("WiFi connected...yeey :)"));
    }

    // Stored  for later usage, from v1.1.0, but clear first
    memset(&WM_config, 0, sizeof(WM_config));
    
    for (uint8_t i = 0; i < NUM_WIFI_CREDENTIALS; i++)
    {
      String tempSSID = ESPAsync_wifiManager.getSSID(i);
      String tempPW   = ESPAsync_wifiManager.getPW(i);
  
      if (strlen(tempSSID.c_str()) < sizeof(WM_config.WiFi_Creds[i].wifi_ssid) - 1)
        strcpy(WM_config.WiFi_Creds[i].wifi_ssid, tempSSID.c_str());
      else
        strncpy(WM_config.WiFi_Creds[i].wifi_ssid, tempSSID.c_str(), sizeof(WM_config.WiFi_Creds[i].wifi_ssid) - 1);

      if (strlen(tempPW.c_str()) < sizeof(WM_config.WiFi_Creds[i].wifi_pw) - 1)
        strcpy(WM_config.WiFi_Creds[i].wifi_pw, tempPW.c_str());
      else
        strncpy(WM_config.WiFi_Creds[i].wifi_pw, tempPW.c_str(), sizeof(WM_config.WiFi_Creds[i].wifi_pw) - 1);  

      // Don't permit NULL SSID and password len < MIN_AP_PASSWORD_SIZE (8)
      if ( (String(WM_config.WiFi_Creds[i].wifi_ssid) != "") && (strlen(WM_config.WiFi_Creds[i].wifi_pw) >= MIN_AP_PASSWORD_SIZE) )
      {
        LOGERROR3(F("* Add SSID = "), WM_config.WiFi_Creds[i].wifi_ssid, F(", PW = "), WM_config.WiFi_Creds[i].wifi_pw );
        wifiMulti.addAP(WM_config.WiFi_Creds[i].wifi_ssid, WM_config.WiFi_Creds[i].wifi_pw);
      }
    }

#if USE_ESP_WIFIMANAGER_NTP      
    String tempTZ   = ESPAsync_wifiManager.getTimezoneName();

    if (strlen(tempTZ.c_str()) < sizeof(WM_config.TZ_Name) - 1)
      strcpy(WM_config.TZ_Name, tempTZ.c_str());
    else
      strncpy(WM_config.TZ_Name, tempTZ.c_str(), sizeof(WM_config.TZ_Name) - 1);

    const char * TZ_Result = ESPAsync_wifiManager.getTZ(WM_config.TZ_Name);
    
    if (strlen(TZ_Result) < sizeof(WM_config.TZ) - 1)
      strcpy(WM_config.TZ, TZ_Result);
    else
      strncpy(WM_config.TZ, TZ_Result, sizeof(WM_config.TZ_Name) - 1);
         
    if ( strlen(WM_config.TZ_Name) > 0 )
    {
      LOGERROR3(F("Saving current TZ_Name ="), WM_config.TZ_Name, F(", TZ = "), WM_config.TZ);

#if ESP8266
      configTime(WM_config.TZ, "pool.ntp.org"); 
#else
      //configTzTime(WM_config.TZ, "pool.ntp.org" );
      configTzTime(WM_config.TZ, "time.nist.gov", "0.pool.ntp.org", "1.pool.ntp.org");
#endif
    }
    else
    {
      LOGERROR(F("Current Timezone Name is not set. Enter Config Portal to set."));
    }
#endif 

    // New in v1.4.0
    ESPAsync_wifiManager.getSTAStaticIPConfig(WM_STA_IPconfig);
    //////
    
    saveConfigData();
  }

  startedAt = millis();

  if (!initialConfig)
  {
    // Load stored data, the addAP ready for MultiWiFi reconnection
    if (!configDataLoaded)
      loadConfigData();

    for (uint8_t i = 0; i < NUM_WIFI_CREDENTIALS; i++)
    {
      // Don't permit NULL SSID and password len < MIN_AP_PASSWORD_SIZE (8)
      if ( (String(WM_config.WiFi_Creds[i].wifi_ssid) != "") && (strlen(WM_config.WiFi_Creds[i].wifi_pw) >= MIN_AP_PASSWORD_SIZE) )
      {
        LOGERROR3(F("* Add SSID = "), WM_config.WiFi_Creds[i].wifi_ssid, F(", PW = "), WM_config.WiFi_Creds[i].wifi_pw );
        wifiMulti.addAP(WM_config.WiFi_Creds[i].wifi_ssid, WM_config.WiFi_Creds[i].wifi_pw);
      }
    }

    if ( WiFi.status() != WL_CONNECTED ) 
    {
      outln(F("ConnectMultiWiFi in setup"));
     
    #ifdef LCD_DISPLAY
      lcdClearText();
      lcdoutln("STA MODE");
      tft.setTextSize(1); lcdoutln();
      tft.setTextSize(2); lcdoutln("   CONNECTING..."); 
      tft.setTextSize(1);
      lcdout("  ");lcdout(Router_SSID); lcdout(" / "); lcdoutln(Router_Pass);

    #endif





      connectMultiWiFi();
    }
  }

  out(F("After waiting "));
  out((float) (millis() - startedAt) / 1000);
  out(F(" secs more in setup(), connection result is "));

  if (WiFi.status() == WL_CONNECTED)
  {
    out(F("connected. Local IP: "));
    outln(WiFi.localIP());
  }
  else
    outln(ESPAsync_wifiManager.getStatus(WiFi.status()));

  if ( !MDNS.begin(host.c_str()) )
  {
    outln(F("Error starting MDNS responder!"));
  }
  
  // Add service to MDNS-SD
  MDNS.addService("http", "tcp", HTTP_PORT);
  
  //SERVER INIT
  events.onConnect([](AsyncEventSourceClient * client)
  {
    client->send("hello!", NULL, millis(), 1000);
  });

  server.addHandler(&events);

  server.on("/heap", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send(200, "text/plain", String(ESP.getFreeHeap()));
  });

  server.addHandler(new SPIFFSEditor(FileFS, http_username, http_password));
  server.serveStatic("/", FileFS, "/").setDefaultFile("index.htm");
  

  server.onNotFound([](AsyncWebServerRequest * request)
  {
    tout(F("NOT_FOUND: "));
    
    if (request->method() == HTTP_GET)
      toutln(F("GET"));
    else if (request->method() == HTTP_POST)
      toutln(F("POST"));
    else if (request->method() == HTTP_DELETE)
      toutln(F("DELETE"));
    else if (request->method() == HTTP_PUT)
      toutln(F("PUT"));
    else if (request->method() == HTTP_PATCH)
      toutln(F("PATCH"));
    else if (request->method() == HTTP_HEAD)
      toutln(F("HEAD"));
    else if (request->method() == HTTP_OPTIONS)
      toutln(F("OPTIONS"));
    else
      toutln(F("UNKNOWN"));
      
    toutln(" http://" + request->host() + request->url());

    if (request->contentLength())
    {
      toutln("_CONTENT_TYPE: " + request->contentType());
      toutln("_CONTENT_LENGTH: " + request->contentLength());
    }

    int headers = request->headers();
    int i;

    for (i = 0; i < headers; i++)
    {
      AsyncWebHeader* h = request->getHeader(i);
      toutln("_HEADER[" + h->name() + "]: " + h->value());
    }

    int params = request->params();

    for (i = 0; i < params; i++)
    {
      AsyncWebParameter* p = request->getParam(i);

      if (p->isFile())
      {
        toutln("_FILE[" + p->name() + "]: " + p->value() + ", size: " + p->size());
      }
      else if (p->isPost())
      {
        toutln("_POST[" + p->name() + "]: " + p->value());
      }
      else
      {
        toutln("_GET[" + p->name() + "]: " + p->value());
      }
    }

    request->send(404);
  });

  server.onFileUpload([](AsyncWebServerRequest * request, const String & filename, size_t index, uint8_t *data, size_t len, bool final)
  {
    (void) request;
    
    if (!index)
      outln("UploadStart: " + filename);

    out((const char*)data);

    if (final)
      outln("UploadEnd: " + filename + "(" + String(index + len) + ")" );
  });

  server.onRequestBody([](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total)
  {
    (void) request;
    
    if (!index)
      outln("BodyStart: " + total);

    out((const char*)data);

    if (index + len == total)
      outln("BodyEnd: " + total);
  });

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/index.html", "text/html");
  });





  AsyncElegantOTA.begin(&server);
  server.begin();
  TelnetServer.begin();
  TelnetServer.setNoDelay(true);
  

  //////

  outln(F("HTTP server started @ "));
  outln(WiFi.localIP());

  outln(separatorLine);
  outln("Open http://"); outln(WiFi.localIP());
  toutln("/edit to see the file browser"); 
  toutln("Using username = " + http_username + " and password = " + http_password);
  toutln(separatorLine);

  






  digitalWrite(LED_BUILTIN, LOW);



}

void telnetLoop(){// Handle Telnet - put on main loop

}
void wifiManagerLoop(){
// Call the double reset detector loop method every so often,
  // so that it can recognise when the timeout expires.
  // You can also call drd.stop() when you wish to no longer
  // consider the next reset as a double reset.
  if (drd)
    drd->loop();

  check_status();
  AsyncElegantOTA.loop();

  // Telnet LOOP
    if (TelnetServer.hasClient()){ // client is connected
  out("client is connected!");
  toutln("Welcome!");
  
    if (!TelnetServer || !Telnet.connected()) {
      if (Telnet) Telnet.stop(); // client disconnected
      Telnet = TelnetServer.available(); //ready for new client
      out("ready for new client");
      toutln("Welcome!");

    } else{
      TelnetServer.available().stop(); // have client, block next
      out("client connected");
      toutln("Welcome!");
    }

  }

}





#endif

