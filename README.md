# Purpose

This sketch runs the E1.31 protocol, allowing you to control an AC device via the sonoff basic.

## Control AC via E131 (xlights or vixen)

The controller is expecting a value between 0 and 255 however 0 is off, anything else is considered on!

## Setup

If you have a Sonoff basic V3 it supports OTA, you can install this firmware that way.  If you have a older Sonoff basic you'll have to put in on via a serial/USB programmer.

Please search the web for the many tutorials and videos on how to update the sonoff basic with your own FW.

### Initial config/connection of new FW once installed

Note:  Please only configure ONE device at a time!

Initial configuration of the device is fairly simple once the FW has been installed it will bootup and without being able to connect to a network it will create a wifi network called "SONOFF".  This only takes a few seconds...

Once the network is created you can connect to it with your laptop, it should be an open wifi network, you'll have a chance to change this!

After connecting to the SONOFF wifi the default IP address of the sonoff basic will be 192.168.4.1.  Open a web browser and connect to this IP address for a setting webpage, change the settings per your network and your done!

## Updates

The firmware itself supports OTA once installed, so you will be able to upgrade this device if needed.

## Info

One interesting thing about this implementation is that in multicast mode, multiple controllers can share a single universe.  They just need to listen to different channels!