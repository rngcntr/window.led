#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <AutoConnect.h>
#include <ESP8266mDNS.h>
#include "src/classes/Prometheus/Prometheus.h"
#include "src/classes/WindowLedModel/WindowLedModel.h"

ESP8266WebServer  server(80);
AutoConnect       portal(server);
AutoConnectConfig config;
Prometheus        prometheus;

const char* softap_apid = "window.led";
const char* softap_psk  = "****************";

const char* auth_username = "window.led";
const char* auth_password = "****************";

const char* mdns = "window.led";
const char* hostName = "window-led";

int warmPins[] = {D1, D5};
int coldPins[] = {D2, D6};
WindowLedState ledState(2700, 2, warmPins, 7100, 2, coldPins);

void setup(void) {
  ledState.begin(1024);
  setupRestServerRouting();
  setupPrometheus();
  setConfiguration();
  startServer();
}

void loop(void) {
  portal.handleClient();
}

void setConfiguration(void) {
  config.apid = softap_apid;
  config.psk = softap_psk;
  config.ota = AC_OTA_BUILTIN;
  config.auth = AC_AUTH_DIGEST;
  config.authScope = AC_AUTHSCOPE_PARTIAL;
  config.username = auth_username;
  config.password = auth_password;
  config.autoReconnect = true;
  config.hostName = hostName;
  config.preserveAPMode = false;
  config.reconnectInterval = 6;
  portal.config(config);
}

void startServer(void) {
  if (portal.begin()) {
    if (MDNS.begin(mdns)) {
      MDNS.addService("http", "tcp", 80);
    }
  }
}
