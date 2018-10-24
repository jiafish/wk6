#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

const char* ssid = "IOT_LAB";
const char* password = "password";
String website; 

WebServer server(80);

const int led = 13;

void buildWeb(){
  website +="<html>";
  website +="<head>\n";
  website +="<meta charset='UTF-8'/>\n>";
  website +="<title>Remote Control LED</title>\n";
  website +="<style>";
 website += "html{background-color:#00ff6e;}";
  website += ".title{text-align:center;color:#002eff;text-shadow:0px 3px 5px #000; margin-bottom: 50px;}";
  website += "a{display:block;text-decoration:none;background-color:#ff00e9;radius:3px; ";
  website += "width:150px;text-align:center;margin:0 auto;margin-bottom:30px;font-size:2em;box-shadow: 0px 3px 6px #000;}";
  website += ".btn1{color:#00ffc3;}";
  website += ".btn2{color:#ff0000;}";
  website += "</style>";
  website += "</head>\n";
  website += "<body>";
  website += "<h1 class=\"title\">Remote LED Control</h1>";
  website += "<a class=\"btn1\" href=\"/H\">ON</a>";
  website += "<a class=\"btn2\" href=\"/L\">OFF</a>";
  website += "</body></html>";
}

void handleRoot() {
  digitalWrite(led, 1);
  server.send(200, "text/html",website);
  digitalWrite(led, 0);
}

void handleNotFound() {
  digitalWrite(led, 1);
  String message =(404,"text.plain", "File Not Found\n\n");
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}
  void ledon(){ digitalWrite(led, 1);}
  void ledoff(){digitalWrite(led, 0);}

void setup(void) {
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
buildWeb();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }


  server.on("/", handleRoot);
  server.on("/H",ledon);
  server.on("/L", ledoff);
  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
}
