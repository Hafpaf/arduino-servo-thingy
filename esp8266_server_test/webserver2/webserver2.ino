#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

ESP8266WebServer Webserver(80);

const char* ssid = "Labitat (free)";
const char* password = "labitatisawesome";

enum stage_names {
  ledON, ledOFF
};
static stage_names stage = ledON;

//String HTMLpage = "";
String out;
int LED = BUILTIN_LED;

/*
void setup(void){
  HTMLpage += "<head><title>Webserver Tutorial</title></head><h3>ESP8266 Webserver Demo (Toggle LED)</h3><p>LED <a href=\"ledON\"><button>ON</button></a>&nbsp;<a href=\"ledOFF\"><button>OFF</button></a></p>";
*/

void HTMLpage() {
  digitalWrite(LED, 1);
  String out;
  out = "<html><head><title>Labitat test webserver wifi light</title></head>";
  out += "<font color=\"#021640\"><body bgcolor=\"#ff8d33\">"; 
  out += "<body style='background-color:#ff8d33'>"; // ffe633 = yellow for bedroom
  out += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=yes\">";
  out += "<FONT SIZE=-1>";
  out += "<h2>Labitat test wifi neopixel light</h2>";
  out += "<br/>";
  out += "Turn led On/OFF<br>";
  out += "<a href=\"ledON\"><button>ON</button></a>";
  out += "<a href=\"ledOFF\"><button>OFF</button></a>";
  out += "<br/>";
  out += "Drive<br>";
  out += "<a href=\"ledON\"><button>ON</button></a>";
  out += "<a href=\"ledOFF\"><button>OFF</button></a>";
  out += "<br/>";
/*  out += "Rainbowcycle<br>";
  out += "<a href=\"rainbowcycle_120\"><button>really slow</button></a>";
  out += "<a href=\"rainbowcycle_100\"><button>pretty slow</button></a>";
  out += "<a href=\"rainbowcycle_90\"><button>slower</button></a>";
  out += "<a href=\"rainbowcycle_60\"><button>slow</button></a>";
  out += "<a href=\"rainbowcycle_30\"><button>normal</button></a><br><br>";
  out += "Brightness<br>";
  out += "<a href=\"b200\"><button>50%</button></a>";
  out += "<a href=\"b250\"><button>100%</button></a><br><br>";
  out += "Colors<br>";
  out += "<a href=\"bright\"><button>white</button></a>";
  out += "<a href=\"yellow\"><button>yellow</button></a><br>";  
  out += "<a href=\"orange\"><button>orange</button></a>";
  out += "<a href=\"red\"><button>red</button></a><br>";
  out += "<a href=\"purple\"><button>purple</button></a>";
  out += "<a href=\"blue\"><button>blue</button></a><br>";
  out += "<a href=\"cyan\"><button>cyan</button></a>"; 
  out += "<a href=\"green\"><button>green</button></a><br><br>";
  out += "Extra<br>";
  out += "<a href=\"rainbow\"><button>plain rainbow</button></a><br><br>";
  out += "<a href=\"lightoff\"><button>-- Light OFF --</button></a><br><br>";*/
  out += "</body>";
  out += "</html>";
//Webserver.send(200, "text/plain", "Mit navn er ikke Kurt!");
  Webserver.send(200, "text/html", out);
  digitalWrite(LED, 0);
}

void handleNotFound() {
  digitalWrite(LED, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += Webserver.uri();
  message += "\nMethod: ";
  message += (Webserver.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += Webserver.args();
  message += "\n";
  for (uint8_t i=0; i<Webserver.args(); i++){
    message += " " + Webserver.argName(i) + ": " + Webserver.arg(i) + "\n";
  }
  Webserver.send(404, "text/plain", message);
  digitalWrite(LED, 0);
}

void setup(void) {
  pinMode(LED, OUTPUT);
  pinMode(D7, OUTPUT);
  pinMode(D8, OUTPUT);
  digitalWrite(LED, LOW);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");

  //Wait for connection
  while (WiFi.status() !=WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266", WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }

  Webserver.on("/", HTMLpage);
  Webserver.on ( "/ledON", []() {stage=ledON; HTMLpage();} );
  Webserver.on ( "/ledOFF", []() {stage=ledOFF; HTMLpage();} );

  Webserver.on("/", [](){
    Webserver.send(200, "text/html", out);
/*
  });
  Webserver.on("/ledON", [](){
    Webserver.send(200, "text/html", HTMLpage+"<p>DRIVE</p>");
    digitalWrite(D7, HIGH);
    delay(1000);
  });

  Webserver.on("/ledOFF", [](){
    Webserver.send(200, "text/html", HTMLpage+"<p>NO DRIVE</p>");
    digitalWrite(D8, HIGH);
    delay(1000);
  });
*/
  });
  Webserver.on("/ledON", [](){
    Webserver.send(200, "text/html", out+"<p>LED is ON</p>");
    digitalWrite(LED, LOW);
    delay(1000);
  });

  Webserver.on("/ledOFF", [](){
    Webserver.send(200, "text/html", out+"<p>LED is OFF</p>");
    digitalWrite(LED, HIGH);
    delay(1000);
  });
  Webserver.onNotFound(handleNotFound);

  Webserver.begin();
  Serial.println("HTTP Webserver started");
}

void loop(void){
  Webserver.handleClient();
    if (stage == ledON) {
    Webserver.send(200, "text/html", out+"<p>LED is ON</p>");
    digitalWrite(LED, LOW);
    delay(1000);
  } else if (stage == ledOFF) {
    Webserver.send(200, "text/html", out+"<p>LED is OFF</p>");
    digitalWrite(LED, HIGH);
    delay(1000);
//  } else if (stage == s_yellow) {
//    colorWipe(strip.Color(255, 255, 0), 20);
   }
}
