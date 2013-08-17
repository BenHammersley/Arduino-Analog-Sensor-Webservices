/*
 Arduino Sensor Webservice, created August 17, 2013, by Ben Hammersley.

 Based on an original version here:
 http://forum.arduino.cc/index.php/topic,6595.0.html#0
 created Sept 17, 2010 by Hari Wiguna, g33k.blogspot.com
 

 Be sure to set the MAC address of your ethernet shield, and the correct ip and gateway values, below.
  
 Visit http://x.x.x.x/debug/ for live human-readable data and further usage instructions

*/

#include <SPI.h>
#include <Ethernet.h>
#include <string.h>

// MAC address can be anything that is unique within your network.
// IP is the address the Arduino Ethernet Card would respond to.  It needs to be an unused address within your network.
byte mac[] = {0x90, 0xA2, 0xDA, 0x0D, 0xA7, 0xB9 };
IPAddress ip(192,168,0,177);
IPAddress gateway(192,168,0,1);

EthernetServer server(80); // Port 80 is http.

//-- Commands and parameters (sent by browser) --
char cmd[15];    // Nothing magical about 15, increase these if you need longer commands/parameters
char param1[15];
char param2[15];

//-- Sample Ports ---

void setup()
{
  Ethernet.begin(mac, ip);
  server.begin();

  Serial.begin(57600);
}

#define bufferMax 128
int bufferSize;
char buffer[bufferMax];

void loop()
{
  EthernetClient client = server.available();
  if (client)
  {
    WaitForRequest(client);
    ParseReceivedRequest();
    PerformRequestedCommands();
    
    client.stop();
  }
}

void WaitForRequest(EthernetClient client) // Sets buffer[] and bufferSize
{
  bufferSize = 0;
 
  while (client.connected()) {
    if (client.available()) {
      char c = client.read();
      if (c == '\n')
        break;
      else
        if (bufferSize < bufferMax)
          buffer[bufferSize++] = c;
        else
          break;
    }
  }
  
}

void ParseReceivedRequest()
{
  Serial.println("in ParseReceivedRequest");
  Serial.println(buffer);
  
  //Received buffer contains "GET /cmd/param1/param2 HTTP/1.1".  Break it up.
  char* slash1;
  char* slash2;
  char* slash3;
  char* space2;
  
  slash1 = strstr(buffer, "/") + 1; // Look for first slash
  slash2 = strstr(slash1, "/") + 1; // second slash
  slash3 = strstr(slash2, "/") + 1; // third slash
  space2 = strstr(slash2, " ") + 1; // space after second slash (in case there is no third slash)
  if (slash3 > space2) slash3=slash2;

  
  // strncpy does not automatically add terminating zero, but strncat does! So start with blank string and concatenate.
  cmd[0] = 0;
  param1[0] = 0;
  param2[0] = 0;
  strncat(cmd, slash1, slash2-slash1-1);
  strncat(param1, slash2, slash3-slash2-1);
  strncat(param2, slash3, space2-slash3-1);
  
}

void PerformRequestedCommands()
{
  if ( strcmp(cmd,"analogRead") == 0 ) RemoteAnalogRead();
  if ( strcmp(cmd,"analogReadXML") == 0 ) RemoteAnalogReadXML();
  if ( strcmp(cmd,"analogReadJSON") == 0 ) RemoteAnalogReadJSON();
  if ( strcmp(cmd,"debug") == 0 ) DisplayDebugPage();
}


void RemoteAnalogRead()
{

    server.println("HTTP/1.1 200 OK");
    server.println("Content-Type: text/html"); // Ensure the correct mime-type for the output
    server.println("Connection: close");  // the connection will be closed after completion of the response
    server.println();


  if ( strcmp(param1,"all") == 0 )
	{
		server.print("<html><head><title>Sensor Data</title></head><body>");
		for (int analogChannel = 0; analogChannel < 6; analogChannel++) 
			{
            // delay a little to let the sensors settle
            delay(10);
            int sensorReading = analogRead(analogChannel);
            server.print("Analog Input ");
            server.print(analogChannel);
            server.print(" is ");
            server.print(sensorReading);
            server.println("<br/>");       
            }	
			server.print("</body></html>");
	}
	else 
	{
  int analogPin = param1[0] - '0'; // Param1 should be one digit analog port
  int analogValue = analogRead(analogPin);
  
  server.print("<html><head><title>Sensor Data</title></head><body>");
  server.print("Analog Input ");
  server.print(analogPin, DEC);
  server.print(" is ");
  server.print(analogValue,DEC);
  server.print("</body></html>");

}
}

void RemoteAnalogReadXML()
{
	
    server.println("HTTP/1.1 200 OK");
    server.println("Content-Type: application/xml"); // Ensure the correct mime-type for the output
    server.println("Connection: close");  // the connection will be closed after completion of the response	
	server.println();
    
  if ( strcmp(param1,"all") == 0 )
	{
		server.print("<xml>\n");
		for (int analogChannel = 0; analogChannel < 6; analogChannel++) 
			{
            // delay a little to let the sensors settle
            delay(10);
            int sensorReading = analogRead(analogChannel);
		 	server.print("<value pin=\"");
		  	server.print(analogChannel, DEC);
		  	server.print("\">");
		  	server.print(sensorReading,DEC);
		  	server.print("</value>");
			server.print("\n");   
            }	
			server.print("</xml>");	}
	else 
	{
  int analogPin = param1[0] - '0'; // Param1 should be one digit analog port
  int analogValue = analogRead(analogPin);
  
  //-- Send response back to browser --
  server.print("<xml>");
  server.print("<value pin=\"");
  server.print(analogPin, DEC);
  server.print("\">");
  server.print(analogValue,DEC);
  server.print("</value>");
  server.print("</xml>");

   }
}

void RemoteAnalogReadJSON()
{
	
    server.println("HTTP/1.1 200 OK");
    server.println("Content-Type: application/json"); // Ensure the correct mime-type for the output
    server.println("Connection: close");  // the connection will be closed after completion of the response
	server.println();
    
  if ( strcmp(param1,"all") == 0 )
	{
		server.print("{");
		for (int analogChannel = 0; analogChannel < 6; analogChannel++) 
			{
            // delay a little to let the sensors settle
            delay(10);
            int sensorReading = analogRead(analogChannel);
		  server.print("\"");
		  server.print(analogChannel, DEC);
		  server.print("\":\"");
		  server.print(sensorReading,DEC);
		  server.print("\"");
		if (analogChannel < 5) { server.print(",");}
			server.print("\n");   
            }	
			server.print("}");
			}
	else 
	{
  int analogPin = param1[0] - '0'; // Param1 should be one digit analog port
  int analogValue = analogRead(analogPin);
  
  //-- Send response back to browser --
  server.print("{\"");
  server.print(analogPin, DEC);
  server.print("\":\"");
  server.print(analogValue,DEC);
  server.print("\"}");

   }
}



void DisplayDebugPage()
{
          server.println("HTTP/1.1 200 OK");
          server.println("Content-Type: text/html");
          server.println("Connection: close");  // the connection will be closed after completion of the response
	      server.println("Refresh: 1");  // refresh the page automatically every 1 sec
          server.println();
          server.println("<!DOCTYPE HTML>");
          server.println("<html>");
          server.println("<h1>Arduino Data Server Debugging</h1>");
          server.println("<p>The ethernet shield has the IP Address: ");
          server.println(Ethernet.localIP());
          
          server.println("<h2>Usage</h2>");
          server.println("<p><code>http://x.x.x.x/debug/</code> will bring you to this page.</p>");
          server.println("<p><code>http://x.x.x.x/analogRead/1</code> will give you the human readable value of the analog pin 1.</p>");
          server.println("<p><code>http://x.x.x.x/analogRead/all</code> will give you the human readable values of all of the analog pins.</p>");

          server.println("<p><code>http://x.x.x.x/analogReadXML/1</code> and <code>http://x.x.x.x/analogReadXML/all</code> will give you the same but in XML</p>");

          server.println("<p><code>http://x.x.x.x/analogReadJSON/1</code> and <code>http://x.x.x.x/analogReadJSON/all</code> will give you the same but in JSON</p>");


          server.println("<h2>Live Analog Inputs</h2>");

          // output the value of each analog input pin
          for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
            // delay a little to let the sensors settle
            delay(10);
            int sensorReading = analogRead(analogChannel);
            server.print("Analog Input ");
            server.print(analogChannel);
            server.print(" = ");
            server.print(sensorReading);
  
            server.println("<br/>");       
          }
          
          server.println("<h2>Digital Inputs</h2>");

          // output the value of each digital input pin
          for (int digitalChannel = 0; digitalChannel < 8; digitalChannel++) {
            int sensorReading = digitalRead(digitalChannel);
            server.print("Digital Input ");
            server.print(digitalChannel);
            server.print(" is ");
            server.print(sensorReading);
            server.println("<br />");       
          }
          
          server.println("</html>");
}