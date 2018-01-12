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

#ifndef _ESPFIRMWARE_H
#define _ESPFIRMWARE_H

#include <stdint.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266httpUpdate.h>

enum fw_upd_mode {
  bin,
  json,
  id_bin,
  id_json
};

struct versionInfo {
    float version ;
    String binfile;  //"ESPFirmware.1.00.bin"
};

class ESPFirmware {
public:
  ESPFirmware(String sketchName);
  ESPFirmware(String rootURL, String sketchName);
  ESPFirmware(String sketchName, float fVersion);
  ESPFirmware(String rootURL, String sketchName, float fVersion);
  ESPFirmware(String sketchName, float fVersion, fw_upd_mode updmode);
  ESPFirmware(String sketchName, float fVersion, fw_upd_mode updmode, int iD);
  void Update();
  void Update( fw_upd_mode updmode );
  void Update( fw_upd_mode updmode, int iD );
  void setRootUrl(String rootURL);
  void setID(int iD);
  void setMode( fw_upd_mode updmode );
  void setDebug(bool debug);

private:
  String _Name;
  int _ID;
  float _FVersion;
  fw_upd_mode _updmode;
  String _fwRootURL = "http://192.168.2.1/firmware/";
  bool _debug;
  versionInfo ReadVersionInfo();
  bool isValidVersion(String str);
  bool doUpdate(versionInfo versInfo);
};

#endif
