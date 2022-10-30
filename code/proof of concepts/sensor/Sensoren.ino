int L1 = A7;
int L2 = A6;
int L3 = A5;
int L4 = A4;
int R4 = A3;
int R3 = A2;
int R2 = A1;
int R1 = A0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(L1, INPUT);
  pinMode(L2, INPUT);
  pinMode(L3, INPUT);
  pinMode(L4, INPUT);
  pinMode(R4, INPUT);
  pinMode(R3, INPUT);
  pinMode(R2, INPUT);
  pinMode(R1, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print(analogRead(L1));
  Serial.print(" ");
  Serial.print(analogRead(L2));
  Serial.print(" ");
  Serial.print(analogRead(L3));
  Serial.print(" ");
  Serial.print(analogRead(L4));
  Serial.print(" ");
  Serial.print(analogRead(R4));
  Serial.print(" ");
  Serial.print(analogRead(R3));
  Serial.print(" ");
  Serial.print(analogRead(R2));
  Serial.print(" ");
  Serial.print(analogRead(R1));
  Serial.println();
}
