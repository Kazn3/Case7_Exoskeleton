#include <Servo.h>

Servo myServo;

const int buttonPin = 7;  // drukknop
int buttonState = HIGH;
int lastButtonState = HIGH;
int state = 0;            // 0 = stilstand, 1 = vooruit, 2 = gestopt, 3 = terug
int currentAngle = 0;     // huidige positie
int targetAngle = 0;      // doelhoek (0 of 180)
int speedDelay = 15;      // snelheid (ms per graad) — lager = sneller
bool returning = false;   // voorkomt onderbreking tijdens terugkeer

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  myServo.attach(9);
  myServo.write(0);  // Servo blijft stil op start (0°)
  Serial.begin(9600);
  Serial.println("Servo-programma gestart. Wacht op eerste druk...");
}

void loop() {
  buttonState = digitalRead(buttonPin);

  // Detecteer knopdruk (flankdetectie)
  if (lastButtonState == HIGH && buttonState == LOW && !returning) {
    state++;

    // Reset state als die te ver gaat
    if (state > 3) state = 1;

    // Controleer de gedragingen per fase
    switch (state) {
      case 1:
        Serial.println("1️⃣ Servo draait naar 180°...");
        targetAngle = 180;
        break;

      case 2:
        // Alleen stoppen als de servo nog NIET 180° bereikt heeft
        if (currentAngle < 180) {
          Serial.println("2️⃣ Servo stopt op huidige positie.");
          targetAngle = currentAngle;
        } else {
          // Als hij al 180° heeft bereikt, sla stap 2 over en ga naar stap 3
          Serial.println("⏩ Servo was al op 180°, overslaat stap 2 → direct terug.");
          state = 3;
          targetAngle = 0;
          returning = true;
        }
        break;

      case 3:
        Serial.println("3️⃣ Servo draait terug naar 0° (kan niet onderbroken worden).");
        targetAngle = 0;
        returning = true; // blokkeert verdere knopdrukken
        break;
    }

    delay(300); // debounce
  }

  lastButtonState = buttonState;

  // Servo beweegt geleidelijk richting targetAngle
  if (currentAngle < targetAngle) {
    currentAngle++;
    myServo.write(currentAngle);
    delay(speedDelay);
  } else if (currentAngle > targetAngle) {
    currentAngle--;
    myServo.write(currentAngle);
    delay(speedDelay);
  }

  // Controleer of hij 180° bereikt heeft tijdens vooruitbeweging
  if (state == 1 && currentAngle >= 180) {
    Serial.println("✅ Servo heeft 180° bereikt, wacht op volgende druk (stap 3).");
  }

  // Als servo terug is op 0° → reset alles
  if (state == 3 && currentAngle == 0 && returning) {
    Serial.println("🏁 Servo terug op 0°, programma wordt gereset.");
    returning = false;
    state = 0;
    targetAngle = 0;
  }
}
