#include <ESPmDNS.h>
#include <ESP32Servo.h>
#include <dummy.h>
//#include <ESP8266WiFi.h>
#include <WiFi.h>
//#include <ESP8266WebServer.h>
//#include <ESP8266mDNS.h>
//#include <Servo.h>


//WiFiServer server(80);

Servo myservo;    //laver servo til obj.

int pos = 0;

//GPI0 som servo er sat til.
//static const int servoPin = 13;

//erstater med netwerks id og pass.
const char* ssid = "ESP8266_AP";
const char* password = "testnet1";

//Set web serverens port nr til 80
WiFiServer server(80);

//variablen til at gemme HTTP 
String header;

//få HTTP value
String valueString = String(5);
int pos1 = 0;
int pos2 = 0;

void setup() {
  pinMode(13, OUTPUT);
  // put your setup code here, to run once:
  Serial.begin(115200);

  myservo.attach(13);   //Sætter Servo objekt til servopin


  //Connect to Wi-Fi network med SSID og kodeordet.
  Serial.print("forbinder til ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  //printer den lokale ip adrese
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}


void loop() {
  // put your main code here, to run repeatedly:
  WiFiClient client = server.available(); //bruges til signal fra client

  if(client){
    Serial.println("Ny Client.");

    String currentLine = "";

    while(client.connected()) {   //While loop for clients forbindelse
      if(client.available()) {    //if bytes for at læse client
        char c = client.read();   //læs byte
 //       Serial.write(c);          //print seriel monitor
        header += c;              
        if (c == '\n') {             //hvis byte er ny linje character
          if(currentLine.length() == 0){  //HTTP
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            //Hvis HTML hjemmesiden
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
          //CSS Til style ved på og fra knap

          client.println("<style>body{ text-align: center; font-family:\"Trebuchet MS\", Arial; margin-left:auto; margin-right:auto;}");
          client.println(".slider{ width: 300px;}</style>");
          client.println("<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js\"></script>");
          //Web siden
          client.println("</head><body><h1>ESP32 Servo controller</h1>");
          client.println("<p>Position: <span id=\"servoPos\"></span></p>");          
          client.println("<input type=\"range\" min=\"0\" max=\"180\" class=\"slider\" id=\"servoSlider\" onchange=\"servo(this.value)\" value=\""+valueString+"\"/>");
          
          client.println("<script>var slider = document.getElementById(\"servoSlider\");");
          client.println("var servoP = document.getElementById(\"servoPos\"); servoP.innerHTML = slider.value;");
          client.println("slider.oninput = function() { slider.value = this.value; servoP.innerHTML = this.value; }");
          client.println("$.ajaxSetup({timeout:1000}); function servo(pos) { ");
          client.println("$.get(\"/?value=\" + pos + \"&\"); {Connection: close};}</script>");

          client.println("</body></html>");

          //Få value og HTTP/1.1
          if(header.indexOf("GET /?value=")>=0){
            pos1 = header.indexOf('=');
            pos2 = header.indexOf('&');
            valueString = header.substring(pos1+1, pos2);

            //Rotere servo
            myservo.write(valueString.toInt());
            Serial.println(valueString);
          }
          //HTTP Response
          client.println();
          // Break out of the while loop
          break;
          } else {//hvis ny linje, så fjern currentline
            currentLine = "";
          }
        } else if (c != '\r'){ //hvis du for alt andet end carriage så return character,
            currentLine += c; //tilføj til currentline
          }
        }
      }
    //Clear Header variablen
    header = "";
    //afbryd forbindelse
    client.stop();
    Serial.println("Client afbrød");
    Serial.println("");
  }
}
