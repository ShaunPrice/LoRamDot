# Shaun Price LoRaWan mDot Library

This library is a wrapper for the [Multitech](http://www.multitech.com) [mDot LoRaWan module](http://www.multitech.com/brands/multiconnect-mdot) with [version 2.0.x firmaware AT command set](http://www.multitech.com/documents/publications/manuals/s000643.pdf). The wrapper supports [The Things Network](https://www.thethingsnetwork.org) by providing two functions that match [The Things Network](https://www.thethingsnetwork.org)) Application EUI and App Key settings required to set up the network. 

## Dependencies

This library requires the latest version of the [Arduino IDE](https://www.arduino.cc/en/Main/Software) (tested with v1.8.0).

The code was tested on a [Arduino/Genuino 101](https://www.arduino.cc/en/Main/ArduinoBoard101) board with a [TinySine XBee Shield Version 2 shield for Arduino](http://www.tinyosshop.com/index.php?route=product/product&path=60_73&product_id=681) for the mDot LoRaWan module. We have no association with this company and the code does not relly on this shield. Other shields connecting via serial are also suitable.

The code uses [The Things Network](https://www.thethingsnetwork.org) for the gateway and backend application to connect to.

## Usage

The included example sketch will walk you through all of the features of the library to connect to The Things Network.

Before trying to connect to [The Things Network](https://www.thethingsnetwork.org) you will need to register and create an application and register your mDot nodes with the network. [See here for details](https://www.thethingsnetwork.org/docs/devices/)

## License

Copyright (c) 2017 [Shaun Price](http://www.priceconsulting.biz). Licensed under the [GNU LESSER GENERAL PUBLIC LICENSE](/COPYING.txt?raw=true).