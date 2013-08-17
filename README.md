Arduino-Analog-Sensor-Webservices
=================================

An Arduino sketch to make it easy to build and test sensor-based webservices with analog sensors and an ethernet shield.

This sketch makes it simple to build internet addressable sensors with an arduino and an ethernet shield.

Your arduino will serve the values of its 6 analog pins as human-readable HTML, XML, or JSON. 
Each pin is individually addressable, or you can pull all of them at the same time:

http://x.x.x.x/debug/ gives you a very useful constantly-refreshing status page

http://x.x.x.x/analogRead/1 will give you the human readable value of the analog pin 1

http://x.x.x.x/analogRead/all will give you the human readable values of all of the analog pins.

http://x.x.x.x/analogReadXML/1 and http://x.x.x.x/analogReadXML/all will give you the same but in XML

http://x.x.x.x/analogReadJSON/1 and http://x.x.x.x/analogReadJSON/all will give you the same but in JSON



Usage:

Be sure to set the MAC address of your ethernet shield, and the correct ip and gateway values, below.
  
