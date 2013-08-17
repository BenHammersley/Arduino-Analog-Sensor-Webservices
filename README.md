Arduino-Analog-Sensor-Webservices
=================================

This sketch makes it simple to build internet addressable sensors with an arduino and an ethernet shield.


An arduino with an ethernet shield can act as a simple webserver. Sensors or switches can be connected to the arduino via its analog inputs and by exposing the sensor values over the internet via the webserver, you can use the sensors to drive other applications, or to feed a database, or to trigger something, or any of the other fashionable Internet of Things type idea.

This sketch makes building such things really simple: it will serve the values of each of the 6 analog pins as human-readable HTML, XML, or JSON, and each pin is individually addressable, or you can pull all of them at the same time.

http://x.x.x.x/debug/ gives you a very useful constantly-refreshing status page

http://x.x.x.x/analogRead/1 will give you the human readable value of the analog pin 1 (Change the last number to change the pin. There are pins numbered 0,1,2,3,4,and 5)

http://x.x.x.x/analogRead/all will give you the human readable values of all of the analog pins.

http://x.x.x.x/analogReadXML/1 and http://x.x.x.x/analogReadXML/all will give you the same but in XML

http://x.x.x.x/analogReadJSON/1 and http://x.x.x.x/analogReadJSON/all will give you the same but in JSON



Usage:

Be sure to set the MAC address of your ethernet shield, and the correct ip and gateway values in obvious section at the top of the code
  
