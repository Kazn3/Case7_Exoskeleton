#include <Stepper.h>

// ====== Instellingen ======
const int stepsPerRevolution = 200;   // pas aan voor jouw motor (bijv. 2048 voor 28BYJ-48)
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);  // pinnen naar IN1–IN4

const int buttonPin = 7;  // drukknop op pin 7

int buttonState = LOW;
int lastButtonState = LOW;

int state = 0; // 0 = stilstand, 1 = vooruit draaien, 2 = gestopt, 3 = terugdraaien
long stepCounter = 0; // telt hoeveel stappen vooruit gedraaid zijn

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);  // knop tussen pin 7 en GND
  myStepper.setSpeed(60);            // snelheid in rpm
  Serial.begin(9600);
  Serial.println("Systeem opgestart: motor staat stil.");
  Serial.println("Druk 1x: start vooruit, 2x: stop, 3x: draai terug.");
}

void loop() {
  buttonState = digitalRead(buttonPin);

  // Detecteer knopdruk (HIGH -> LOW overgang)
  if (lastButtonState == HIGH && buttonState == LOW) {
    state++;
    if (state > 3) state = 1;  // terug naar begin van cyclus

    switch (state) {
      case 1: // Start motor vooruit
        stepCounter = 0; // reset teller
        Serial.println("1️⃣ Motor gestart (vooruit).");
        break;

      case 2: // Stop motor
        Serial.print("2️⃣ Motor gestopt. Gedraaide stappen: ");
        Serial.println(stepCounter);
        break;

      case 3: // Draai terug evenveel stappen
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

  // Motor draait alleen vooruit in state 1
  if (state == 1) {
    myStepper.step(1);
    stepCounter++; // bijhouden hoeveel stappen zijn gezet
  }
}
