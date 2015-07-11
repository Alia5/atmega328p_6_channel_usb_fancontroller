this is a firmware for my custom 6 channel usb fan-controller based on an atmel atmega328p avr

This project uses obdevs v-usb https://www.obdev.at/products/vusb/index.html
Licensed under GPLv3 or GPLv2

The "basic" schematics are found in "rough_schmetaics.png"
Simply work after the schematic for every one of the pwm channels, exact pinout can be read from source!

Schematics for the usb part can be taken from https://www.obdev.at/products/vusb/index.html
any one of the possible variants should work

please edit the usbconfig.h file to represent the usb-pins you've chosen!

Also I'd suggest to don't connect the red 5V wire from usb, connect only ground and data-pins! Use 5V from s-ata or molex, so the microcontroller shuts off when your computer is off
you are going to use s-ata / molex anyway since you need 12V and ample amperage to power your fans!


You can find the host-application source and compiled binaries here: ...


This software is licensed under the GPLv3, you can found it under "license.txt"
