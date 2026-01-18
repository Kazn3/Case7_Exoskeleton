#include <ArduinoBLE.h>
#include <Servo.h>

// ==========================================
// === INSTELLINGEN VOOR MG996R SNELHEID ===
// ==========================================
// Tijd in milliseconden tussen elke graad verandering
// MG996R is zwaar, te snel = trillen of stuiteren
const int SERVO_SNELHEID = 30; // 25-35 ms is soepel

// ==========================================

// Een structuur om soepele beweging te beheren zonder 'delay'
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

  void stopImmediate(int pos) {
    targetPos = pos;
  }
};

// Clamp functie om servo niet te overbelasten
int clampServo(int pos) {
  if (pos < 5) return 5;
  if (pos > 175) return 175;
  return pos;
}

// === Servo Objecten ===
SmoothServo s1, s2, s3, s4, s5;
const int servoPins[] = {3, 5, 6, 9, 10};

// === Patroon-instellingen ===
int pattern = 0;
unsigned long lastToggle = 0;
bool toggleState = false;

// Intervallen per patroon (ms)
unsigned long intervalPattern1, intervalPattern2, intervalPattern3, intervalPattern4, intervalPattern5;

// === Manuele knop instellingen ===
const int buttonPin = 2;
bool buttonState = false;
bool lastButtonState = false;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

bool running = false;

// === BLE ===
BLEService servoService("180A");
BLEStringCharacteristic commandCharacteristic("2A57", BLEWrite, 20);

void setup() {
  Serial.begin(9600);

  // Servo's initialiseren
  s1.attach(servoPins[0], 0, SERVO_SNELHEID);
  s2.attach(servoPins[1], 0, SERVO_SNELHEID);
  s3.attach(servoPins[2], 0, SERVO_SNELHEID);
  s4.attach(servoPins[3], 0, SERVO_SNELHEID);
  s5.attach(servoPins[4], 0, SERVO_SNELHEID);

  // Intervallen automatisch berekenen
  intervalPattern1 = (180 * SERVO_SNELHEID) + 500;
  intervalPattern2 = (180 * SERVO_SNELHEID) + 2000;
  intervalPattern3 = (120 * SERVO_SNELHEID) + 500;
  intervalPattern4 = (90 * SERVO_SNELHEID) + 500;
  intervalPattern5 = (180 * SERVO_SNELHEID) + 200;

  pinMode(buttonPin, INPUT_PULLUP);

  if (!BLE.begin()) {
    Serial.println("❌ BLE-start mislukt!");
    while (1);
  }

  BLE.setLocalName("NanoIOT-ServoCtrl");
  BLE.setAdvertisedService(servoService);
  servoService.addCharacteristic(commandCharacteristic);
  BLE.addService(servoService);

  commandCharacteristic.writeValue("0");
  BLE.advertise();

  Serial.println("✅ Klaar! MG996R ready.");
}

void loop() {
  // Servo updates
  s1.update();
  s2.update();
  s3.update();
  s4.update();
  s5.update();

  BLEDevice central = BLE.central();
  if (central && central.connected()) {
    if (commandCharacteristic.written()) {
      String cmd = commandCharacteristic.value();
      handleCommand(cmd);
    }
    checkButton();

    if (running) runPatternLogic();
    else stopAllServos();
  }
}

// ====== Hulpfuncties ======

void handleCommand(String cmd) {
  if (cmd == "0") {
    pattern = 0;
    running = false;
    Serial.println("⏹ Stop commando");
  } else {
    pattern = cmd.toInt();
    if (pattern >= 1 && pattern <= 5) {
      Serial.print("▶️ Patroon ");
      Serial.print(pattern);
      Serial.println(" geselecteerd");
    }
  }
}

void checkButton() {
  bool reading = !digitalRead(buttonPin);
  if (reading != lastButtonState) lastDebounceTime = millis();

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState) {
        running = !running;
        Serial.print("🔘 Knop: ");
        Serial.println(running ? "START" : "STOP");
      }
    }
  }
  lastButtonState = reading;
}

void runPatternLogic() {
  unsigned long currentMillis = millis();

  if (pattern == 1 && currentMillis - lastToggle >= intervalPattern1) {
    toggleState = !toggleState;
    lastToggle = currentMillis;
    int pos = toggleState ? 0 : 180;
    s1.setTarget(clampServo(pos));
    s2.setTarget(clampServo(pos));
  }
  else if (pattern == 2 && currentMillis - lastToggle >= intervalPattern2) {
    toggleState = !toggleState;
    lastToggle = currentMillis;
    int pos = toggleState ? 0 : 180;
    s3.setTarget(clampServo(pos));
    s4.setTarget(clampServo(pos));
  }
  else if (pattern == 3 && currentMillis - lastToggle >= intervalPattern3) {
    toggleState = !toggleState;
    lastToggle = currentMillis;
    int pos = toggleState ? 30 : 150;
    s1.setTarget(clampServo(pos));
    s5.setTarget(clampServo(pos));
  }
  else if (pattern == 4 && currentMillis - lastToggle >= intervalPattern4) {
    toggleState = !toggleState;
    lastToggle = currentMillis;
    int pos = toggleState ? 45 : 135;
    s1.setTarget(clampServo(pos));
    s2.setTarget(clampServo(pos));
    s3.setTarget(clampServo(pos));
    s4.setTarget(clampServo(pos));
    s5.setTarget(clampServo(pos));
  }
  else if (pattern == 5 && currentMillis - lastToggle >= intervalPattern5) {
    toggleState = !toggleState;
    lastToggle = currentMillis;
    int posA = toggleState ? 0 : 180;
    int posB = toggleState ? 180 : 0;
    s1.setTarget(clampServo(posA));
    s3.setTarget(clampServo(posA));
    s5.setTarget(clampServo(posA));
    s2.setTarget(clampServo(posB));
    s4.setTarget(clampServo(posB));
  }
}

void stopAllServos() {
  s1.setTarget(5);
  s2.setTarget(5);
  s3.setTarget(5);
  s4.setTarget(5);
  s5.setTarget(5);
}
