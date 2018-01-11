/*
  ESPFirmware - This Library can be used for Automatik update the
                Firmware of a ESP8266/ESP32
  Copyright (c) 2018 R.Thomitzek.  All right reserved.

  Project home: https://github.com/Thro42/ESPFirmware/
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 3 of the License, or (at your option) any later version.
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with library. If not, see <http://www.gnu.org/licenses/>
*/
// H
#include "ESPFirmware.h"

// Includes
#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266httpUpdate.h>

#ifdef DEBUG
#define Debug(x) Serial.print(x)
#define DebugLn(x) Serial.println(x)
#else
#define Debug(x)
#define DebugLn(x)
#endif

ESPFirmware::ESPFirmware(String sketchName){
  _Name = sketchName;
  _FVersion = 1.0;
  _updmode = bin;
}

ESPFirmware::ESPFirmware(String rootURL, String sketchName){
  _fwRootURL = rootURL;
  _Name = sketchName;
  _FVersion = 1.0;
  _updmode = bin;
}

ESPFirmware::ESPFirmware(String sketchName, float fVersion) {
  _Name = sketchName;
  _FVersion = fVersion;
  _updmode = json;
}
ESPFirmware::ESPFirmware(String rootURL, String sketchName, float fVersion){
  _fwRootURL = rootURL;
  _Name = sketchName;
  _FVersion = fVersion;
  _updmode = json;

}

ESPFirmware::ESPFirmware(String sketchName, float fVersion, fw_upd_mode updmode) {
  _Name = sketchName;
  _FVersion = fVersion;
  _updmode = updmode;
  if ((_updmode == bin) || (_updmode == id_bin)) {
    _ID = 0;
  }
}

ESPFirmware::ESPFirmware(String sketchName, float fVersion, fw_upd_mode updmode, int iD){
  _Name = sketchName;
  _FVersion = fVersion;
  _updmode = updmode;
  _ID = iD;
}

void ESPFirmware::Update( fw_upd_mode updmode ) {
  _updmode = updmode;
  Update();
}

void ESPFirmware::Update( fw_upd_mode updmode , int iD ) {
  _updmode = updmode;
  _ID = iD;
  Update();
}

void ESPFirmware::Update() {
  versionInfo _versInfo;
  _versInfo = ReadVersionInfo();
  if (doUpdate(_versInfo)) {
//
  }
}

bool ESPFirmware::isValidVersion(String str) {
  bool isNum = false;
  for (byte i = 0; i < str.length(); i++) {
    isNum = isDigit(str.charAt(i));
    if (!isNum)
      return false;
  }
  return isNum;
}

void ESPFirmware::setRootUrl(String rootURL) { _fwRootURL = rootURL; }

void ESPFirmware::setID(int iD){ _ID = iD; }

void ESPFirmware::setMode( fw_upd_mode updmode ){ _updmode = updmode; }

versionInfo ESPFirmware::ReadVersionInfo() {
  HTTPClient http;
  String url_VersInfo;
  versionInfo _versInfo;

  switch (_updmode) {
  case bin: // direckt Binfile Definition erzeugen
    _versInfo.binfile = _fwRootURL + _Name + ".bin";
    _versInfo.version =
        _FVersion + 0.1; // dafür sorgen das immer ein Updat gemacht wird
    break;
  case json: // Definition aus JSON Datei lesen
    url_VersInfo = _fwRootURL + _Name + ".json";
    break;
  case id_bin: // direckt Binfile Definition mit ID erzeugen
    _versInfo.binfile = _fwRootURL + _Name + "." + String(_ID) + ".bin";
    _versInfo.version = _FVersion + 0.1;
    ; // dafür sorgen das immer ein Updat gemacht wird
    break;
  case id_json: // Definition aus JSON Datei lesen
    url_VersInfo = _fwRootURL + _Name + "." + String(_ID) + ".json";
    break;
  }
  if ((_updmode == json) || (_updmode == id_json)) {
    http.begin(url_VersInfo);            // Webseite aufrufen
    int httpCode = http.GET();           // Antwort des Servers einlesen
    if (httpCode != HTTP_CODE_OK) {      // Wenn Antwort nicht OK
      String payload = http.getString(); // Webseite einlesen
      delay(500);
      StaticJsonBuffer<200> recBuffer;
      //    DynamicJsonBuffer recBuffer ;
      JsonObject &root = recBuffer.parseObject(payload);
      if (root.containsKey("version")) {
        const char *cVers = root["version"];
        String sVersion = String(cVers);
        if (isValidVersion(sVersion))
          _versInfo.version = sVersion.toFloat(); // Zahl aus String bilden
      }
      if (root.containsKey("binfile")) {
        const char *cFilename = root["binfile"];
        _versInfo.binfile = String(cFilename);
      }
    }
  }
  return _versInfo;
}

bool ESPFirmware::doUpdate(versionInfo versInfo) {
  t_httpUpdate_return ret;
  bool ret_val;
  if (versInfo.version > _FVersion) // Firmwareversion mit aktueller vergleichen
  {
    // reboot abschalten, wir wollen erst Meldungen ausgeben
    ESPhttpUpdate.rebootOnUpdate(false);
    // Update lesen
    ret = ESPhttpUpdate.update(versInfo.binfile);
    switch (ret) {
    case HTTP_UPDATE_FAILED:
      ret_val = false;
      break;
    case HTTP_UPDATE_OK:
      ret_val = true;
      delay(1);
      ESP.reset();
      delay(100);
      break;
    }
  } else {
    ret_val = true;
  }
  return ret_val;
}
