# E46 custom Air Conditioning control

In case you've decided to put a non-standard engine in your bmw e46, 
you will face an issue that A/C pump is controlled by a stock engine ECU.

This Arduino sketch listens to car's CAN BUS, and in case there is an AirCon request from a climete control IHKR/IHKA (mine is IHKR, but IHKA should also work), 
it will trigger the relay, so you can wire your AC pump and Fan controls to an arduino, and control it via button on a climate dashboard.

The code will also turn the AC compressor off, in case throttle is opened >80%, to speed up your car.

Under .stl file you can find a 3d-model case which I have 3d-printed for the setup.

*Please note, that there is NO CAN BUS is connected to e46's OBD interface, so you should search and connect to CAN bus wires coming to instrument cluster gauges.*

## Hardware:
* Arduino UNO R3 https://aliexpress.com/item/32556087234.html
* Speeduino Can Bus shield https://wiki.seeedstudio.com/CAN-BUS_Shield_V2.0/ / https://aliexpress.com/item/1005001459656600.html
* Arduino Dual 5v relay https://aliexpress.com/item/32946245306.html?spm=a2g2w.orderdetail.0.0.35934aa6ml81fk&sku_id=66306480848

## Arduino Libraries
* Speed_Arduino_CAN: https://github.com/Seeed-Studio/Seeed_Arduino_CAN

## Links
* MS42 CAN bus messages: https://www.ms4x.net/index.php?title=Siemens_MS42_CAN_Bus
