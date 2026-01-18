#include <Stepper.h>

// ====== Instellingen ======
const int stepsPerRevolution = 2048;   // 28BYJ-48 heeft 2048 stappen per omwenteling
Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11);  // juiste volgorde voor ULN2003 driver

const int buttonPin = 7;  // drukknop op pin 7

int buttonState = HIGH;
int lastButtonState = HIGH;

int state = 0; // 0 = stilstand, 1 = draaien vooruit, 2 = gestopt, 3 = terugdraaien
long stepCounter = 0;          
const long maxSteps = stepsPerRevolution * 2;  // limiet: 3 toeren vooruit

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);  // knop tussen pin 7 en GND
  myStepper.setSpeed(15);            // snelheid in rpm (10 is veilig voor 28BYJ-48)
  Serial.begin(9600);
  Serial.println("Systeem opgestart: motor staat stil.");
  Serial.println("1x druk = vooruit (max 3 toeren), volgende druk = terugdraaien.");
}

void loop() {
  buttonState = digitalRead(buttonPin);

  // Detecteer knopdruk (HIGH → LOW overgang)
  if (lastButtonState == HIGH && buttonState == LOW) {
    state++;

    // Als motor al 3 toeren heeft gedaan, sla stap 2 over (ga direct naar 3)
    if (stepCounter >= maxSteps && state == 2) {
      state = 3;
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
      Serial.println("🔸 Limiet van 3 toeren bereikt, motor stopt automatisch.");
      state = 2; // stoptoestand
    }
  }
}
