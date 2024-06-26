#include "cserial.h"
#ifdef USE_SERIAL
#ifdef SOFTWARE_SERIAL
SoftwareSerial mySerial(3,4);  //rx, tx
#else
#define mySerial Serial
#endif
#endif
void ComboSerialClass::begin(unsigned long baud) {
    #ifdef USE_SERIAL
    mySerial.begin(baud);
    #endif
}
#ifdef WIFI_SERIAL
void ComboSerialClass::connected(AsyncWebServer *server) { 
    WebSerial.begin(server);
    WebSerial.onMessage([=](uint8_t *data, size_t len) {
        this->println("Received Data...");
        String d = "";
        for(int i=0; i < len; i++){
            d += char(data[i]);
        }
        this->println(d);
    }
    );
}
#else
void ComboSerialClass::connected(void *server) { 
}
#endif

// Print
size_t ComboSerialClass::write(uint8_t m) {
    size_t r;
    #ifdef USE_SERIAL
    r = mySerial.write(m);
    #endif
#ifdef WIFI_SERIAL
    r = WebSerial.write(m);
    #endif
    return r;
}

size_t ComboSerialClass::write(const uint8_t* buffer, size_t size) {
    size_t r;
    #ifdef USE_SERIAL
    r = mySerial.write(buffer,size);
    #endif
    #ifdef WIFI_SERIAL
    r = WebSerial.write(buffer,size);
    #endif
    return r;
}

ComboSerialClass CSerial;
