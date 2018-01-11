#define SKETCH_NAME "FwUpdJson"
#define SKETCH_VERSION 1.50
#define FW_ROOTURL "http://192.168.1.1/fw/"

#include <ESPFirmware.h>

#include <ESP8266WiFi.h>

const char* ssid = "mySsid";
const char* password = "MySsidPw";
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
    ESPFirmware fwUpd(FW_ROOTURL, SKETCH_NAME, SKETCH_VERSION );
    fwUpd.Update();
  }
}

/*****************************************************************/
  void loop() {
  }
