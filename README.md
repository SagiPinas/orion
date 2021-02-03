# orion

Base firmware for the IoT device communicating via LoRa with a Mesh protocol Implementation

The version of the firmware is written in Arduino C++ to support a large array of hardware devices.

### Supported devices:

1.) TTGO OLED LoRa32 Development board v1
2.) Heltec LoRa 32 v2
[![image](https://community.hiveeyes.org/uploads/default/eda042713108809e3511e822a1aa4e582ee70ebc)]()

### Device Overview:

[![image](https://mist.now.sh/mist/on-1.png)]()
The actual picture of an Orion node prototype consists of a power jack that takes in `5v DC` and a Heltec LoRa 32 development board. The node device puts up a wifi network with an HTTP web server that can receive some JSON payload data and transmit it via LoRa.

The mesh protocol allows the device to echo other messages from other nodes effectively.
