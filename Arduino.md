# Code arduino

#include <ArduinoBLE.h>
#include <Servo.h>

// ==========================================

// === INSTELLINGEN VOOR SNELHEID ===
// ==========================================
// Tijd in milliseconden tussen elke graad verandering.
// 5 = Heel snel
// 20 = Normaal 
// 50 = Langzaam
const int SERVO_SNELHEID = 15; 

// ==========================================

// Een structuur om soepele beweging te beheren zonder 'delay'
struct SmoothServo {
  Servo servo;
  int currentPos;    // Waar is de servo nu?
  int targetPos;     // Waar moet hij heen?
  unsigned long lastMoveTime; 
  int stepDelay;     // Snelheid

  void attach(int pin, int startPos, int speed) {
    servo.attach(pin);
    currentPos = startPos;
    targetPos = startPos;
    stepDelay = speed;
    servo.write(currentPos);
  }

  // Deze functie roep je continu aan in de loop
  void update() {
    // Als we al op het doel zijn, doe niets
    if (currentPos == targetPos) return;

    // Check of het tijd is voor de volgende stap
    if (millis() - lastMoveTime >= stepDelay) {
      lastMoveTime = millis();
      
      // Stapje dichterbij het doel
      if (currentPos < targetPos) {
        currentPos++;
      } else {
        currentPos--;
      }
      servo.write(currentPos);
    }
  }

  // Stel een nieuw doel in, de update() functie regelt de beweging
  void setTarget(int pos) {
    targetPos = pos;
  }
  
  // Direct stoppen of resetten
  void stopImmediate(int pos) {
    targetPos = pos;
  }
};

// === Servo Objecten ===
SmoothServo s1, s2, s3, s4, s5;
const int servoPins[] = {3, 5, 6, 9, 10};

// === Patroon-instellingen ===
int pattern = 0;                
unsigned long lastToggle = 0;   
bool toggleState = false;       

// Intervallen per patroon (ms) - Nu variabelen in plaats van constanten
// Ze worden berekend in setup() zodat de servo altijd de overkant haalt.
unsigned long intervalPattern1;
unsigned long intervalPattern2;
unsigned long intervalPattern3;
unsigned long intervalPattern4;
unsigned long intervalPattern5;

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
  
  // Servo's initialiseren met de ingestelde snelheid
  s1.attach(servoPins[0], 0, SERVO_SNELHEID);
  s2.attach(servoPins[1], 0, SERVO_SNELHEID);
  s3.attach(servoPins[2], 0, SERVO_SNELHEID);
  s4.attach(servoPins[3], 0, SERVO_SNELHEID);
  s5.attach(servoPins[4], 0, SERVO_SNELHEID);

  // === BEREKEN MINIMALE TIJD VOOR VOLLEDIGE BEWEGING ===
  // Formule: (Aantal graden * Snelheid) + Beetje rustpauze
  
  // Patroon 1: 0-180 graden (180 graden reis)
  intervalPattern1 = (180 * SERVO_SNELHEID) + 500; 
  
  // Patroon 2: 0-180 graden, maar we willen deze langzamer laten voelen (extra pauze)
  intervalPattern2 = (180 * SERVO_SNELHEID) + 2000;
  
  // Patroon 3: 30-150 graden (120 graden reis)
  intervalPattern3 = (120 * SERVO_SNELHEID) + 500;
  
  // Patroon 4: 45-135 graden (90 graden reis)
  intervalPattern4 = (90 * SERVO_SNELHEID) + 500;
  
  // Patroon 5: 0-180 graden (was heel snel, nu minimaal haalbaar)
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

  Serial.println("✅ Klaar! Intervallen zijn automatisch aangepast aan de snelheid.");
}

void loop() {
  // 1. Update de fysieke beweging van ALLE servo's (doet kleine stapjes indien nodig)
  s1.update();
  s2.update();
  s3.update();
  s4.update();
  s5.update();

  BLEDevice central = BLE.central();

  if (central) {
    if (central.connected()) {
      if (commandCharacteristic.written()) {
        String cmd = commandCharacteristic.value();
        handleCommand(cmd);
      }

      checkButton();

      if (running) {
        runPatternLogic(); // Bepaalt alleen de DOELEN
      } else {
        stopAllServos();   // Zet DOEL op 0
      }
    }
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

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

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

// Deze functie zet nu alleen de TARGETS (doelen), hij beweegt ze niet direct.
// De .update() functies in de loop() doen de daadwerkelijke beweging.
void runPatternLogic() {
  unsigned long currentMillis = millis();

  // --- Patroon 1 ---
  if (pattern == 1 && currentMillis - lastToggle >= intervalPattern1) {
    toggleState = !toggleState;
    lastToggle = currentMillis;
    int pos = toggleState ? 0 : 180;
    s1.setTarget(pos);
    s2.setTarget(pos);
  }

  // --- Patroon 2 ---
  else if (pattern == 2 && currentMillis - lastToggle >= intervalPattern2) {
    toggleState = !toggleState;
    lastToggle = currentMillis;
    int pos = toggleState ? 0 : 180;
    s3.setTarget(pos);
    s4.setTarget(pos);
  }

  // --- Patroon 3 ---
  else if (pattern == 3 && currentMillis - lastToggle >= intervalPattern3) {
    toggleState = !toggleState;
    lastToggle = currentMillis;
    int pos = toggleState ? 30 : 150;
    s1.setTarget(pos);
    s5.setTarget(pos);
  }

  // --- Patroon 4 ---
  else if (pattern == 4 && currentMillis - lastToggle >= intervalPattern4) {
    toggleState = !toggleState;
    lastToggle = currentMillis;
    int pos = toggleState ? 45 : 135;
    s1.setTarget(pos);
    s2.setTarget(pos);
    s3.setTarget(pos);
    s4.setTarget(pos);
    s5.setTarget(pos);
  }

  // --- Patroon 5 ---
  else if (pattern == 5 && currentMillis - lastToggle >= intervalPattern5) {
    toggleState = !toggleState;
    lastToggle = currentMillis;
    int posA = toggleState ? 0 : 180;   
    int posB = toggleState ? 180 : 0;   
    s1.setTarget(posA);
    s3.setTarget(posA);
    s5.setTarget(posA);
    s2.setTarget(posB);
    s4.setTarget(posB);
  }
}

void stopAllServos() {
  // Stuur iedereen rustig naar 0
  s1.setTarget(0);
  s2.setTarget(0);
  s3.setTarget(0);
  s4.setTarget(0);
  s5.setTarget(0);
}
