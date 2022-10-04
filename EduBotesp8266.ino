#include <ESP8266WiFi.h>
 
const char* ssid = "Gehirnablage 4.0";
const char* password = "+50cminnach840Pete";
bool softwareAP = true;

int d0 = 16;
int d1 = 5;
int d2 = 4;
int d3 = 0;

String ipAddress = "";

WiFiServer server(80);
 
void setup() {
  Serial.begin(115200);
  delay(10);
  pinMode(d0, OUTPUT);
  pinMode(d1, OUTPUT);
  pinMode(d2, OUTPUT);
  pinMode(d3, OUTPUT);

  digitalWrite(d0, LOW);
  digitalWrite(d1, LOW);
  digitalWrite(d2, LOW);
  digitalWrite(d3, LOW);

  if(softwareAP)
  {
    WiFi.softAP("miau", "123456789");
  }
  else
  {
    Serial.print("Verbinden mit: "); 
    Serial.println(ssid);
    WiFi.begin(ssid, password);
  }

  if(softwareAP)
  {
    ipAddress = WiFi.softAPIP().toString();
    Serial.println();
    Serial.print("Server IP address: ");
    Serial.println(WiFi.softAPIP());
    Serial.print("Server MAC address: ");
    Serial.println(WiFi.softAPmacAddress());
  }
  else
  {
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    ipAddress = WiFi.localIP().toString();
  }

  Serial.println("");
  Serial.println("WiFi verbunden");
 
  // Start des Servers
  server.begin();
  Serial.println("Server gestartet");
 
  // Print the IP address
  Serial.print("Diese URL zum Verbinden aufrufen: ");
  Serial.print("http://");
  Serial.print(ipAddress);
  Serial.println("/");
}

void loop() {

  WiFiClient client = server.available();
  if (!client) {
    return;
  }
 
  // Warten auf Daten vom Client
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
 
  // Erste Zeile des Requests lesen
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
 
  // Match the request
  int value = LOW;
  Serial.println("Request is:");
  Serial.println(request);

  // rueckwarts
  if(request.indexOf("/keyDown=37") > -1)
  {
    digitalWrite(d3, HIGH);
  }
  if(request.indexOf("/keyDown=38") > -1)
  {
    digitalWrite(d1, HIGH);
  }
  if(request.indexOf("/keyDown=39") > -1)
  {
    digitalWrite(d0, HIGH);
  }
  if(request.indexOf("/keyDown=40") > -1)
  {
    digitalWrite(d2, HIGH);
  }

  if(request.indexOf("/keyUp=37") > -1)
  {
    digitalWrite(d3, LOW);
  }
  if(request.indexOf("/keyUp=38") > -1)
  {
    digitalWrite(d1, LOW);
  }
  if(request.indexOf("/keyUp=39") > -1)
  {
    digitalWrite(d0, LOW);
  }
  if(request.indexOf("/keyUp=40") > -1)
  {
    digitalWrite(d2, LOW);
  }

  client.println("<html><head></head><body>Diese Seite auswählen und dann sollte man mit den Pfeiltasten steuern können.</body><script>");
  client.println("function httpGet(theUrl) {  let xmlHttpReq = new XMLHttpRequest();  xmlHttpReq.open(\"GET\", theUrl, false);   xmlHttpReq.send(null);  return xmlHttpReq.responseText;};oldValue = -1;");
  client.println("function flipflop(value){ if(value == oldValue) { return false;  }  oldValue = value; return true;};");
  client.println("window.addEventListener(\"keyup\", function(e) { console.log('keyup' + e.keyCode); if(e.keyCode >= 37 && e.keyCode <= 40) { if(flipflop('keyup' + e.keyCode)) { console.log(\"send request\"); httpGet(\"http://" + ipAddress + "/keyUp=\" + e.keyCode); } }});");
  client.println("window.addEventListener(\"keydown\", function(e) {console.log('keydown' + e.keyCode);if(e.keyCode >= 37 && e.keyCode <= 40) { if(flipflop('keydown' + e.keyCode))  { console.log(\"send request\"); httpGet(\"http://" + ipAddress + "/keyDown=\" + e.keyCode); } }});");
  client.println("</script></html>");

  delay(1);
  Serial.println("TCP disconnected");
}