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

int pattern = 0;
bool running = false;
unsigned long lastToggle = 0;
bool toggleState = false;
unsigned long interval1, interval2, interval3, interval4, interval5;
bool wasConnected = false;

BLEService exoService("12345678-1234-5678-1234-56789abcdef0");
BLECharacteristic commandChar("abcdef01-1234-5678-1234-56789abcdef0", BLEWrite | BLEWriteWithoutResponse, 1);

void onWrite(BLEDevice central, BLECharacteristic characteristic) {
  uint8_t value = characteristic.value()[0];
  if (value >= 1 && value <= 5) {
    pattern = value;
    running = true;
    toggleState = false;
    lastToggle = millis();
  } else if (value == 0) {
    stopAllServos();
  }
}

void setup() {
  // 1. VERWIJDER: while(!Serial). De Arduino start nu direct op zonder USB.
  Serial.begin(115200);
  
  // Status LED configureren
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  // 2. VERTRAGING: Geef de batterijspanning 2 seconden om te stabiliseren
  delay(2000); 

  // 3. SEQUENTIËLE START: Start servo's één voor één met rustpauzes
  // Dit voorkomt een enorme stroompiek die de BLE-chip doet crashen.
  thumb.attach(pins[0], 0, SERVO_SPEED);    delay(150);
  indexF.attach(pins[1], 0, SERVO_SPEED);   delay(150);
  middleF.attach(pins[2], 0, SERVO_SPEED);  delay(150);
  ringF.attach(pins[3], 0, SERVO_SPEED);    delay(150);
  pinkF.attach(pins[4], 0, SERVO_SPEED);    delay(150);

  stopAllServos(); 

  interval1 = (180 * SERVO_SPEED) + 500;
  interval2 = (180 * SERVO_SPEED) + 900;
  interval3 = (180 * SERVO_SPEED) + 500;
  interval4 = (180 * SERVO_SPEED) + 800;
  interval5 = (180 * SERVO_SPEED) + 700;

  if (!BLE.begin()) {
    while (1) { // Snel knipperen bij fout
      digitalWrite(LED_BUILTIN, HIGH); delay(100);
      digitalWrite(LED_BUILTIN, LOW);  delay(100);
    }
  }

  BLE.setLocalName("ExoHand-WebBLE");
  BLE.setAdvertisedService(exoService);
  exoService.addCharacteristic(commandChar);
  BLE.addService(exoService);
  commandChar.setEventHandler(BLEWritten, onWrite);
  commandChar.writeValue((byte)0);
  BLE.advertise();

  // LED constant aan = Klaar voor verbinding
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
    stopAllServos();
    pattern = 0;
    // LED uit tijdens verbinding (bespaart beetje stroom)
    digitalWrite(LED_BUILTIN, LOW); 
  }

  if (!isConnected && wasConnected) {
    stopAllServos();
    pattern = 0;
    digitalWrite(LED_BUILTIN, HIGH); // LED weer aan: wacht op nieuwe verbinding
  }

  wasConnected = isConnected;
  if (running) runPatternLogic();
}

// ... de rest van je runPatternLogic en stopAllServos functies blijven exact gelijk ...

void runPatternLogic() {
  unsigned long now = millis();
  if (pattern == 1 && now - lastToggle >= interval1) {
    toggleState = !toggleState;
    lastToggle = now;
    int pos = toggleState ? 180 : 0;
    thumb.setTarget(pos); indexF.setTarget(pos); middleF.setTarget(pos); ringF.setTarget(pos); pinkF.setTarget(pos);
  }
  else if (pattern == 2) {
    static int step = 0;
    if (now - lastToggle >= interval2) {
      lastToggle = now;
      step = (step + 1) % 10;
      bool closing = step < 5;
      int pos = closing ? 180 : 0;
      if (step % 5 == 0) thumb.setTarget(pos);
      else if (step % 5 == 1) indexF.setTarget(pos);
      else if (step % 5 == 2) middleF.setTarget(pos);
      else if (step % 5 == 3) ringF.setTarget(pos);
      else pinkF.setTarget(pos);
    }
  }
  else if (pattern == 3 && now - lastToggle >= interval3) {
    toggleState = !toggleState;
    lastToggle = now;
    int pos = toggleState ? 180 : 0;
    thumb.setTarget(pos); indexF.setTarget(pos); middleF.setTarget(0); ringF.setTarget(0); pinkF.setTarget(0);
  }
  else if (pattern == 4) {
    static int step = 0;
    if (now - lastToggle >= interval4) {
      lastToggle = now;
      step = (step + 1) % 10;
      bool closing = step < 5;
      int pos = closing ? 180 : 0;
      if (step % 5 == 0) thumb.setTarget(pos);
      else if (step % 5 == 1) indexF.setTarget(pos);
      else if (step % 5 == 2) middleF.setTarget(pos);
      else if (step % 5 == 3) ringF.setTarget(pos);
      else pinkF.setTarget(pos);
    }
  }
  else if (pattern == 5 && now - lastToggle >= interval5) {
    toggleState = !toggleState;
    lastToggle = now;
    int pos = toggleState ? 180 : 0;
    indexF.setTarget(pos); middleF.setTarget(pos); ringF.setTarget(pos); pinkF.setTarget(pos); thumb.setTarget(0);
  }
}

void stopAllServos() {
  thumb.setTarget(0); indexF.setTarget(0); middleF.setTarget(0); ringF.setTarget(0); pinkF.setTarget(0);
  running = false;
}