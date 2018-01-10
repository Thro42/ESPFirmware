#define SKETCH_NAME "FwUpdJson"
#define SKETCH_VERSION 1.50

#include <ESPFirmware.h>

#include <ESP8266WiFi.h>

const char* ssid = "Thomitzek";
const char* password = "thomitzek6410";
String sname = SKETCH_NAME;

void setup() {
  Serial.begin(115200);
  WiFi.hostname(deviceName);      // DHCP Hostname (useful for finding device for static lease)
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("\nConnecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  if (  WiFi.status() == WL_CONNECTED  ) {
    Serial.print ( "IP address: " );
    Serial.println ( WiFi.localIP() );
    ESPFirmware fwUpd(sname, SKETCH_VERSION )
    fwUpd.Update();
  }
}

/*****************************************************************/
  void loop() {
  }
