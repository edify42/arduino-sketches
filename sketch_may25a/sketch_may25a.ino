#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

/*Put your SSID & Password*/
const char* ssid = "TelstraC7DB71_2GEXT";  // Enter SSID here
const char* password = "f9vnhue8xw";  //Enter Password here

// the IP address for the shield:
IPAddress ip(192, 168, 0, 147);  
IPAddress gateway(192,168,0,1);   
IPAddress subnet(255,255,255,0);   

ESP8266WebServer server(80);

uint8_t LEDpin = 2;                     // debug pin 2 with LED
uint8_t SWITCHpin = 0;                  // real I/O pin for
bool LEDstatus = LOW;
bool timing = false;                    // logic to activate the timing circuit.
unsigned long interval = 3000;          // 3 seconds to wait before shutting off LED.
unsigned long currentMillis = 0;        // stores the value of millis() in each iteration of loop()
unsigned long previousMillis = 0;       // set this with interval when the button is hit.

void setup() {
  Serial.begin(9600);
  delay(100);
  pinMode(LEDpin, OUTPUT);
  pinMode(SWITCHpin, OUTPUT);

  Serial.println("Connecting to ");
  Serial.println(ssid);

  WiFi.config(ip, gateway, subnet);

  //connect to your local wi-fi network
  WiFi.begin(ssid, password);

  //check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());

  server.on("/", handle_OnConnect);
  server.on("/ledon", handle_ledon);
  server.on("/ledoff", handle_ledoff);
  server.on("/timer", handle_timer);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
}
void loop() {
  server.handleClient();
  currentMillis = millis();
  if(timing) {
    if(currentMillis - previousMillis >= interval){
      previousMillis = 0;
      LEDstatus = LOW;
      timing = false;
    }
  }
  if(LEDstatus)
  ctl_pins(HIGH);
  else
  ctl_pins(LOW); 
}

void ctl_pins(bool option){
  digitalWrite(LEDpin, !option);
  digitalWrite(SWITCHpin, option);
}

void handle_OnConnect() {
  LEDstatus = LOW;
  server.send(200, "text/html", SendHTML(false)); 
}

void handle_ledon() {
  LEDstatus = HIGH;
  server.send(200, "text/html", SendHTML(true)); 
}

void handle_ledoff() {
  LEDstatus = LOW;
  server.send(200, "text/html", SendHTML(false)); 
}

void handle_timer() {
  LEDstatus = HIGH;
  timing = true;
  previousMillis = millis();   // capture the latest value of millis()
  server.send(200, "text/html", SendHTML(true));  
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String SendHTML(uint8_t led){
  String ptr = "<!DOCTYPE html>\n";
  ptr +="<html>\n";
  ptr +="<head>\n";
  ptr +="<title>LED Control</title>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>LED</h1>\n";
  ptr +="<p>Click to switch LED on and off.</p>\n";
  ptr +="<form method=\"get\">\n";
  if(led)
  ptr +="<input type=\"button\" value=\"LED OFF\" onclick=\"window.location.href='/ledoff'\">\n";
  else
  ptr +="<input type=\"button\" value=\"LED ON\" onclick=\"window.location.href='/ledon'\">\n";
  ptr +="</form>\n";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
