# Wifi AC controller

Allows AC units to be controlled over Wi-Fi. 
This code is deprecated, pleae check the following repository for the newest version:
https://github.com/maximemoreillon/ac_controller_json

## Video
[![Alt text](https://img.youtube.com/vi/c-dv-OPsKKY/0.jpg)](https://www.youtube.com/watch?v=c-dv-OPsKKY)

## Parts list
* Wemos D1 Mini
* HC-SR501 PIR sensor
* DHT22 temperature and humidity sensor
* Infrared LED
* NPN transistor

## Schematic
![AC controller schematic](http://maximemoreillon.com/images/projects/iot/ac/ac_schematic.png)

Note: R2 seems wrong. I'll look into it.

## Requirements
* Arduino IDE with [support for ESP8266](https://github.com/esp8266/Arduino)
* [Async MQTT client](https://github.com/marvinroger/async-mqtt-client)
* [ESPAsyncTCP](https://github.com/me-no-dev/ESPAsyncTCP)

## References
AC remote IR signals decoding by [Perhof](https://perhof.wordpress.com/2015/03/29/reverse-engineering-hitachi-air-conditioner-infrared-remote-commands/)
