#include "ota.h"
#include "cserial.h"


AsyncWebServer server(80);

bool isServerRunning = false;
const char* ssids[] = {"The Universal Wavefunction","Hobbit House 2.4"};
const char* passwords[] = {"likesbutts","lineofsight" };
void ensureConnection();
void onWifiConnected(const char * ssid);
void WiFiEvent(WiFiEvent_t event);

unsigned long ota_progress_millis = 0;

void onOTAStart() {
  // Log when OTA has started
  CSerial.println("OTA update started!");
  // <Add your own code here>
}

void onOTAProgress(size_t current, size_t final) {
  // Log every 1 second
  if (millis() - ota_progress_millis > 1000) {
    ota_progress_millis = millis();
    Serial.printf("OTA Progress Current: %u bytes, Final: %u bytes\n", current, final);
  }
}

void onOTAEnd(bool success) {
  // Log when OTA has finished
  if (success) {
    CSerial.println("OTA update finished successfully!");
  } else {
    CSerial.println("There was an error during OTA update!");
  }
  // <Add your own code here>
}


void setupOta() {
    #ifndef USE_WIFI
    CSerial.println("WIFI Disabled");
    return;
    #else
    WiFi.mode(WIFI_STA);
    CSerial.println("Starting WIFI");
    WiFi.onEvent(WiFiEvent);
    WiFi.getHostname();
    ensureConnection();
    #endif


}
void onWifiConnected(const char * ssid) {
    CSerial.println("");
    CSerial.print("Connected to ");
    CSerial.println(ssid);
    CSerial.print("IP address: ");
    CSerial.println(WiFi.localIP());
    if(isServerRunning) {
      CSerial.print("Server was running, unexpectedly, ending");
      server.end();
      isServerRunning = false;
    }
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", "Hi! This is ElegantOTA AsyncDemo.");
    });

    ElegantOTA.begin(&server);    // Start ElegantOTA
    // ElegantOTA callbacks
    ElegantOTA.onStart(onOTAStart);
    ElegantOTA.onProgress(onOTAProgress);
    ElegantOTA.onEnd(onOTAEnd);
    CSerial.connected(&server);
    server.begin();
    CSerial.println("HTTP server started");
    isServerRunning = true;
}

void loopOta() {
    #ifdef USE_WIFI
    ensureConnection();
    if(isServerRunning)
      ElegantOTA.loop();
    #endif

}

int ssidIx=0;
int length = ARRAY_SIZE(ssids);
int numDelays = WIFI_TIMOUT/WIFI_DELAY;
long connectionStart = -1;
bool isConnecting = false;
bool isDisconnected = false;

 
void ensureConnection() {
  if(isServerRunning && isDisconnected) {
    CSerial.println("Disconnected, Ending server");
    server.end();
    isServerRunning = false;
    isDisconnected = false;
  }
  if(WiFi.status() == WL_CONNECTED) {
      isConnecting = false;
      if(!isServerRunning) {
        onWifiConnected(ssids[ssidIx]);
      }
  }
  else {
    if(isConnecting && millis() - connectionStart > WIFI_TIMOUT) {
      ssidIx = (ssidIx +1)%length; 
      isConnecting = false;
    }
    if(!isConnecting) {
      CSerial.print("Connecting to ");
      CSerial.println(ssids[ssidIx]);
      WiFi.begin(ssids[ssidIx], passwords[ssidIx]);
      connectionStart = millis();  
      isConnecting = true;
    }
  }
}



void WiFiEvent(WiFiEvent_t event) {
  Serial.printf("[WiFi-event] event: %d\n", event);

  switch (event) {
    case WIFI_EVENT_STAMODE_CONNECTED:       
      CSerial.println("Connected to access point"); break;
    case WIFI_EVENT_STAMODE_DISCONNECTED:    

      isDisconnected = true;
      CSerial.println("Disconnected from WiFi access point"); break;
    case WIFI_EVENT_STAMODE_GOT_IP:
      CSerial.print("Obtained IP address: ");
      CSerial.println(WiFi.localIP());
      break;
    default:                                    break;
  }
}
