#include <WiFiNINA.h>
#include <Servo.h>

// ==== WiFi instellingen ====
const char* ssid = "AndroidAP1FE0"; // Vul in
const char* password = "ajkt4748";  // Vul in

WiFiServer server(80);

// ==== Servo setup ====
Servo servo1, servo2, servo3, servo4;

int pattern = 0; // 0=stop, 1=patroon1, 2=patroon2
bool toggleState1 = false;
bool toggleState2 = false;
unsigned long lastToggle1 = 0;
unsigned long lastToggle2 = 0;

const unsigned long intervalPattern1 = 2000;
const unsigned long intervalPattern2 = 4000;

void setup() {
  Serial.begin(9600);
  while (!Serial && millis() < 5000) ; // wacht op Serial (optioneel)

  // Servo’s initialiseren
  servo1.attach(3);
  servo2.attach(5);
  servo3.attach(6);
  servo4.attach(9);

  servo1.write(90);
  servo2.write(90);
  servo3.write(90);
  servo4.write(90);

  // ==== WiFi verbinden ====
  Serial.print("Verbinden met WiFi...");
  WiFi.begin(ssid, password);

  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    // eventuele timeout: na 30s opnieuw proberen
    if (millis() - start > 30000) {
      Serial.println("\nVerbinden duurt te lang. Probeer opnieuw of controleer credentials.");
      start = millis();
    }
  }

  Serial.println("\n✅ WiFi verbonden!");
  Serial.print("Arduino IP adres: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("Client connected.");
    // Lees volledige request (kopregels) naar een String
    String request = "";
    unsigned long timeout = millis() + 1000; // 1s timeout om niet te blokkeren
    while (client.connected() && millis() < timeout) {
      while (client.available()) {
        String line = client.readStringUntil('\n'); // lees tot newline
        if (line.endsWith("\r")) line.remove(line.length()-1); // verwijder CR
        // lege regel betekent einde headers
        if (line.length() == 0) {
          timeout = 0; // signaleert einde
          break;
        }
        request += line;
        request += '\n';
      }
    }

    Serial.println("Request ontvangen:");
    Serial.println(request);

    // Behandel favicon verzoek direct (geen 404)
    if (request.indexOf("favicon.ico") != -1) {
      client.println("HTTP/1.1 204 No Content");
      client.println("Connection: close");
      client.println();
      client.stop();
      Serial.println("favicon.ico -> 204 No Content verzonden");
      return;
    }

    // Commando herkennen op basis van eerste regel of path
    if (request.indexOf("/cmd/0") != -1) pattern = 0;
    else if (request.indexOf("/cmd/1") != -1) pattern = 1;
    else if (request.indexOf("/cmd/2") != -1) pattern = 2;

    // JSON terugsturen + CORS header zodat fetch() in browser werkt
    String json = "{\"pattern\":" + String(pattern) + "}";
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: application/json");
    client.println("Access-Control-Allow-Origin: *");
    client.println("Connection: close");
    client.print("Content-Length: ");
    client.println(json.length());
    client.println();
    client.println(json);
    delay(1); // geef client tijd om te versturen
    client.stop();
    Serial.print("Reactie verzonden: ");
    Serial.println(json);
  }

  // === Servo-patronen (gescheiden timers) ===
  unsigned long now = millis();

  if (pattern == 1 && now - lastToggle1 > intervalPattern1) {
    lastToggle1 = now;
    toggleState1 = !toggleState1;
    int pos = toggleState1 ? 0 : 180;
    servo1.write(pos);
    servo2.write(pos);
    Serial.print("Patroon1 toggle -> pos=");
    Serial.println(pos);
  }

  if (pattern == 2 && now - lastToggle2 > intervalPattern2) {
    lastToggle2 = now;
    toggleState2 = !toggleState2;
    int pos = toggleState2 ? 0 : 180;
    servo3.write(pos);
    servo4.write(pos);
    Serial.print("Patroon2 toggle -> pos=");
    Serial.println(pos);
  }
}
