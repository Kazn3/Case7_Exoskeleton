#include <ArduinoBLE.h>
#include <Servo.h>

// ==========================================
// === INSTELLINGEN ===
// ==========================================

// 5–50 ms per graad
const int SERVO_SPEED = 15;

// ==========================================
// === SOFT SERVO ENGINE ===
// ==========================================

struct SmoothServo {
  Servo servo;
  int currentPos;
  int targetPos;
  unsigned long nextStepTime;
  int stepDelay;

  void attach(int pin, int start, int speed) {
    servo.attach(pin);
    currentPos = start;
    targetPos = start;
    stepDelay = speed;
    nextStepTime = millis();
    servo.write(start);
  }

  void update() {
    if (currentPos == targetPos) return;

    unsigned long now = millis();
    if (now >= nextStepTime) {
      nextStepTime = now + stepDelay;

      if (currentPos < targetPos) currentPos++;
      else currentPos--;

      servo.write(currentPos);
    }
  }

  void setTarget(int pos) {
    targetPos = constrain(pos, 0, 180);
  }
};

// ==========================================
// === SERVOS ===
// ==========================================

SmoothServo s1, s2, s3, s4, s5;
const int servoPins[] = {3, 5, 6, 9, 10};

// ==========================================
// === PATTERNS ===
// ==========================================

int pattern = 0;
bool running = false;
unsigned long lastToggle = 0;
bool toggled = false;

// Automatisch berekende intervals (ALTIJD 180°)
unsigned long interval1; 
unsigned long interval2;
unsigned long interval3;
unsigned long interval4;
unsigned long interval5;

// ==========================================
// === BUTTON ===
// ==========================================

const int buttonPin = 2;
bool lastButtonState = HIGH;
unsigned long lastDebounce = 0;
const int debounceDelay = 50;

// ==========================================
// === BLE ===
// ==========================================

BLEService svc("180A");
BLEStringCharacteristic cmdChar("2A57", BLEWrite, 20);

// ==========================================
// === SETUP ===
// ==========================================

void setup() {
  Serial.begin(115200);

  // Servo setup
  s1.attach(servoPins[0], 0, SERVO_SPEED);
  s2.attach(servoPins[1], 0, SERVO_SPEED);
  s3.attach(servoPins[2], 0, SERVO_SPEED);
  s4.attach(servoPins[3], 0, SERVO_SPEED);
  s5.attach(servoPins[4], 0, SERVO_SPEED);

  // Interval = volledige slag (180°) + pauze
  unsigned long moveTime = 180UL * SERVO_SPEED;

  interval1 = moveTime + 500;
  interval2 = moveTime + 2000;
  interval3 = moveTime + 500;
  interval4 = moveTime + 500;
  interval5 = moveTime + 200;

  pinMode(buttonPin, INPUT_PULLUP);

  // BLE init
  if (!BLE.begin()) {
    Serial.println("BLE FAILED");
    while (1);
  }

  BLE.setLocalName("NanoIOT-ServoCtrl");
  BLE.setAdvertisedService(svc);
  svc.addCharacteristic(cmdChar);
  BLE.addService(svc);
  BLE.advertise();

  Serial.println("READY");
}

// ==========================================
// === LOOP ===
// ==========================================

void loop() {
  s1.update(); s2.update(); s3.update(); s4.update(); s5.update();

  BLEDevice central = BLE.central();

  if (central && central.connected()) {
    if (cmdChar.written()) handleCommand(cmdChar.value());
    checkButton();

    if (running) runPatterns();
    else stopAll();
  }
}

// ==========================================
// === COMMAND HANDLING ===
// ==========================================

void handleCommand(String cmd) {
  if (cmd == "0") {
    running = false;
    pattern = 0;
  } else {
    pattern = cmd.toInt();
    running = true;
    lastToggle = 0;
  }
}

// ==========================================
// === BUTTON ===
// ==========================================

void checkButton() {
  bool reading = digitalRead(buttonPin);
  if (reading != lastButtonState) lastDebounce = millis();

  if ((millis() - lastDebounce) > debounceDelay) {
    if (reading == LOW && lastButtonState == HIGH) {
      running = !running;
      lastToggle = 0;
    }
  }
  lastButtonState = reading;
}

// ==========================================
// === PATTERNS ===
// ==========================================

void runPatterns() {
  unsigned long now = millis();

  switch (pattern) {

    case 1:
      if (now - lastToggle >= interval1) {
        lastToggle = now;
        toggled = !toggled;
        int pos = toggled ? 180 : 0;
        s1.setTarget(pos);
        s2.setTarget(pos);
      }
      break;

    case 2:
      if (now - lastToggle >= interval2) {
        lastToggle = now;
        toggled = !toggled;
        int pos = toggled ? 180 : 0;
        s3.setTarget(pos);
        s4.setTarget(pos);
      }
      break;

    case 3:
      if (now - lastToggle >= interval3) {
        lastToggle = now;
        toggled = !toggled;
        int pos = toggled ? 180 : 0;
        s1.setTarget(pos);
        s5.setTarget(pos);
      }
      break;

    case 4:
      if (now - lastToggle >= interval4) {
        lastToggle = now;
        toggled = !toggled;
        int pos = toggled ? 180 : 0;
        s1.setTarget(pos);
        s2.setTarget(pos);
        s3.setTarget(pos);
        s4.setTarget(pos);
        s5.setTarget(pos);
      }
      break;

    case 5:
      if (now - lastToggle >= interval5) {
        lastToggle = now;
        toggled = !toggled;
        int A = toggled ? 180 : 0;
        int B = toggled ? 0 : 180;
        s1.setTarget(A);
        s3.setTarget(A);
        s5.setTarget(A);
        s2.setTarget(B);
        s4.setTarget(B);
      }
      break;
  }
}

// ==========================================
// === STOP ALL ===
// ==========================================

void stopAll() {
  s1.setTarget(0);
  s2.setTarget(0);
  s3.setTarget(0);
  s4.setTarget(0);
  s5.setTarget(0);
}
