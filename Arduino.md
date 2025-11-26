# Arduino BLE Servo Controller

Deze repository bevat Arduino-code voor het aansturen van **5 servo’s via Bluetooth Low Energy (BLE)** en een **fysieke knop**.  
De servo’s bewegen vloeiend met een niet-blokkerende beweging (zonder `delay()`), zodat meerdere acties tegelijk kunnen blijven werken.

---

## 1. Functionaliteit

De code laat toe om:

- Vijf servo’s afzonderlijk aan te sturen
- Vloeiende bewegingen te maken zonder `delay()`
- Patronen te activeren via BLE
- De animatie te starten en stoppen via een fysieke knop

---

## 2. Benodigdheden

Hardware:
- Arduino (bv. Nano 33 IoT)
- 5 servo motoren
- Drukknop
- Weerstand (10kΩ voor pull-down of pull-up)
- Externe voeding voor servo’s

Software:
- Arduino IDE
- ArduinoBLE library
- Servo library

---

## 3. Libraries

```cpp
#include <ArduinoBLE.h>
#include <Servo.h>
Uitleg
ArduinoBLE verzorgt de Bluetooth communicatie.

Servo maakt het mogelijk om servo motoren aan te sturen.

4. SmoothServo Struct
Deze struct zorgt voor vloeiende servo bewegingen zonder delays.

cpp
Copy code
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

    unsigned long now = millis();
    if (now - lastMoveTime >= stepDelay) {
      lastMoveTime = now;

      if (currentPos < targetPos) currentPos++;
      else if (currentPos > targetPos) currentPos--;

      servo.write(currentPos);
    }
  }

  void setTarget(int pos) {
    targetPos = pos;
  }

  void stopImmediate(int pos) {
    currentPos = pos;
    targetPos = pos;
    servo.write(pos);
  }
};
Functies uitgelegd
Functie	Beschrijving
attach(pin, startPos, speed)	Verbindt de servo met een pin en zet de beginpositie en snelheid
update()	Beweegt de servo stap voor stap richting de doelpositie
setTarget(pos)	Stelt een nieuwe doelpositie in
stopImmediate(pos)	Zet de servo onmiddellijk op een positie

5. Servo Configuratie
cpp
Copy code
SmoothServo s1, s2, s3, s4, s5;

const int servoPins[] = {3, 5, 6, 9, 10};
const int SERVO_SNELHEID = 15;
Uitleg
Er worden vijf servo-objecten aangemaakt.

SERVO_SNELHEID bepaalt hoe snel ze bewegen (lager = sneller).

6. Knop Logica
cpp
Copy code
const int buttonPin = 2;
bool running = false;
Uitleg
Met de knop kan je de volledige animatie starten en stoppen.

7. BLE Configuratie
cpp
Copy code
BLEService servoService("180A");
BLEStringCharacteristic commandCharacteristic(
  "2A57", 
  BLERead | BLEWrite, 
  20
);
Uitleg
Via BLE kan je commando’s sturen (zoals "pattern1", "stop", …).
De Arduino luistert naar deze berichten en past de bewegingen aan.

8. Setup Functie
cpp
Copy code
void setup() {
  Serial.begin(115200);

  pinMode(buttonPin, INPUT_PULLUP);

  s1.attach(servoPins[0], 90, SERVO_SNELHEID);
  s2.attach(servoPins[1], 90, SERVO_SNELHEID);
  s3.attach(servoPins[2], 90, SERVO_SNELHEID);
  s4.attach(servoPins[3], 90, SERVO_SNELHEID);
  s5.attach(servoPins[4], 90, SERVO_SNELHEID);

  if (!BLE.begin()) {
    Serial.println("BLE failed to start");
    while (1);
  }

  BLE.setLocalName("ServoController");
  BLE.setAdvertisedService(servoService);

  servoService.addCharacteristic(commandCharacteristic);
  BLE.addService(servoService);

  commandCharacteristic.writeValue("ready");

  BLE.advertise();
}
9. Loop Functie
cpp
Copy code
void loop() {
  BLEDevice central = BLE.central();

  if (central) {
    while (central.connected()) {
      
      if (commandCharacteristic.written()) {
        String command = commandCharacteristic.value();
        handleCommand(command);
      }

      if (digitalRead(buttonPin) == LOW) {
        running = !running;
        delay(300);
      }

      if (running) {
        runPattern();
      }

      s1.update();
      s2.update();
      s3.update();
      s4.update();
      s5.update();
    }
  }
}
Wat gebeurt hier?
De Arduino checkt of er een BLE toestel verbonden is.

Leest Bluetooth-commando’s.

Leest de drukknop.

Laat servo’s vloeiend bewegen met update().

10. Command Verwerking
cpp
Copy code
void handleCommand(String cmd) {
  if (cmd == "pattern1") {
    pattern = 1;
  }
  else if (cmd == "stop") {
    running = false;
  }
}
Hier kan je extra patronen en commando’s toevoegen.

11. Patronen
cpp
Copy code
void runPattern() {
  switch (pattern) {
    case 1:
      s1.setTarget(20);
      s2.setTarget(160);
      break;

    case 2:
      s3.setTarget(40);
      s4.setTarget(120);
      break;
  }
}
Je kan hier eigen sequenties maken voor je installaties of prototypes.

12. Toekomstige uitbreidingen
Mogelijke upgrades:

Meer servo’s

Complexere animatiepatronen

App-interface

Sensor input

