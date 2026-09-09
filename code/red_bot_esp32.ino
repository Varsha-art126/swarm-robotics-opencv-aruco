// Red (follower) robot firmware — ESP32, WiFi + UDP motor control
// Extracted from WhatsApp self-chat, 9 Sept 2026

#include <WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
unsigned int localPort = 3333;

// --- RED BOT PINS (From your snippet) ---
#define ENA 15 // Left motor PWM
#define ENB 25 // Right motor PWM
#define IN1 2  // Left motor direction 1
#define IN2 4  // Left motor direction 2
#define IN3 27 // Right motor direction 1
#define IN4 26 // Right motor direction 2

WiFiUDP Udp;
char packetBuffer[255];
int leftPWM = 200;
int rightPWM = 200;

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
      // LEFT motor forward
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      // RIGHT motor forward
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      analogWrite(ENA, leftPWM);
      analogWrite(ENB, rightPWM);
    }
    else if (cmd == "backward") {
      // LEFT motor backward
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      // RIGHT motor backward
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      analogWrite(ENA, leftPWM);
      analogWrite(ENB, rightPWM);
    }
    else if (cmd == "left") {
      // LEFT backward, RIGHT forward
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      analogWrite(ENA, leftPWM);
      analogWrite(ENB, rightPWM);
    }
    else if (cmd == "right") {
      // LEFT forward, RIGHT backward
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      analogWrite(ENA, leftPWM);
      analogWrite(ENB, rightPWM);
    }
    else if (cmd == "stop") {
      analogWrite(ENA, 0);
      analogWrite(ENB, 0);
    }
  }
}
