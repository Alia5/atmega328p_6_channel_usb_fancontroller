this is a firmware for my custom 6 channel usb fan-controller based on an atmel atmega328p avr
the "basic" schematics can also be found here
simply build the schematic for every one of the pwm channels, exact pins can be read from source!
schematics for the usb part can be taken from https://www.obdev.at/products/vusb/index.html
wich this project also uses
please edit the usbconfig.h file

also dont connect the red 5V wire from usb, connect only ground and data-pins! Use 5V from s-ata or molex, so the microcontroller shuts off when your computer is off
you are going to use s-ata / molex anyway since you need 12V and ample amperage to power your fans!
