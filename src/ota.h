#ifndef _OTA_H
#define _OTA_H
#ifdef USE_WIFI
#if defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#elif defined(ESP32)
  #include <WiFi.h>
  #include <AsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>
#include <ElegantOTA.h>


#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))
#define WIFI_TIMOUT 8000
#define WIFI_DELAY 200
extern const char* ssids[];
extern const char* passwords[];
ESP8266WIFISTA_H_
#endif
void setupOta();
void loopOta();
#endif
