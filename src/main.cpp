#include <Arduino.h>
#include <defaults.h>
#include <types.h>
#include <FS.h>
#include <Strip/Strip.h>
#include <Network.h>
#include <Mosquitto.h>
#include <Utils.h>
#include <WebServer.h>


Strip* strip;

void messageHandler(String cmd, String payload) {
  Serial.print("|CMD: " + cmd + " |");
  Serial.print("|Payload:" + payload + " |");
  Serial.println("");

  // strip->cmd(cmd, payload);
}

void setup ( void ) {
  Utils::initStorage();
  settings_t settings = Utils::getSettings();
  
  if (isnan(settings.strip_size)) {
    strip = new Strip(settings.strip_size);
  } else {
    strip = new Strip(1);
  }

  Serial.begin(115200);
  digitalWrite(2, HIGH); // turn of device led

  #ifdef USE_FS
    SPIFFS.begin();
  #endif

  Network::init(settings.ssid, settings.pass);
  
  if (Network::getMode() == Network::MODES::ST)
    Mosquitto::init(settings.broker, settings.topic, messageHandler);
  
  WebServer::init(messageHandler);
}

void loop ( void ) {
  yield();
  Mosquitto::loop();
  WebServer::loop();
  strip->loop();
}
