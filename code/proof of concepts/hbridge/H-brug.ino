// Startknop
const int Knop = 2;

// Led
const int LedBlauw = 12;

// Motor Rechts
const int IN1 = 3;
const int IN2 = 9;

// Motor Links
const int IN3 = 10;
const int IN4 = 11;

// Variabelen
bool StatusKnop;
int SnelheidMotorLinks;
int SnelheidMotorRechts;

void setup() {
  Serial.begin(9600);

  // Digitaal
  pinMode(LedBlauw, OUTPUT);
  pinMode(Knop, INPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Snelheid
  SnelheidMotorLinks = 255;
  SnelheidMotorRechts = 255;
}

void loop() {
  StatusKnop = digitalRead(Knop);

  if (StatusKnop == HIGH)
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
    delay(500);

    Motoren_Voorwaarts();
    delay(500);
    Motoren_Stop();
    delay(500);

    Motoren_Achterwaarts();
    delay(500);
    Motoren_Stop();
    delay(500);

    Motoren_Links();
    delay(500);
    Motoren_Stop();
    delay(500);

    Motoren_Rechts();
    delay(500);
    Motoren_Stop();
    delay(500);
  }

  else
  {
    Motoren_Stop();
  }
}

void Motoren_Voorwaarts() {
  analogWrite(IN1, SnelheidMotorRechts);
  analogWrite(IN2, 0);
  analogWrite(IN3, 0);
  analogWrite(IN4, SnelheidMotorLinks);
}

void Motoren_Achterwaarts() {
  analogWrite(IN1, 0);
  analogWrite(IN2, SnelheidMotorRechts);
  analogWrite(IN3, SnelheidMotorLinks);
  analogWrite(IN4, 0);
}

void Motoren_Rechts() {
  analogWrite(IN1, 0);
  analogWrite(IN2, SnelheidMotorRechts);
  analogWrite(IN3, 0);
  analogWrite(IN4, SnelheidMotorLinks);
}

void Motoren_Links() {
  analogWrite(IN1, SnelheidMotorRechts);
  analogWrite(IN2, 0);
  analogWrite(IN3, SnelheidMotorLinks);
  analogWrite(IN4, 0);
}

void Motoren_Stop() {
  analogWrite(IN1, 0);
  analogWrite(IN2, 0);
  analogWrite(IN3, 0);
  analogWrite(IN4, 0);
}
