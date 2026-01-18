#include <Stepper.h>

// ====== Instellingen ======
const int stepsPerRevolution = 200;   // pas aan voor jouw motor (bijv. 2048 voor 28BYJ-48)
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);  // pinnen naar IN1–IN4

const int buttonPin = 7;  // drukknop op pin 7

int buttonState = HIGH;
int lastButtonState = HIGH;

int state = 0; // 0 = stilstand, 1 = draaien vooruit, 2 = gestopt, 3 = terugdraaien
long stepCounter = 0;          // houdt bij hoeveel stappen er zijn gemaakt
const long maxSteps = stepsPerRevolution * 3;  // limiet: 3 volledige toeren

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);  // knop naar GND
  myStepper.setSpeed(30);            // snelheid in rpm
  Serial.begin(9600);
  Serial.println("Systeem opgestart: motor staat stil.");
  Serial.println("1x druk = vooruit (max 3 toeren), volgende druk = terugdraaien.");
}

void loop() {
  buttonState = digitalRead(buttonPin);

  // Detecteer knopdruk (HIGH → LOW overgang)
  if (lastButtonState == HIGH && buttonState == LOW) {
    // knop ingedrukt
    state++;

    // speciale logica: als motor al 3 toeren heeft gedaan, skip stap 2 (stop)
    if (stepCounter >= maxSteps && state == 2) {
      state = 3; // direct naar terugdraaien
    }

    if (state > 3) state = 1; // terug naar begin van cyclus

    switch (state) {
      case 1:
        stepCounter = 0; // reset teller
        Serial.println("1️⃣ Motor gestart (vooruit, max 3 toeren).");
        break;

      case 2:
        Serial.print("2️⃣ Motor gestopt. Gedraaide stappen: ");
        Serial.println(stepCounter);
        break;

      case 3:
        Serial.print("3️⃣ Motor draait terug: ");
        Serial.print(stepCounter);
        Serial.println(" stappen...");
        myStepper.step(-stepCounter); // evenveel stappen terug
        Serial.println("Terug op startpositie, motor stil.");
        state = 0; // terug naar stilstand
        break;
    }

    delay(300); // debounce
  }

  lastButtonState = buttonState;

  // Motor draait alleen vooruit zolang state == 1 en limiet niet bereikt is
  if (state == 1) {
    if (stepCounter < maxSteps) {
      myStepper.step(1);
      stepCounter++;
    } else {
      // automatisch stoppen als limiet is bereikt
      Serial.println("🔸 Limiet van 3 toeren bereikt, motor stopt automatisch.");
      state = 2; // ga naar stoptoestand
    }
  }
}
