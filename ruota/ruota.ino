#include <WiFi.h>

#define SendKey 0  //Button to send data Flash BTN on NodeMCU

//Server connect to WiFi Network
const char *ssid = "peppa";  //Enter your wifi SSID
const char *password = "abcdefgh";  //Enter your wifi Password
int port = 8888;  //Port number

const int nLeds = 40;
const int angleCoeff = 360/5;

// array size nLeds / 8 * angleCoeff

uint8_t image[] = 
{
  32,127,255,254,255,16,0,66,31,143,16,0,0,63,249,16,0,63,255,255,16,0,127,255,255,16,3,255,255,255,16,15,255,255,255,16,31,31,255,
  255,16,14,15,255,252,24,0,15,255,195,8,0,127,254,63,12,0,127,251,255,12,0,255,131,255,7,1,254,124,127,3,129,253,255,135,0,255,243,
  255,248,0,15,231,255,252,0,3,207,255,255,0,3,159,255,255,0,1,159,255,255,0,1,63,255,252,0,3,63,255,199,0,2,127,254,63,0,62,127,225,
  255,0,63,127,135,255,0,255,124,15,255,0,255,120,127,255,0,255,113,255,255,0,63,131,255,255,0,31,135,255,255,0,7,143,239,255,0,7,7,
  255,255,0,7,3,255,255,0,15,48,255,255,0,14,120,255,241,0,14,126,31,15,7,254,127,0,255,3,254,63,192,255,7,254,63,7,143,7,192,60,63,
  249,7,0,56,255,255,6,0,16,255,255,0,0,3,255,255,0,0,3,251,255,0,0,3,255,255,0,0,64,255,255,0,1,204,127,255,0,3,222,15,255,0,3,159,129,
  255,0,7,223,240,255,0,7,207,191,15,0,15,207,255,241,0,15,207,255,255,0,15,231,255,255,0,15,227,255,255,0,15,240,255,255,0,31,248,255,255,
  0,255,252,63,248,7,239,254,31,143,15,135,255,128,255,12,7,255,240,255,12,3,255,252,31,8,1,255,255,241,24,0,255,255,254,16,0,63,255,255,16,
  28,15,255,255,16,31,7,255,255,16,3,15,254,255,16,0,15,255,255,16,0,31,255,255,16,126,63,249,255,16,127,255,255,255,
};

WiFiServer server(port);

void setup() 
{
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password); //Connect to wifi
 
  // Wait for connection  
  Serial.println("Connecting to Wifi");
  while (WiFi.status() != WL_CONNECTED) {   
    delay(500);
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  
  server.begin();
  Serial.print("Open Telnet and connect to IP:");
  Serial.print(WiFi.localIP());
  Serial.print(" on port ");
  Serial.println(port);
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("Client connesso");
    while (client.connected()) {
      while (client.available() > 0) {
        Serial.write(client.read()); 
      }
    }
    client.stop();
    Serial.println("Client disconnesso");
  }
}
