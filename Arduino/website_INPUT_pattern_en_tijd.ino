#include <ArduinoBLE.h>
#include <Servo.h>

const int SERVO_SPEED = 15; 

struct SmoothServo {
  Servo servo;
  int currentPos;
  int targetPos;
  unsigned long lastMoveTime;
  int stepDelay;

  void attach(int pin, int startPos, int speed) {
    servo.attach(pin);
    currentPos = startPos;
    targetPos = startPos;
    stepDelay = speed;
    servo.write(currentPos);
  }

  void update() {
    if (currentPos == targetPos) return;
    if (millis() - lastMoveTime >= stepDelay) {
      lastMoveTime = millis();
      if (currentPos < targetPos) currentPos++;
      else currentPos--;
      servo.write(currentPos);
    }
  }

  void setTarget(int pos) {
    targetPos = pos;
  }
};

SmoothServo thumb, indexF, middleF, ringF, pinkF;
int pins[] = {3, 5, 6, 9, 10};

// Variabelen voor sessiebeheer
int pattern = 0;
int durationMinutes = 0;
bool running = false;
unsigned long sessionStartTime = 0;
unsigned long lastToggle = 0;
bool toggleState = false;

// Intervallen voor de patronen (tijd tussen bewegingen)
unsigned long interval;
bool wasConnected = false;

BLEService exoService("12345678-1234-5678-1234-56789abcdef0");
// Characteristic ingesteld op 2 bytes: [Code, Minuten]
BLECharacteristic commandChar("abcdef01-1234-5678-1234-56789abcdef0", BLEWrite | BLEWriteWithoutResponse, 2);

void onWrite(BLEDevice central, BLECharacteristic characteristic) {
  if (characteristic.valueLength() >= 2) {
    const uint8_t* data = characteristic.value();
    pattern = data[0];
    durationMinutes = data[1];

    Serial.println("------------------------------------");
    Serial.println("Nieuwe Revalidatie Sessie Ontvangen");
    Serial.print("Oefening Type: "); Serial.println(pattern);
    Serial.print("Sessie Duur:   "); Serial.print(durationMinutes); Serial.println(" minuut/minuten");

    if (pattern >= 1 && pattern <= 5 && durationMinutes > 0) {
      running = true;
      toggleState = false;
      lastToggle = millis();
      sessionStartTime = millis(); 
      Serial.println("Status: Sessie GESTART");
    } else {
      Serial.println("Status: STOP of Ongeldige invoer");
      stopAllServos();
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(2000); 

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  Serial.println("Systeem wordt opgestart...");
  thumb.attach(pins[0], 0, SERVO_SPEED);    delay(150);
  indexF.attach(pins[1], 0, SERVO_SPEED);   delay(150);
  middleF.attach(pins[2], 0, SERVO_SPEED);  delay(150);
  ringF.attach(pins[3], 0, SERVO_SPEED);    delay(150);
  pinkF.attach(pins[4], 0, SERVO_SPEED);    delay(150);

  stopAllServos(); 

  if (!BLE.begin()) {
    Serial.println("Fout: BLE-module niet gevonden!");
    while (1);
  }

  BLE.setLocalName("ExoRehab-WebBLE");
  BLE.setAdvertisedService(exoService);
  exoService.addCharacteristic(commandChar);
  BLE.addService(exoService);
  commandChar.setEventHandler(BLEWritten, onWrite);
  BLE.advertise();

  Serial.println("Klaar voor verbinding via Bluetooth.");
  digitalWrite(LED_BUILTIN, HIGH); 
}

void loop() {
  thumb.update();
  indexF.update();
  middleF.update();
  ringF.update();
  pinkF.update();

  BLEDevice central = BLE.central();
  bool isConnected = central && central.connected();

  if (isConnected && !wasConnected) {
    Serial.println("Status: App verbonden.");
    digitalWrite(LED_BUILTIN, LOW); 
  }

  if (!isConnected && wasConnected) {
    Serial.println("Status: Verbinding verbroken. Oefening gestopt.");
    stopAllServos();
    digitalWrite(LED_BUILTIN, HIGH); 
  }

  wasConnected = isConnected;

  if (running) {
    // Check sessie timer
    unsigned long sessionElapsed = millis() - sessionStartTime;
    unsigned long sessionLimit = (unsigned long)durationMinutes * 1000; //tijd voorlopig aangepast naar sseconden in plaats van minuten

    if (sessionElapsed >= sessionLimit) {
      Serial.println("------------------------------------");
      Serial.println("SESSIE VOLTOOID: Tijd is verstreken.");
      stopAllServos();
    } else {
      runPatternLogic();
    }
  }
}

void runPatternLogic() {
  unsigned long now = millis();
  int speedDelay = (180 * SERVO_SPEED) + 800; // Dynamische pauze tussen bewegingen

  switch (pattern) {
    case 1: // Volledige vuist
      if (now - lastToggle >= speedDelay) {
        toggleState = !toggleState;
        lastToggle = now;
        setAllTargets(toggleState ? 180 : 0);
        Serial.println(toggleState ? "Actie: Vuist maken" : "Actie: Hand openen");
      }
      break;

    case 2: // Vinger-per-vinger
      static int vingerStep = 0;
      if (now - lastToggle >= (SERVO_SPEED * 180 + 200)) {
        lastToggle = now;
        vingerStep = (vingerStep + 1) % 10;
        int pos = (vingerStep < 5) ? 180 : 0;
        int v = vingerStep % 5;
        if (v == 0) thumb.setTarget(pos);
        else if (v == 1) indexF.setTarget(pos);
        else if (v == 2) middleF.setTarget(pos);
        else if (v == 3) ringF.setTarget(pos);
        else pinkF.setTarget(pos);
        Serial.print("Actie: Vinger "); Serial.print(v); Serial.println(pos == 180 ? " sluiten" : " openen");
      }
      break;

    case 3: // Pinch grip (Duim + Wijsvinger)
      if (now - lastToggle >= speedDelay) {
        toggleState = !toggleState;
        lastToggle = now;
        int pos = toggleState ? 180 : 0;
        thumb.setTarget(pos);
        indexF.setTarget(pos);
        middleF.setTarget(0); ringF.setTarget(0); pinkF.setTarget(0);
        Serial.println("Actie: Pinch grip");
      }
      break;

    case 4: // Flexiegolf
      static int waveStep = 0;
      if (now - lastToggle >= 400) { // Snellere opeenvolging voor golf-effect
        lastToggle = now;
        waveStep = (waveStep + 1) % 10;
        int pos = (waveStep < 5) ? 180 : 0;
        int v = waveStep % 5;
        if (v == 0) pinkF.setTarget(pos); // Golf begint bij pink
        else if (v == 1) ringF.setTarget(pos);
        else if (v == 2) middleF.setTarget(pos);
        else if (v == 3) indexF.setTarget(pos);
        else thumb.setTarget(pos);
      }
      break;

    case 5: // Vuist zonder duim
      if (now - lastToggle >= speedDelay) {
        toggleState = !toggleState;
        lastToggle = now;
        int pos = toggleState ? 180 : 0;
        thumb.setTarget(0); // Duim blijft open
        indexF.setTarget(pos); middleF.setTarget(pos); ringF.setTarget(pos); pinkF.setTarget(pos);
        Serial.println("Actie: Vier vingers flexie");
      }
      break;
  }
}

void setAllTargets(int pos) {
  thumb.setTarget(pos); indexF.setTarget(pos); middleF.setTarget(pos); ringF.setTarget(pos); pinkF.setTarget(pos);
}

void stopAllServos() {
  setAllTargets(0);
  running = false;
  pattern = 0;
  durationMinutes = 0;
}