#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoOTA.h>

#define PWM_CHANNEL_A 0
#define PWM_CHANNEL_B 1
#define PWM_FREQ 5000
#define PWM_RESOLUTION 8

// Definições dos pinos do ESP32
int PWM_A = -1;
int PWM_B = -1;
int DIR_A = 5;
int DIR_B = 18;
int TA = 19; 
int TB = 21;

const int buzPin = 12;     
const int ledPin = 2;      
const int wifiLedPin = 4;  

String command;
int SPEED = 255;        
int speed_Coeff = 1;

WebServer server(80);

unsigned long previousMillis = 0;

String sta_ssid = "SEU_SSID";      
String sta_password = "SUA_SENHA";  


void HTTP_handleRoot() {
  server.send(200, "text/html", "");

  if (server.hasArg("State")) {
    Serial.println(server.arg("State"));
  }
}

void Forward() {
  digitalWrite(DIR_A, HIGH);
  digitalWrite(DIR_B, LOW);
  digitalWrite(TA, LOW);
  digitalWrite(TB, HIGH);
}

void Backward() {
  digitalWrite(DIR_A, LOW);
  digitalWrite(DIR_B, HIGH);
  digitalWrite(TA, HIGH);
  digitalWrite(TB, LOW);

}

void TurnRight() {
  digitalWrite(DIR_A, HIGH);
  digitalWrite(DIR_B, LOW);
  digitalWrite(TA, HIGH);
  digitalWrite(TB, LOW);
}

void TurnLeft() {
  digitalWrite(DIR_A, LOW);
  digitalWrite(DIR_B, HIGH);
  digitalWrite(TA, LOW);
  digitalWrite(TB, HIGH);
}

void ForwardLeft() {
  //frente esquerda
  digitalWrite(DIR_A, LOW);
  digitalWrite(DIR_B, LOW);
  digitalWrite(TA, LOW);
  digitalWrite(TB, HIGH);
  
}

void ForwardRight() {
  digitalWrite(DIR_A,  HIGH);
  digitalWrite(DIR_B, LOW);
  digitalWrite(TA, LOW);
  digitalWrite(TB, LOW);

}

void BackwardLeft() {
//tras esquerda
  digitalWrite(DIR_A, LOW);
  digitalWrite(DIR_B, HIGH);
  digitalWrite(TA, LOW);
  digitalWrite(TB, LOW);
}

void BackwardRight() {
    digitalWrite(DIR_A, LOW);
  digitalWrite(DIR_B, LOW);
  digitalWrite(TA, HIGH);
  digitalWrite(TB, LOW);
}

void Stop() {
  digitalWrite(DIR_A, LOW);
  digitalWrite(DIR_B, LOW);
  digitalWrite(TA, LOW);
  digitalWrite(TB, LOW);

}

void setup() {
  Serial.begin(115200);
  Serial.println("\n*WiFi Robot Remote Control Mode*");

  pinMode(DIR_A, OUTPUT);
  pinMode(DIR_B, OUTPUT);
  pinMode(TA, OUTPUT);
  pinMode(TB, OUTPUT);
  digitalWrite(DIR_A, LOW);
  digitalWrite(DIR_B, LOW);
  digitalWrite(TA, LOW);
  digitalWrite(TB, LOW);
 
  Serial.println("Conectando ao WiFi...");
  WiFi.mode(WIFI_AP);
  WiFi.softAP("ESP32_ROBOT");
  Serial.println("\nModo AP ativado!");
  Serial.print("IP AP: ");
  Serial.println(WiFi.softAPIP()); 
  digitalWrite(wifiLedPin, HIGH);
  server.on("/", HTTP_handleRoot);
  server.onNotFound(HTTP_handleRoot); 
  server.begin();
  ArduinoOTA.begin();
}

void loop() {
  ArduinoOTA.handle();
  server.handleClient();
  command = server.arg("State");
  if (command == "F") Forward();
  else if (command == "B") Backward();
  else if (command == "R") TurnRight();
  else if (command == "L") TurnLeft();
  else if (command == "G") ForwardLeft();
  else if (command == "H") BackwardLeft();
  else if (command == "I") ForwardRight();
  else if (command == "J") BackwardRight();
  else if (command == "S") Stop();
}
