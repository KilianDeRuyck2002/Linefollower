/*
   Bluetooth module HC-05
   Code om te connecteren = 1234 of 0000
*/

String reading;

void setup() {
  Serial.begin(9600);
}

void loop() {
  if (Serial.available())
  {
    reading = Serial.readString();
    Serial.println(reading);
  }
}
