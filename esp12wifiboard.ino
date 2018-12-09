#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h>
 
// put your service values here
const char* ssid = "Your SSID";
const char* password = "Your Password";

#define DEBUG 1  // Debug on serial monitor [0|1]

// 
const int LDR = 99;

// RGB Led
const int GREEN = 12;
const int BLUE = 13;
const int RED = 15;

// sm LED
const int LED1 = 2;
const int LED2 = 0;
const int LED3 = 4;
const int LED4 = 5;
const int LED5 = 14;
const int LED6 = 16;

//WiFiServer server(80);
ESP8266WebServer server(80);

void setLed() {
  
  String led_num = server.arg("led");
  String enable = server.arg("enable");

  int pin = LED1; // Default
  if (led_num == "led1") pin = LED1;
  if (led_num == "led2") pin = LED2;
  if (led_num == "led3") pin = LED3;
  if (led_num == "led4") pin = LED4;
  if (led_num == "led5") pin = LED5;
  if (led_num == "led6") pin = LED6;

  // all Leds are active on LOW state!
  digitalWrite(pin,(enable == "1")?LOW:HIGH);
    
  if(DEBUG) {
    Serial.print("led N. ");
    Serial.print(led_num);
    Serial.println((enable == "1")?" ON":" OFF");
  }  
  server.send(200, "text/html", "ok");  
}

void setRGBLed() {
  String red = server.arg("red");
  String green = server.arg("green");  
  String blue = server.arg("blue");  

  int r = red.toInt();
  int g = green.toInt();
  int b = blue.toInt();
  r = (r * 1023) / 255;
  g = (g * 1023) / 255;
  b = (b * 1023) / 255;

  if(DEBUG) {
    Serial.print("RGB values - R: ");
    Serial.print(r);
    Serial.print(" G: ");
    Serial.print(g);
    Serial.print(" B: ");
    Serial.println(b);
  } 

  analogWrite(RED, r); 
  analogWrite(GREEN, g); 
  analogWrite(BLUE, b); 

  server.send(200, "text/html", "ok");
}
 
void setup() {
  if(DEBUG){
    Serial.begin(115200);
  }

  pinMode(LED1, OUTPUT); 
  pinMode(LED2, OUTPUT); 
  pinMode(LED3, OUTPUT); 
  pinMode(LED4, OUTPUT); 
  pinMode(LED5, OUTPUT); 
  pinMode(LED6, OUTPUT); 
  pinMode(GREEN, OUTPUT); 
  pinMode(BLUE, OUTPUT); 
  pinMode(RED, OUTPUT); 

  // set all OFF
  
  // the sm leds
  digitalWrite(LED1, HIGH);  
  digitalWrite(LED2, HIGH);  
  digitalWrite(LED3, HIGH);  
  digitalWrite(LED4, HIGH);  
  digitalWrite(LED5, HIGH);  
  digitalWrite(LED6, HIGH);
  
  // the RGB LED
  analogWrite(GREEN,0);
  analogWrite(BLUE,0);    
  analogWrite(RED,0);  
  
  // set a low frequency to help stabilize the PWM
  analogWriteFreq(200);
  delay(5000);
  Serial.println("working...");
  
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    if(DEBUG){
      Serial.print(".");
    }
  }
  if(DEBUG){
    Serial.println("");
    Serial.println("WiFi connected");
  }
  
  // Start the server
  if (!SPIFFS.begin()) {
    if(DEBUG) {
      // Serious problem
      Serial.println("SPIFFS Mount failed");
    }
  } else {
    if(DEBUG) {
      Serial.println("SPIFFS Mount succesfull");
    }
  }

  server.on("/setLed", setLed);
  server.on("/setRGBLed", setRGBLed);

  server.serveStatic("/", SPIFFS, "/index.html");

  server.begin();
  Serial.println ( "HTTP server started" );
  if(DEBUG){
    Serial.println("Server started");
    Serial.println(WiFi.localIP());
  }
}
 
void loop() {

   server.handleClient();
 
}
 
