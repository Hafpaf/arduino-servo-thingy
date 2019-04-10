#include <ESPmDNS.h>
#include <ESP32Servo.h>
//#include <dummy.h>
//#include <ESP8266WiFi.h>
#include <WiFi.h>
//#include <ESP8266WebServer.h>
//#include <ESP8266mDNS.h>
//#include <Servo.h>
#include <FS.h>
#include <SPIFFS.h>

#define FORMAT_SPIFFS_IF_FAILED true

void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    Serial.printf("Listing directory: %s\r\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        Serial.println("- failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        Serial.println(" - not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if(levels){
                listDir(fs, file.name(), levels -1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("\tSIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}

void readFile(fs::FS &fs, const char * path){
    Serial.printf("Reading file: %s\r\n", path);

    File file = fs.open(path);
    if(!file || file.isDirectory()){
        Serial.println("- failed to open file for reading");
        return;
    }

    Serial.println("- read from file:");
    while(file.available()){
        Serial.write(file.read());
    }
}




//WiFiServer server(80);

Servo myservo;    //laver servo til obj.
Servo myservo2; 

int pos = 0;

//GPI0 som servo er sat til.
//static const int servoPin = 13;

//tilslutter til netvaerk med ssid og password
const char* ssid = "ESP8266_AP";
const char* password = "testnet1";


//Set webserverens port til 80
WiFiServer server(80);

//variablen til at gemme HTTP 
String header;

//få HTTP value
String valueString = String(5);
String valueString2 = String(5);
int pos1 = 0;
int pos2 = 0;

void setup() {
  pinMode(13, OUTPUT);
  pinMode(14, OUTPUT);
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  if(!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)){
    Serial.println("SPIFFS Mount Failed");
    return;
  }

  myservo.attach(13);   //Sætter Servo objekt til servopin
  myservo2.attach(14);


  //Tilslut to Wi-Fi network med SSID og password.
  Serial.print("forbinder til ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  //printer den lokale ip adresse
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}


void loop() {
  listDir(SPIFFS, "/", 0); //list files in on file system
  //readFile(SPIFFS, "/jquery.min.js"); //read file on file system
  
  // put your main code here, to run repeatedly:
  WiFiClient client = server.available(); //bruges til signal fra client

  if(client){
    Serial.println("Ny Client.");

    String currentLine = "";

    while(client.connected()) {   //While loop for clients forbindelse
      if(client.available()) {    //if bytes for at læse client
        char c = client.read();   //læs byte
        Serial.write(c);          //print seriel monitor
        header += c;              
        if (c == '\n') {             //hvis byte er ny linje character
          if(currentLine.length() == 0){  //HTTP
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            //Vis HTML hjemmesiden
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
          //CSS Til style ved på og fra knap

          client.println("<style>body{ text-align: center; font-family:\"Trebuchet MS\", Arial; margin-left:auto; margin-right:auto;}");
          client.println(".slider{ width: 300px;}</style>");
          client.println("<script type = \"text/javascript\">");
          client.println("<src=\"/jquery.min.js\"/script>");
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
            valueString2 = header.substring(pos1+1, pos2);

            //Rotere servo
            myservo.write(valueString.toInt());
            myservo2.write(2*(valueString.toInt()));
            Serial.println(valueString.toInt());
            Serial.println(2*(valueString.toInt()));
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
