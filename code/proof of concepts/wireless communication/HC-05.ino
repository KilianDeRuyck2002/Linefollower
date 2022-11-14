/*
   Bluetooth module HC-05
   Code om te connecteren = 1234 of 0000
*/
int LedBlauw = 12;
char ReadSerial = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(LedBlauw, OUTPUT);
}
void loop()
{
  if (Serial.available() > 0)
  {
    ReadSerial = Serial.read();
    Serial.println(ReadSerial);
    if (ReadSerial == '0')
    {
      digitalWrite(LedBlauw, LOW);
    }
    else if (ReadSerial == '1')
    {
      digitalWrite(LedBlauw, HIGH);
    }
    else if (ReadSerial == '2')
    {
      digitalWrite(LedBlauw, HIGH);
      delay(500);
      digitalWrite(LedBlauw, LOW);
      delay(500);
      digitalWrite(LedBlauw, HIGH);
      delay(500);
      digitalWrite(LedBlauw, LOW);
      delay(500);
      digitalWrite(LedBlauw, HIGH);
      delay(500);
      digitalWrite(LedBlauw, LOW);
    }
  }
}
