#include <ArduinoBLE.h>
#include <Servo.h>

// ===============================================================
// ========== INSTELBARE SNELHEID VAN ALLE SERVO'S ===============
// ===============================================================
const int SERVO_SPEED = 15; // ms per graad – 15 = mooi vloeiend

// ===============================================================
// ========== SMOOTH SERVO STRUCT =================================
// ===============================================================

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

// ===============================================================
// ========== SERVO DEFINITIE (EXOSKELET HAND) ====================
// ===============================================================

// Servo 1 = Duim
// Servo 2 = Wijsvinger
// Servo 3 = Middelvinger
// Servo 4 = Ringvinger
// Servo 5 = Pink

SmoothServo thumb, indexF, middleF, ringF, pinkF;
int pins[] = {3,5,6,9,10};

// ===============================================================
// ========== PATTERN VARIABELEN =================================
// ===============================================================

int pattern = 0;
bool running = false;
unsigned long lastToggle = 0;
bool toggleState = false;

// Patroon intervallen (worden in setup berekend)
unsigned long interval1, interval2, interval3, interval4, interval5;

// ===============================================================
// ========== BLE DEFINITIES =====================================
// ===============================================================

BLEService servoService("180A");
BLEStringCharacteristic commandCharacteristic("2A57", BLEWrite, 20);

// ===============================================================
// ========================= SETUP ================================
// ===============================================================

void setup() {
  Serial.begin(9600);

  thumb.attach(pins[0], 0, SERVO_SPEED);
  indexF.attach(pins[1], 0, SERVO_SPEED);
  middleF.attach(pins[2], 0, SERVO_SPEED);
  ringF.attach(pins[3], 0, SERVO_SPEED);
  pinkF.attach(pins[4], 0, SERVO_SPEED);

  // Bereken minimale tijd voor een volledige beweging (0° → 180°)
  interval1 = (180 * SERVO_SPEED) + 500;  
  interval2 = (180 * SERVO_SPEED) + 900;
  interval3 = (180 * SERVO_SPEED) + 500;
  interval4 = (180 * SERVO_SPEED) + 800;
  interval5 = (180 * SERVO_SPEED) + 700;

  if (!BLE.begin()) {
    Serial.println("❌ BLE ERROR");
    while (1);
  }

  BLE.setLocalName("ExoHand-Rehab");
  BLE.setAdvertisedService(servoService);
  servoService.addCharacteristic(commandCharacteristic);
  BLE.addService(servoService);
  BLE.advertise();

  commandCharacteristic.writeValue("0");
  Serial.println("✅ Exoskelet klaar voor revalidatieoefeningen!");
}

// ===============================================================
// ============================ LOOP ===============================
// ===============================================================

void loop() {
  // Altijd servo's updaten
  thumb.update();
  indexF.update();
  middleF.update();
  ringF.update();
  pinkF.update();

  BLEDevice central = BLE.central();

  if (central && central.connected()) {

    if (commandCharacteristic.written()) {
      String cmd = commandCharacteristic.value();
      handleCommand(cmd);
    }

    if (running) runPatternLogic();
    else stopAllServos();
  }
}

// ===============================================================
// ======================== COMMAND HANDLING ======================
// ===============================================================

void handleCommand(String cmd) {
  if (cmd == "0") {
    running = false;
    pattern = 0;
    Serial.println("⏹ STOP");
    return;
  }

  pattern = cmd.toInt();
  running = true;
  Serial.print("▶️ Nieuw revalidatie patroon: ");
  Serial.println(pattern);
}

// ===============================================================
// ======================== PATTERNS ==============================
// ===============================================================

void runPatternLogic() {
  unsigned long now = millis();

  // ===============================================================
  // PATTERN 1 – Volledige vuist herhalen (alle vingers samen)
  // ===============================================================
  if (pattern == 1 && now - lastToggle >= interval1) {
    toggleState = !toggleState;
    lastToggle = now;

    int pos = toggleState ? 180 : 0; // dicht/open

    thumb.setTarget(pos);
    indexF.setTarget(pos);
    middleF.setTarget(pos);
    ringF.setTarget(pos);
    pinkF.setTarget(pos);
  }

  // ===============================================================
  // PATTERN 2 – Één vinger per keer sluiten en openen
  // ===============================================================
  else if (pattern == 2) {
    static int step = 0;

    if (now - lastToggle >= interval2) {
      lastToggle = now;
      step++;

      // Reset cycle
      if (step > 10) step = 0;

      // 0–5 = sluiten, 6–10 = openen
      bool closing = (step < 5);

      int pos = closing ? 180 : 0;

      if (step % 5 == 0) thumb.setTarget(pos);
      else if (step % 5 == 1) indexF.setTarget(pos);
      else if (step % 5 == 2) middleF.setTarget(pos);
      else if (step % 5 == 3) ringF.setTarget(pos);
      else if (step % 5 == 4) pinkF.setTarget(pos);
    }
  }

  // ===============================================================
  // PATTERN 3 – Pinch grip (duim + wijsvinger)
  // ===============================================================
  else if (pattern == 3 && now - lastToggle >= interval3) {
    toggleState = !toggleState;
    lastToggle = now;

    int pinchPos = toggleState ? 180 : 0;

    thumb.setTarget(pinchPos);
    indexF.setTarget(pinchPos);

    // Andere vingers blijven open
    middleF.setTarget(0);
    ringF.setTarget(0);
    pinkF.setTarget(0);
  }

  // ===============================================================
  // PATTERN 4 – Flexie-golf door de hand (duim → pink)
  // ===============================================================
  else if (pattern == 4) {
    static int step = 0;

    if (now - lastToggle >= interval4) {
      lastToggle = now;
      step++;

      if (step > 10) step = 0;

      bool closing = (step < 5);
      int pos = closing ? 180 : 0;

      if (step % 5 == 0) thumb.setTarget(pos);
      else if (step % 5 == 1) indexF.setTarget(pos);
      else if (step % 5 == 2) middleF.setTarget(pos);
      else if (step % 5 == 3) ringF.setTarget(pos);
      else if (step % 5 == 4) pinkF.setTarget(pos);
    }
  }

  // ===============================================================
  // PATTERN 5 – Vuist zonder duim
  // ===============================================================
  else if (pattern == 5 && now - lastToggle >= interval5) {
    toggleState = !toggleState;
    lastToggle = now;

    int pos = toggleState ? 180 : 0;

    indexF.setTarget(pos);
    middleF.setTarget(pos);
    ringF.setTarget(pos);
    pinkF.setTarget(pos);

    // Duim blijft open
    thumb.setTarget(0);
  }
}

// ===============================================================
// ======================== STOP FUNCTIE ==========================
// ===============================================================

void stopAllServos() {
  thumb.setTarget(0);
  indexF.setTarget(0);
  middleF.setTarget(0);
  ringF.setTarget(0);
  pinkF.setTarget(0);
}
