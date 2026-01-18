#include <Stepper.h>

// ====== Instellingen ======
const int stepsPerRevolution = 200;   // pas aan voor jouw motor (bijv. 2048 voor 28BYJ-48)
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);  // pinnen naar IN1–IN4

const int buttonPin = 7;  // drukknop op pin 7
int buttonState = LOW;
int lastButtonState = LOW;

int state = 0; // 0 = stilstand, 1 = draaien vooruit, 2 = gestopt na draaien, 3 = achteruit één toer

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);  // knop naar GND
  myStepper.setSpeed(60);            // snelheid in rpm
  Serial.begin(9600);
  Serial.println("Systeem opgestart: motor staat stil.");
  Serial.println("Druk op de knop om de motor te bedienen in 3 stappen.");
}

void loop() {
  buttonState = digitalRead(buttonPin);

  // Detecteer knopdruk (overgang HIGH → LOW)
  if (lastButtonState == HIGH && buttonState == LOW) {
    state++; // volgende fase
    if (state > 3) state = 1; // herstart cyclus

    switch (state) {
      case 1:
        Serial.println("1️⃣ Motor gestart (vooruit).");
        break;

      case 2:
        Serial.println("2️⃣ Motor gestopt.");
        break;

      case 3:
        Serial.println("3️⃣ Motor draait 1 toer achteruit...");
        myStepper.step(-stepsPerRevolution); // één toer terug
        Serial.println("Klaar, motor staat stil.");
        state = 0; // terug naar stilstand
        break;
    }
    delay(300); // debounce
  }

  lastButtonState = buttonState;

  // Laat de motor draaien als state == 1
  if (state == 1) {
    myStepper.step(1);  // vooruit draaien
  }
}
