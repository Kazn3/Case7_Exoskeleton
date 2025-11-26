# Arduino BLE Servo Controller

Deze Arduino-code stuurt **5 servo motoren** aan via **Bluetooth Low Energy (BLE)** en een **fysieke drukknop**.  
De servo’s bewegen **vloeiend zonder `delay()`**, zodat je animaties soepel blijven lopen terwijl BLE en input gewoon blijven werken.

Ideaal voor interactieve installaties, kinetische sculpturen of Arduino-gestuurde prototypes met meerdere servo’s.

---

## Features

✅ 5 servo’s met soepele beweging  
✅ Aansturing via Bluetooth (BLE)  
✅ Start / Stop knop op hardware  
✅ 5 verschillende bewegingspatronen  
✅ Geen `delay()` → geen blokkering  
✅ Dynamische timing gebaseerd op servo snelheid  

---

## Benodigde hardware

- Arduino Nano 33 IoT  
- 5x Servo motoren  
- Drukknop  
- Externe 5–6V voeding voor servo’s  
- Verbindingen / breadboard

Pin mapping:
| Servo | Pin |
|------|-----|
| s1 | 3 |
| s2 | 5 |
| s3 | 6 |
| s4 | 9 |
| s5 | 10 |
| Knop | 2 |

---

## BLE werking

De Arduino biedt een BLE service aan:
- **Service UUID**: `180A`
- **Characteristic UUID**: `2A57`

Je kan via BLE een getal sturen:
| Waarde | Betekenis |
|-------|-----------|
| `0` | Stop alles |
| `1`–`5` | Activeer een patroon |

---

## Fysieke knop

De knop werkt als **start/stop schakelaar**.
Elke keer dat je klikt:
- Start als het systeem stilstaat
- Stopt als het systeem loopt

---

## 🎛 Servobeweging

Elke servo gebruikt een **SmoothServo-struct** die zorgt voor vloeiende beweging in plaats van abrupt bewegen.

De snelheid wordt bepaald via:

```cpp
const int SERVO_SNELHEID = 15;
````

Hoe lager dit getal, hoe sneller de servo beweegt.

---

## Beschikbare patronen

| Patroon | Beschrijving                                |
| ------- | ------------------------------------------- |
| 1       | Servo 1 & 2 bewegen samen tussen 0° en 180° |
| 2       | Servo 3 & 4 langzaam tussen 0° en 180°      |
| 3       | Servo 1 & 5 bewegen tussen 30° en 150°      |
| 4       | Alle servo's bewegen tussen 45° en 135°     |
| 5       | Golvende beweging: 1,3,5 tegenover 2,4      |

---

## De volledige Arduino code

```cpp
#include <ArduinoBLE.h>
#include <Servo.h>

// ==========================================
// === INSTELLINGEN VOOR SNELHEID ===
// ==========================================
const int SERVO_SNELHEID = 15;

// ==========================================

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

// === Servo Objecten ===
SmoothServo s1, s2, s3, s4, s5;
const int servoPins[] = {3, 5, 6, 9, 10};

// === Patroon instellingen ===
int pattern = 0;
unsigned long lastToggle = 0;
bool toggleState = false;

// Dynamische intervallen
unsigned long intervalPattern1;
unsigned long intervalPattern2;
unsigned long intervalPattern3;
unsigned long intervalPattern4;
unsigned long intervalPattern5;

// Knop
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

  s1.attach(servoPins[0], 0, SERVO_SNELHEID);
  s2.attach(servoPins[1], 0, SERVO_SNELHEID);
  s3.attach(servoPins[2], 0, SERVO_SNELHEID);
  s4.attach(servoPins[3], 0, SERVO_SNELHEID);
  s5.attach(servoPins[4], 0, SERVO_SNELHEID);

  intervalPattern1 = (180 * SERVO_SNELHEID) + 500;
  intervalPattern2 = (180 * SERVO_SNELHEID) + 2000;
  intervalPattern3 = (120 * SERVO_SNELHEID) + 500;
  intervalPattern4 = (90 * SERVO_SNELHEID) + 500;
  intervalPattern5 = (180 * SERVO_SNELHEID) + 200;

  pinMode(buttonPin, INPUT_PULLUP);

  if (!BLE.begin()) {
    Serial.println("BLE start mislukt");
    while (1);
  }

  BLE.setLocalName("NanoIOT-ServoCtrl");
  BLE.setAdvertisedService(servoService);
  servoService.addCharacteristic(commandCharacteristic);
  BLE.addService(servoService);

  commandCharacteristic.writeValue("0");
  BLE.advertise();
}

void loop() {
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

    if (running) {
      runPatternLogic();
    } else {
      stopAllServos();
    }
  }
}

void handleCommand(String cmd) {
  if (cmd == "0") {
    pattern = 0;
    running = false;
  } else {
    pattern = cmd.toInt();
  }
}

void checkButton() {
  bool reading = !digitalRead(buttonPin);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState) running = !running;
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
    s1.setTarget(pos);
    s2.setTarget(pos);
  }

  else if (pattern == 3 && currentMillis - lastToggle >= intervalPattern3) {
    toggleState = !toggleState;
    lastToggle = currentMillis;
    int pos = toggleState ? 30 : 150;
    s1.setTarget(pos);
    s5.setTarget(pos);
  }
}

void stopAllServos() {
  s1.setTarget(0);
  s2.setTarget(0);
  s3.setTarget(0);
  s4.setTarget(0);
  s5.setTarget(0);
}
```

---

## Mogelijke uitbreidingen

* Web interface via BLE
* Extra patronen
* Potentiometer voor snelheid
* Startpositie per servo opslaan

---



Laat me weten als je ook een **Engelse versie**, **diagram** of **extra visuals** wil 👌
