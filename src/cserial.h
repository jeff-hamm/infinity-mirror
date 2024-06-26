#ifndef _CSERIAL_H
#define _CSERIAL_H
#include <Arduino.h>
#ifdef USE_WIFI
#include <ESPAsyncWebServer.h>
#include <WebSerialLite.h>
#endif
#ifdef SOFTWARE_SERIAL
#include <SoftwareSerial.h>
#endif

class ComboSerialClass : public Print {
 public:  // Print
  void begin(unsigned long baud);
#ifdef WIFI_SERIAL
  void connected(AsyncWebServer *server);
#else
  void connected(void *server);
#endif
  size_t write(uint8_t);
  size_t write(const uint8_t* buffer, size_t size);
};
extern ComboSerialClass CSerial;

#endif