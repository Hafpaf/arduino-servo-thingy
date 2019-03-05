#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
 
//SSID and Password to ESP Acces Point
const char* ssid = "ESP8266_AP";
const char* password = "testnet1";

/* Put IP Address details */
IPAddress local_ip(192,168,2,1);
IPAddress gateway(192,168,2,1);
IPAddress subnet(255,255,255,0);

//WiFi.mode(WIFI_STA);   //This line hides the viewing of ESP as wifi network
 
ESP8266WebServer server(80); //Server on port 80

//==============================================================
//                 Webpage genereated
//==============================================================
void handleRoot() {
  server.send(200, "text/plain", "ESP8266 AP server is working.");
}
 
//===============================================================
//                  SETUP
//===============================================================
void setup(void){
  Serial.begin(9600);
  Serial.println("");
  WiFi.mode(WIFI_AP);           //Only Access point
  WiFi.softAP(ssid, password);  //Start Hotspot removing password will disable security
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
 
  IPAddress myIP = WiFi.softAPIP(); //Get IP address
  Serial.print("HotSpot IP:");
  Serial.println(myIP);
 
  server.on("/", handleRoot);      //Which routine to handle at root location
 
  server.begin();                  //Start server
  Serial.println("HTTP server started");
}
//===============================================================
//                     LOOP
//===============================================================
void loop(void){
  server.handleClient();          //Handle client requests
}
