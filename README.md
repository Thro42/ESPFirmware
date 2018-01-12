# ESP Firmware Update Library
This Library can be used for Automatik update the Firmware of a ESP8266/ESP32

### Example versionInfo
The Version Information is stored in JSON Format in a File on a Webserver.  
#### versionInfo without IS
File Name: **SketchName.json**
#### versionInfo with ID
File Name: **SketchName.ID.json**
#### versionInfo content
```json
{
    "version":"1.00",
    "binfile":"ScetchName.Verson.bin"
}
```

### How to Using the library
```c++
  #define SKETCH_NAME "FwUpdJson"
  #define SKETCH_VERSION 1.50
  #define FW_ROOTURL "http://192.168.1.1/fw/"

  void setup() {
...
    ESPFirmware fwUpd(FW_ROOTURL, SKETCH_NAME, SKETCH_VERSION );
    fwUpd.Update();
...
  }

```
