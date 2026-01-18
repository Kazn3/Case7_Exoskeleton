#include <WiFiNINA.h>
#include <Servo.h>

// === WiFi SETTINGS ===
const char* ssid = "AndroidAP1FE0";
const char* password = "ajkt4748";

WiFiServer server(80);

// === SERVO SETTINGS ===
Servo servo1, servo2, servo3, servo4;

const int restPos = 90;          // servo rusthoek
bool running = false;            // patroon actief?
int pattern = 0;                 // 0=stop, 1=patroon1, 2=patroon2

// Timers
unsigned long lastToggle1 = 0;
unsigned long lastToggle2 = 0;

const unsigned long interval1 = 2000;
const unsigned long interval2 = 4000;

bool state1 = false;
bool state2 = false;

// === BUTTONS ===
const int startBtn = 7;
const int stopBtn  = 8;

unsigned long lastDebounceStart = 0;
unsigned long lastDebounceStop  = 0;
const unsigned long debounceDelay = 50;

// === FUNCTIONS ===

// --- Zet servo's in ruststand met detach ---
void servosRest() {
  servo1.write(restPos);
  servo2.write(restPos);
  servo3.write(restPos);
  servo4.write(restPos);

  // detach voorkomt servo-trillen
  servo1.detach();
  servo2.detach();
  servo3.detach();
  servo4.detach();
}

// --- Start patroon: attach servo’s opnieuw ---
void startPattern() {
  running = true;

  servo1.attach(3);
  servo2.attach(5);
  servo3.attach(6);
  servo4.attach(9);

  state1 = false;
  state2 = false;

  lastToggle1 = millis();
  lastToggle2 = millis();
}

// --- Stop patroon volledig ---
void stopPattern() {
  running = false;
  pattern = 0;
  servosRest();
}

// === SETUP ===
void setup() {
  Serial.begin(9600);
  while (!Serial && millis() < 5000);

  // Buttons
  pinMode(startBtn, INPUT_PULLUP);
  pinMode(stopBtn, INPUT_PULLUP);

  // Servo’s beginnen in rust
  servosRest();

  // WiFi connectie
  Serial.print("Verbinden met WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }

  Serial.println("\nWiFi verbonden");
  Serial.print("IP adres: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

// === LOOP ===
void loop() {
  unsigned long now = millis();

  // ======= BUTTON HANDLING =======
  if (digitalRead(startBtn) == LOW) {
    if (now - lastDebounceStart > debounceDelay) {
      if (!running && pattern != 0) startPattern();
      lastDebounceStart = now;
    }
  }

  if (digitalRead(stopBtn) == LOW) {
    if (now - lastDebounceStop > debounceDelay) {
      stopPattern();
      lastDebounceStop = now;
    }
  }

  // ======= SERVO PATTERNS =======
  if (running) {

    if (pattern == 1 && now - lastToggle1 > interval1) {
      lastToggle1 = now;
      state1 = !state1;
      int pos = state1 ? 0 : 180;

      servo1.write(pos);
      servo2.write(pos);
    }

    if (pattern == 2 && now - lastToggle2 > interval2) {
      lastToggle2 = now;
      state2 = !state2;
      int pos = state2 ? 0 : 180;

      servo3.write(pos);
      servo4.write(pos);
    }
  }

  // ======= WIFI CLIENT HANDLING =======
  WiFiClient client = server.available();
  if (client) {
    String request = client.readString();

    // favicon negeren
    if (request.indexOf("favicon") != -1) {
      client.println("HTTP/1.1 204 No Content");
      client.println("Connection: close");
      client.println();
      return;
    }

    // pattern keuze
    if (request.indexOf("/cmd/0") != -1) {
      stopPattern();
    } 
    else if (request.indexOf("/cmd/1") != -1) {
      pattern = 1;
    } 
    else if (request.indexOf("/cmd/2") != -1) {
      pattern = 2;
    }

    // JSON terug
    String json = "{\"pattern\": " + String(pattern) +
                  ", \"running\": " + String(running) + "}";

    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: application/json");
    client.println("Access-Control-Allow-Origin: *");
    client.println("Connection: close");
    client.print("Content-Length: ");
    client.println(json.length());
    client.println();
    client.println(json);

    client.stop();
  }
}
