// Leader robot firmware — ESP32, WiFi + UDP motor control
// Extracted from WhatsApp self-chat, 9 Sept 2026

#include <WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
unsigned int localPort = 3333;

// --- LEADER PINS (From your snippet) ---
const int ENA = 19; // Right motor PWM
const int IN1 = 2;  // Right motor direction
const int IN2 = 4;
const int IN3 = 18; // Left motor direction
const int IN4 = 5;
const int ENB = 15; // Left motor PWM

WiFiUDP Udp;
char packetBuffer[255];
int motorSpeed = 200; // Renamed to match your snippet variable

void setup() {
  Serial.begin(115200);
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT); pinMode(ENA, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT); pinMode(ENB, OUTPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { delay(500); }
  Serial.println(WiFi.localIP());
  Udp.begin(localPort);
}

void loop() {
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    int len = Udp.read(packetBuffer, 255);
    if (len > 0) packetBuffer[len] = 0;
    String cmd = String(packetBuffer); cmd.trim();

    // --- YOUR EXACT LOGIC ---
    if (cmd == "forward") {
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      analogWrite(ENA, motorSpeed);
      analogWrite(ENB, motorSpeed);
    }
    else if (cmd == "backward") {
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      analogWrite(ENA, motorSpeed);
      analogWrite(ENB, motorSpeed);
    }
    else if (cmd == "left") {
      // PIVOT LEFT
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
      analogWrite(ENA, motorSpeed);
      analogWrite(ENB, 0);
    }
    else if (cmd == "right") {
      // PIVOT RIGHT
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      analogWrite(ENA, 0);
      analogWrite(ENB, motorSpeed);
    }
    else if (cmd == "stop") {
      analogWrite(ENA, 0);
      analogWrite(ENB, 0);
    }
  }
}
