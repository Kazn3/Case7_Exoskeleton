int sensorPin = A1;    // select the input pin for the sensor
int sensorValue = 0;

void setup() {
  pinMode(sensorPin, INPUT);
  
  Serial.begin(9600);
}

void loop() {
  // read the value from the sensor:
  sensorValue = analogRead(sensorPin);
  Serial.println(sensorValue);
}