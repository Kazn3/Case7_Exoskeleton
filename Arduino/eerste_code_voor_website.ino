#include <ArduinoBLE.h>
#include <Servo.h>

// ===============================================================
// ========== INSTELBARE SNELHEID VAN ALLE SERVO'S ===============
// ===============================================================
const int SERVO_SPEED = 15; // ms per graad – vloeiend

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
SmoothServo thumb, indexF, middleF, ringF, pinkF;
int pins[] = {3,5,6,9,10};

// ===============================================================
// ========== PATTERN VARIABELEN =================================
// ===============================================================
int pattern = 0;
bool running = false;
unsigned long lastToggle = 0;
bool toggleState = false;

unsigned long interval1, interval2, interval3, interval4, interval5;

// Voor Web Bluetooth commando
int currentMovement = 0;
bool movings = false;

// ===============================================================
// ========== BLE DEFINITIES (WEB BLUETOOTH READY) ================
// ===============================================================
BLEService exoService("12345678-1234-5678-1234-56789abcdef0");

// Characteristic: 1 byte voor commando's 1–5
BLECharacteristic commandChar(
  "abcdef01-1234-5678-1234-56789abcdef0",
  BLEWrite | BLEWriteWithoutResponse,
  1
);

// ===============================================================
// ========================= BLE CALLBACK ========================
// ===============================================================
void onWrite(BLEDevice central, BLECharacteristic characteristic) {
  uint8_t value = characteristic.value()[0];
  Serial.print("Ontvangen waarde (byte): ");
  Serial.println(value);

  if(value >= 1 && value <= 5){
    currentMovement = value;
    running = true;
    pattern = currentMovement;
    movings = false;
    Serial.print("Movement set to pattern: ");
    Serial.println(currentMovement);
  } 
  else if(value == 0){
    // Stop de oefening en zet servos terug naar startpositie
    Serial.println("Stopping all movements. Resetting servos...");
    running = false;
    pattern = 0;
    stopAllServos();  // zorgt ervoor dat alle servos terug naar 0 gaan
  }
  else {
    Serial.println("Ongeldige waarde ontvangen.");
  }
}

// ===============================================================
// ========================= SETUP ================================
// ===============================================================
void setup() {
  Serial.begin(115200);
  while(!Serial);

  // Servo setup
  thumb.attach(pins[0], 0, SERVO_SPEED);
  indexF.attach(pins[1], 0, SERVO_SPEED);
  middleF.attach(pins[2], 0, SERVO_SPEED);
  ringF.attach(pins[3], 0, SERVO_SPEED);
  pinkF.attach(pins[4], 0, SERVO_SPEED);

  // Interval berekeningen
  interval1 = (180 * SERVO_SPEED) + 500;
  interval2 = (180 * SERVO_SPEED) + 900;
  interval3 = (180 * SERVO_SPEED) + 500;
  interval4 = (180 * SERVO_SPEED) + 800;
  interval5 = (180 * SERVO_SPEED) + 700;

  // BLE setup
  if(!BLE.begin()){
    Serial.println("❌ BLE ERROR");
    while(1);
  }

  BLE.setLocalName("ExoHand-WebBLE");
  BLE.setAdvertisedService(exoService);

  exoService.addCharacteristic(commandChar);
  BLE.addService(exoService);

  // Callback koppelen
  commandChar.setEventHandler(BLEWritten, onWrite);

  commandChar.writeValue((byte)0);

  BLE.advertise();
  Serial.println("✅ BLE actief en wacht op verbinding…");
}

// ===============================================================
// ============================ LOOP ===============================
// ===============================================================
void loop() {
  // Servo updates
  thumb.update();
  indexF.update();
  middleF.update();
  ringF.update();
  pinkF.update();

  // BLE handling
  BLEDevice central = BLE.central();
  if(central && central.connected()){
    // beweging gebeurt via callback, pattern wordt geüpdatet
  }

  if(running) runPatternLogic();
  else stopAllServos();
}

// ===============================================================
// ======================== PATTERNS ==============================
// ===============================================================
void runPatternLogic() {
  unsigned long now = millis();

  // PATTERN 1 – Volledige vuist
  if(pattern == 1 && now - lastToggle >= interval1){
    toggleState = !toggleState;
    lastToggle = now;
    int pos = toggleState ? 180 : 0;
    thumb.setTarget(pos);
    indexF.setTarget(pos);
    middleF.setTarget(pos);
    ringF.setTarget(pos);
    pinkF.setTarget(pos);
  }
  // PATTERN 2 – Vinger per vinger
  else if(pattern == 2){
    static int step = 0;
    if(now - lastToggle >= interval2){
      lastToggle = now;
      step++;
      if(step > 10) step = 0;
      bool closing = (step < 5);
      int pos = closing ? 180 : 0;
      if(step % 5 == 0) thumb.setTarget(pos);
      else if(step % 5 == 1) indexF.setTarget(pos);
      else if(step % 5 == 2) middleF.setTarget(pos);
      else if(step % 5 == 3) ringF.setTarget(pos);
      else if(step % 5 == 4) pinkF.setTarget(pos);
    }
  }
  // PATTERN 3 – Pinch grip
  else if(pattern == 3 && now - lastToggle >= interval3){
    toggleState = !toggleState;
    lastToggle = now;
    int pinchPos = toggleState ? 180 : 0;
    thumb.setTarget(pinchPos);
    indexF.setTarget(pinchPos);
    middleF.setTarget(0);
    ringF.setTarget(0);
    pinkF.setTarget(0);
  }
  // PATTERN 4 – Flexiegolf
  else if(pattern == 4){
    static int step = 0;
    if(now - lastToggle >= interval4){
      lastToggle = now;
      step++;
      if(step > 10) step = 0;
      bool closing = (step < 5);
      int pos = closing ? 180 : 0;
      if(step % 5 == 0) thumb.setTarget(pos);
      else if(step % 5 == 1) indexF.setTarget(pos);
      else if(step % 5 == 2) middleF.setTarget(pos);
      else if(step % 5 == 3) ringF.setTarget(pos);
      else if(step % 5 == 4) pinkF.setTarget(pos);
    }
  }
  // PATTERN 5 – Vuist zonder duim
  else if(pattern == 5 && now - lastToggle >= interval5){
    toggleState = !toggleState;
    lastToggle = now;
    int pos = toggleState ? 180 : 0;
    indexF.setTarget(pos);
    middleF.setTarget(pos);
    ringF.setTarget(pos);
    pinkF.setTarget(pos);
    thumb.setTarget(0);
  }
}

// ===============================================================
// ======================== STOP FUNCTIE ==========================
// ===============================================================
void stopAllServos(){
  thumb.setTarget(0);
  indexF.setTarget(0);
  middleF.setTarget(0);
  ringF.setTarget(0);
  pinkF.setTarget(0);
  running = false;
}                                                                      