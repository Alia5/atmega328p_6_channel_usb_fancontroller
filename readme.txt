This FanController project  was created by my desire to to have a custom FanController-Hardware, which has 6 channels,
is usb-controlled, and gets its temperatues from the hardware-sensors instead of some crappy temperature fingers you attach to a heatsink
this is a firmware for my custom 6 channel usb fan-controller based on an atmel atmega328p avr
You can find the host-application  source and compiled binaries here: https://github.com/Alia5/FanControllerV2

The hardware controlls 3pin and 4pin (PWM) fans trough digital voltage regulation
Without the host application all fans spin up to 100% after about 2-3seconds (as as safety feature)

This Project is provided without ANY warranties.
The creator of this project is not responsible for any damage you might inflicted to your hardware by this.

The schematics can be found here: https://github.com/Alia5/atmega328p_6_channel_usb_fancontroller/blob/master/FanController_Schematics.png
It is advised to put heatsinks on the mosfets (IRF9510) or choose bigger ones that can provide the needed power without the need for a heatsink

!!!
You NEED to set the (6x) 10kOhm Potentiometers so that you have just under 12V on the FanX_PWR pins!
!!!


Partlist:
1x Molex  OR  S-ATA connector
1x USB plug OR Cable + Pinheader for pluging in into mainboard
1x Atmega328p
1x 1kOhm Resitor
1x 1.5kOhm Resitor
1x 0.1uF Capacitor or any capacity higher
1x 16 Mhz Crystal (NOT ceramic oszillator!)
2x Ceramic apacitor 18pF each
2x 68 Ohm Resitor
2x 3.6V Zener-Diodes (500mW or less)(1N4729)
3x LM358N Operational amplifiers (1IC with 2 OpAmps inside)
6x 2.2µF electrolytic capacitor
6x 100µF electrolytic capacitor
6x 10kOhm Resitor
6x 10kOhm Potentiometer
6x P-Channel Mosfet (IRF9510)
6x 3Pin-FanConnector

And in case you don't already have the following things:
A Prototyping board (about the size of an 3.5" HDD)
Maybe a bit of wire
Soldering Iron / solder
An In Circuit Programmer to burn the firmware onto the Atmega328p-Microcontroller





This project uses obdevs v-usb https://www.obdev.at/products/vusb/index.html
Licensed under GPLv3 or GPLv2


This software is licensed under the GPLv3, you can found it under "license.txt"

Last but not least:
One giant shout out to matrixstorm and his tinyUSBboard-project ( http://matrixstorm.com/avr/tinyusbboard/ ) who helped my kickstart working with microcontrollers!