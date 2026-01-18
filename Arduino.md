# Arduino BLE Servo Controller

Deze Arduino-code stuurt **5 servo motoren** aan met een **website** verbonden via **Bluetooth Low Energy (BLE)**.  
De servo’s bewegen **vloeiend zonder `delay()`**, zodat je animaties soepel blijven lopen terwijl BLE en input gewoon blijven werken.

Ideaal voor interactieve installaties, kinetische sculpturen of Arduino-gestuurde prototypes met meerdere servo’s.

---

## Features

- 5 servo’s met soepele beweging  
- Aansturing met website verbonden via Bluetooth (BLE)  
- 5 verschillende bewegingspatronen  
- Geen `delay()` → geen blokkering  

---

## Benodigde hardware

- Arduino Nano 33 IoT  
- 5x Servo motoren  
- Externe 5–6V voeding voor servo’s
- Externe 5V voeding voor arduino
- Verbindingen

Pin mapping:
| Servo | Pin | Vinger |
|------|-----|-----|
| s1 | 3 |Duim|
| s2 | 5 |Wijsvinger|
| s3 | 6 |Middelvinger|
| s4 | 9 |Ringvinger|
| s5 | 10 |Pink|

---

## Servobeweging

Elke servo gebruikt een **SmoothServo-struct** die zorgt voor vloeiende beweging in plaats van abrupt bewegen.

De snelheid wordt bepaald via:

```cpp
const int SERVO_SNELHEID = 15;
````

Hoe lager dit getal, hoe sneller de servo beweegt.

De servo’s ontvangen hun INPUT via de website, waar zowel het type bewegingspatroon als de duur van de uitvoering wordt ingesteld.

---

## Beschikbare patronen

| Patroon | Naam|Beschrijving                                            |
| ------- |-----------------|------------------------------------------- |
| 1       |Volledige vuist  |Alle servo's bewegen tegelijk herhalend van 0° naar 180° en van 180° naar 0°|
| 2       |Vinger per vinger|Servo's bewegen één voor één van 0° naar 180° en van 180° naar 0°|
| 3       |Pinch grip       |Servo's 1 en 2 bewegen herhalend van 0° naar 180° en van 180° naar 0° |
| 4       |Flexiegolf       |Servo's sluiten en openen in een golf beweging |
| 5       |Vuist zonder duim|Alle servo's bewegen herhalend van 0° naar 180° en van 180° naar 0° buiten servo1|

---

## De volledige Arduino code

```cpp
#include <ArduinoBLE.h> // Bibliotheek voor Bluetooth Low Energy
#include <Servo.h>      // Bibliotheek voor het aansturen van de servo's

// --- CONFIGURATIE ---
const int SERVO_SPEED = 10; // aantal ms per stap --> snelheid van de servo

// Deze 'struct' bundelt alle eigenschappen van een vinger-servo
struct SmoothServo {
  Servo servo;          
  int currentPos;       // De huidige positie van de vinger (0-180)
  int targetPos;        // Waar de vinger naartoe moet
  unsigned long lastMoveTime; 
  int stepDelay;        // Snelheid van de beweging
  bool reversed = false; // Bepaalt of de motor fysiek omgekeerd gemonteerd is

  // Initialisatie van de vinger
  void attach(int pin, int startPos, int speed, bool isReversed = false) {
    servo.attach(pin);
    reversed = isReversed; 
    currentPos = startPos;
    targetPos = startPos;
    stepDelay = speed;

    // Als de motor omgekeerd zit, moeten we 180 - positie sturen
    int physicalPos = reversed ? (180 - currentPos) : currentPos;
    servo.write(physicalPos);
  }

  // Update-functie: zorgt voor de soepele beweging (geen delay() nodig!)
  void update() {
    if (currentPos == targetPos) return; // Al op de plek? Doe niets.
    
    // Controleer of het tijd is voor de volgende stap (graad)
    if (millis() - lastMoveTime >= stepDelay) {
      lastMoveTime = millis();
      if (currentPos < targetPos) currentPos++; // Beweeg richting doel
      else currentPos--;
      
      // Stuur de gecorrigeerde positie naar de fysieke motor
      int physicalPos = reversed ? (180 - currentPos) : currentPos;
      servo.write(physicalPos); 
    }
  }

  // Stel een nieuw doel in voor deze vinger
  void setTarget(int pos) {
    targetPos = pos;
  }
};

// --- OBJECTEN EN VARIABELEN ---
SmoothServo thumb, indexF, middleF, ringF, pinkF; // De 5 vingers
int pins[] = {3, 5, 6, 9, 10}; // De digitale pinnen op de Arduino

int pattern = 0;              // Welke oefening (1 t/m 5)
int durationMinutes = 0;      // Hoe lang de sessie duurt
bool running = false;         // Is de oefening nu bezig?
unsigned long sessionStartTime = 0; 
unsigned long lastToggle = 0; // Tijdstip van laatste beweging (open/dicht)
bool toggleState = false;     // Houdt bij of de hand open of dicht moet
bool wasConnected = false;    // Status van de Bluetooth verbinding

// Bluetooth instellingen (Service en Karakteristiek)
BLEService exoService("12345678-1234-5678-1234-56789abcdef0");
BLECharacteristic commandChar("abcdef01-1234-5678-1234-56789abcdef0", BLEWrite | BLEWriteWithoutResponse, 2);

// --- BLUETOOTH ONTVANGST ---
// Deze functie start als de Web App data stuurt (bijv. [1, 5] voor patroon 1, 5 seconden)
void onWrite(BLEDevice central, BLECharacteristic characteristic) {
  if (characteristic.valueLength() >= 2) {
    const uint8_t* data = characteristic.value(); 
    pattern = data[0];           // Eerste getal is het patroon
    durationMinutes = data[1];   // Tweede getal is de tijd

    Serial.println("------------------------------------");
    Serial.println("Nieuwe Revalidatie Sessie Ontvangen");
    
    // Controleer of de data geldig is
    if (pattern >= 1 && pattern <= 5 && durationMinutes > 0) {
      running = true;
      toggleState = false;
      lastToggle = millis();
      sessionStartTime = millis(); 
      Serial.println("Status: Sessie GESTART");
    } else {
      stopAllServos(); // Stop als patroon 0 of tijd 0 wordt gestuurd
    }
  }
}

void setup() {
  Serial.begin(115200); 
  delay(2000); 

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW); // LED uit = klaar voor verbinding

  // Koppel de vinger-objecten aan de juiste pinnen
  // Let op: indexF en middleF zijn omgekeerd (true) gemonteerd in dit voorbeeld
  thumb.attach(pins[0], 0, SERVO_SPEED, false);    delay(150);
  indexF.attach(pins[1], 0, SERVO_SPEED, true);     delay(150); 
  middleF.attach(pins[2], 0, SERVO_SPEED, true);    delay(150); 
  ringF.attach(pins[3], 0, SERVO_SPEED, false);    delay(150);
  pinkF.attach(pins[4], 0, SERVO_SPEED, false);    delay(150);

  stopAllServos(); // Begin in ruststand

  // Start Bluetooth
  if (!BLE.begin()) {
    while (1); // Stop als Bluetooth niet werkt
  }

  BLE.setLocalName("ExoRehab-WebBLE");
  BLE.setAdvertisedService(exoService);
  exoService.addCharacteristic(commandChar);
  BLE.addService(exoService);
  commandChar.setEventHandler(BLEWritten, onWrite); 
  BLE.advertise();

  digitalWrite(LED_BUILTIN, HIGH); // LED aan = aan het adverteren
}

void loop() {
  // Update constant de vingerposities (voor vloeiende beweging)
  thumb.update();
  indexF.update();
  middleF.update();
  ringF.update();
  pinkF.update();

  // Bluetooth status beheer
  BLEDevice central = BLE.central();
  bool isConnected = central && central.connected();

  if (isConnected && !wasConnected) {
    digitalWrite(LED_BUILTIN, LOW); // LED uit als verbonden
  }
  if (!isConnected && wasConnected) {
    stopAllServos(); // Veiligheid: stop alles als verbinding verbreekt
    digitalWrite(LED_BUILTIN, HIGH); 
  }
  wasConnected = isConnected;

  // Als de sessie loopt, voer het patroon uit
  if (running) {
    unsigned long sessionElapsed = millis() - sessionStartTime;
    unsigned long sessionLimit = (unsigned long)durationMinutes * 1000; // Omzetten naar ms

    if (sessionElapsed >= sessionLimit) {
      stopAllServos(); // Tijd is om
    } else {
      runPatternLogic(); // Voer de gekozen oefening uit
    }
  }
}

// --- OEFENING LOGICA ---
void runPatternLogic() {
  unsigned long now = millis();
  // Bereken hoe lang we moeten wachten tot de hand volledig open of dicht is
  int speedDelay = (180 * SERVO_SPEED) + 800; 

  switch (pattern) {
    case 1: // VOLLEDIGE VUIST (Alle vingers tegelijk)
      if (now - lastToggle >= speedDelay) {
        toggleState = !toggleState;
        lastToggle = now;
        setAllTargets(toggleState ? 180 : 0);
      }
      break;

    case 2: // VINGER-PER-VINGER (Eén voor één sluiten)
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
      }
      break;

    case 3: // PINCH GRIP (Alleen duim en wijsvinger)
      if (now - lastToggle >= speedDelay) {
        toggleState = !toggleState;
        lastToggle = now;
        int pos = toggleState ? 180 : 0;
        thumb.setTarget(pos);
        indexF.setTarget(pos);
        middleF.setTarget(0); ringF.setTarget(0); pinkF.setTarget(0);
      }
      break;

    case 4: // FLEXIEGOLF (Wave effect van pink naar duim)
      static int waveStep = 0;
      if (now - lastToggle >= 400) { 
        lastToggle = now;
        waveStep = (waveStep + 1) % 10;
        int pos = (waveStep < 5) ? 180 : 0;
        int v = waveStep % 5;
        if (v == 0) pinkF.setTarget(pos); 
        else if (v == 1) ringF.setTarget(pos);
        else if (v == 2) middleF.setTarget(pos);
        else if (v == 3) indexF.setTarget(pos);
        else thumb.setTarget(pos);
      }
      break;

    case 5: // VUIST ZONDER DUIM
      if (now - lastToggle >= speedDelay) {
        toggleState = !toggleState;
        lastToggle = now;
        int pos = toggleState ? 180 : 0;
        thumb.setTarget(0); 
        indexF.setTarget(pos); middleF.setTarget(pos); ringF.setTarget(pos); pinkF.setTarget(pos);
      }
      break;
  }
}

// Hulpmiddelen om targets te zetten
void setAllTargets(int pos) {
  thumb.setTarget(pos); indexF.setTarget(pos); middleF.setTarget(pos); ringF.setTarget(pos); pinkF.setTarget(pos);
}

void stopAllServos() {
  setAllTargets(0); // Alles open
  running = false;
  pattern = 0;
  durationMinutes = 0;
  Serial.println("Status: Sessie GESTOPT");
}
```

---

## Mogelijke uitbreidingen

* Extra patronen
* Manuele bediening met een fysieke knop
* Sterkte van de servo's aanpassen via de website
