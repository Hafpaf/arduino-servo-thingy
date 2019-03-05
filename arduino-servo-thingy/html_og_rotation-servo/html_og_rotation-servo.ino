#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Servo.h>


ESP8266WebServer Webserver(80);

Servo myservo;    //laver servo til obj.
Servo rotservo; //Til drejnings servoerne.

int pos = 0;
int rot = 0;

//GPI0 som servo er sat til.
//static const int servoPin = 13;

//erstater med netwerks id og pass.
const char* ssid = "TENTOB";
const char* password = "H72gj+oPa";

//Set web serverens port nr til 80
WiFiServer server(80);

//variablen til at gemme HTTP 
String header;
String Rheader;

//få HTTP value
//position
String valueString = String(5);
int pos1 = 0;
int pos2 = 0;

//rotation
String RvalueString = String(5)
int rot1 = 0;
int rot2 = 0;

void setup() {
  pinMode(D5, OUTPUT);
  // put your setup code here, to run once:
  Serial.begin(115200);

  myservo.attach(D5);   //Sætter Servo objekt til servopin

  rotservo.attach(D6);    //rotations servo objeckt  til servopin

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
        Rheader+= c;              
        if (c == '\n') {             //hvis byte er ny linje character
          if(currentLine.length() == 0){  //HTTP
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            //Hvis HTML hjemmesiden
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
          //CSS Til style ved på og fra knap

          client.println("<style>body{ text-align: center; font-family:\"Trebuchet MS\", Arial; margin-left:auto; margin-right:auto;}");
          client.println(".slider{ width: 300px;}</style>");
          client.println("<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js\"></script>");
          //Web siden
          //Position
          client.println("</head><body><h1>ESP2866 with Servo</h1>");
          client.println("<p>Position: <span id=\"servoPos\"></span></p>");          
          client.println("<input type=\"range\" min=\"0\" max=\"180\" class=\"slider\" id=\"servoSlider\" onchange=\"servo(this.value)\" value=\""+valueString+"\"/>");
          //rotation
          client.println("<h>ESP32 With Servo Rotation</h1>");
          client.println("<p>Rotation: <span id=\"servoRot\"></span></p>");
          //check koden neden under fordi tror valueString bliver brugt 2 steder.
          //*****************************
          //*****************************
          //*****************************
          //Fixed til RvalueString, så position og rotation ikke køre på samme valueString = String(5).
          client.println("<input type=\"range\" min=\"0\" max=\"90\" class=\"sliderR\" id=\"servoRotSlider\" onchange=\"servo(this.value)\" value=\""+RvalueString+"\"/>")

          //Position
          client.println("<script>var slider = document.getElementById(\"servoSlider\");");
          client.println("var servoP = document.getElementById(\"servoPos\"); servoP.innerHTML = slider.value;");
          client.println("slider.oninput = function() { slider.value = this.value; servoP.innerHTML = this.value; }");
          
          //Rotation
          client.println("var Rotslider = document.getElementById(\"servoRotSlider\");");
          client.println("var servoR = document.getElementById(\"servoRot\"); servoR.innerHTML = sliderR.value;");
          client.println("sliderR.oninput = function(){ sliderR.value = this.value; servoR.innerHTML = this.value; }");
          
          //HTTP get request for ESP IP addressen i den specefikke URL.
          //Position
          client.println("$.ajaxSetup({timeout:1000}); function servo(pos) { ");
          client.println("$.get(\"/?value=\" + pos + \"&\"); {Connection: close};}");//Posetion value.
          //Rotation
          client.println("function servo(rot) { ");
          client.println("$.get(\"/?value=\" + rot + \"&\"); {Connection: close};} </script>"); //rotations value
                    //Camera HTML
       //   client.println("<video id="video" width="640" height="480" autoplay></video> "); //Vi laver vinduet til camereaeet på hjemmesiden
       //   client.println("<button id="snap">Snap Photo</button>");
       //   client.println("<canvas id="canvas" width="640" height="480"></canvas> ");
            
                    //Note til Camera delen, hvis vi laver den så skal den sættes op sammen med resten, og ikke havde flere <script> zones.
                    //Camera view Javascript
          //client.println("<script>var video = document.getElementById(\"video\");");
          //client.println("if(navigator.mediaDevices.getUserMedia){navigator.mediaDevices.getUserMedia({video = true}).then(function(stream){video.srcObject = stream; video.play();});}</script>"); // vi for acess til video librariet.
          
          client.println("</body></html>");

          //Få value og HTTP/1.1
          //Position
          if(header.indexOf("GET /?value=")>=0){
            pos1 = header.indexOf('=');
            pos2 = header.indexOf('&');
            valueString = header.substring(pos1+1, pos2);

            //Rotere servo
            myservo.write(valueString.toInt());
            Serial.println(valueString);
          }
          //Rotation / dregnings mekanisme
          if(Rheader.indexOf("GET /?value=")>=0){
            rot1 = Rheader.indexOf('=');
            rot2 = Rheader.indexOf('&');
            RvalueString = Rheader.substring(rot1+1, rot2);

            //sæt til servo objekt
            rotservo.write(RvalueString.toInt());
            Serial.println(RvalueString);
          }
          if(
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
    Rheader = "";
    //afbryd forbindelse
    client.stop();
    Serial.println("Client afbrød");
    Serial.println("");
  }
}
