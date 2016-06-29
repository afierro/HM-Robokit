#include <EthernetClient.h>
#include <Dhcp.h>
#include <EthernetServer.h>
#include <Dns.h>
#include <Ethernet.h>
#include <EthernetUdp.h>


byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 0, 177);

EthernetServer server(1010);

int enablePin = 11;
int in1Pin = 10;
int in2Pin = 9;
int potPin = 0;

void setup() {
  Serial.begin(9600);
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("setup starting...");

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  Serial.println("Eth Started... ");

  Serial.println("Starting server... ");
  server.begin();
  Serial.print("server is at: ");
  Serial.println(Ethernet.localIP());

  // cibfig motor
   pinMode(in1Pin, OUTPUT);
   pinMode(in2Pin, OUTPUT);
   pinMode(enablePin, OUTPUT);
}

String readString;

void loop() {
  EthernetClient client = server.available();
  if (client) {
     Serial.println("new client");
     
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);

        if (readString.length() < 100) {
        //store characters to string 
        readString += c; 
        }

        if (c == '_') {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  
          //delay(100);
          //client.stop();
          //Serial.write("Saliendo..");
          int speed = analogRead(potPin) / 4;



          if(readString.indexOf("back") >0)//checks for on
          {
          //adelante 
          Serial.write("Atras..");
          setMotor(speed, false);
          }
          if(readString.indexOf("go") >0)//checks for off
          {
          //reversa 
          Serial.write("Adelante..");
          setMotor(speed, true);
          }

          readString = "";
          
          break;
          
        }
      }         
    } 


    }   else {
    Serial.println("No request ...");
  }
          delay(1);
          client.stop();
          delay(1000);
}


void setMotor(int speed, boolean reverse) {
 analogWrite(enablePin, speed);
 digitalWrite(in1Pin, !reverse);
 digitalWrite(in2Pin, reverse);  
 delay(1000);
 analogWrite(enablePin, false);
  digitalWrite(in1Pin, true);
 digitalWrite(in2Pin, true);
}
