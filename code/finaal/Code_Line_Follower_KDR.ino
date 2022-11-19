
// Library
#include "SerialCommand.h"
#include "EEPROMAnything.h"

// Serial
#define SerialPort Serial
#define Baudrate 9600
SerialCommand sCmd(SerialPort);

// PinOut
#define MotorRightForward 11
#define MotorRightBackward 10
#define MotorLeftForward 3
#define MotorLeftBackward 9
const int Sensoren[] = {A7, A6, A5, A4, A3, A2, A1, A0};
const int StartKnop = 2;
const int LedBlauw = 12;
const int LedSensor = 13;

// Variabelen
int normalised[8];
bool StatusStartKnop;
bool debug;
int run;
int Error;
float Position;
unsigned long Current, Previous, Difference, CalculationTime;
unsigned long CycleTime = 2000;

struct Parameter_t
{
  unsigned long CycleTime;
  int Black[8];
  int White[8];
  int Power = 40;
  float Diff = 0.8;
  float Kp = 2.0;
  float Ki = 0;
  float Kd = 0;
} Parameters;

void setup()
{
  SerialPort.begin(Baudrate);

  // Set commando
  sCmd.addCommand("set", onSet);
  sCmd.addCommand("debug", onDebug);
  sCmd.addCommand("calibrate", onCalibrate);
  sCmd.addCommand("run", onRun);
  sCmd.addCommand("stop", onStop);
  sCmd.setDefaultHandler(onUnknownCommand);

  EEPROM_readAnything(0, Parameters);

  // Sensoren
  for (int i = 0; i < 7; i++)
  {
    pinMode(Sensoren[i], INPUT);
  }

  // Sensoren actief
  pinMode(LedSensor, OUTPUT);
  digitalWrite(LedSensor, HIGH);

  // Startknop
  pinMode(StartKnop, INPUT);
  attachInterrupt(digitalPinToInterrupt(StartKnop), Interrupt, RISING);

  // LED
  pinMode(LedBlauw, OUTPUT);

  // Motoren
  pinMode(MotorRightForward, OUTPUT);
  pinMode(MotorRightBackward, OUTPUT);
  pinMode(MotorLeftBackward, OUTPUT);
  pinMode(MotorLeftForward, OUTPUT);

  SerialPort.println("Ready To Rumble");
}

void loop()
{
  sCmd.readSerial();

  Current = micros();
  if (Current - Previous >= Parameters.CycleTime)
  {
    Previous = Current;

    for (int i = 0; i < 8; i++)
    {
      normalised[i] = map(analogRead(Sensoren[i]), Parameters.Black[i], Parameters.White[i], 0, 1000);
    }

    int index = 0;

    for (int i = 1; i < 8; i++) if (normalised[i] < normalised[index]) index = i;

    // Stop rijden als alle sensoren wit zien
    if (normalised[index] > 900) 
    {
      digitalWrite(LedBlauw, LOW);
      run = 0;
    }

    if (index == 0) Position = -30;
    else if (index == 7) Position = 30;
    else
    {
      int SensorNul = normalised[index];
      int SensorMinEen = normalised[index - 1];
      int SensorPlusEen = normalised[index + 1];

      float b = SensorPlusEen - SensorMinEen;
      b = b / 2;

      float a = SensorPlusEen - b - SensorNul;

      Position = -b / (2 * a);
      Position += index;
      Position -= 3.5;

      Position *= 9.525;
    }

    // Error = setpoint - input
    Error = -Position;

    // P regelaar
    float Output = Error * Parameters.Kp;

    // I regelaar
    int iTerm;
    iTerm += Parameters.Ki * Error;
    iTerm = constrain(iTerm, -510, 510);
    Output += iTerm;

    // D regelaar
    int LastError;
    Output += Parameters.Kd * (Error - LastError);
    LastError = Error;

    // Output begrenzen tot wat fysiek mogelijk is
    Output = constrain(Output, -510, 510);
    SerialPort.println(Output);

    float PowerLeft = 0;
    float PowerRight = 0;

    if (run) if (Output >= 0)
      {
        PowerLeft = constrain((Parameters.Power + (Parameters.Diff * Output)), -255, 255);
        PowerRight = constrain((PowerLeft - Output), -255, 255);
        PowerLeft = PowerRight + Output;
      }
      else
      {
        PowerRight = constrain((Parameters.Power - (Parameters.Diff * Output)), -255, 255);
        PowerLeft = constrain((PowerRight + Output), -255, 255);
        PowerRight = PowerLeft - Output;
      }
      SerialPort.print("PowerRight: ");
      SerialPort.println(PowerRight);
      SerialPort.print("PowerLeft: ");
      SerialPort.println(PowerLeft);

    analogWrite(MotorLeftForward, PowerLeft > 0 ? PowerLeft : 0);
    analogWrite(MotorLeftBackward, PowerLeft < 0 ? -PowerLeft : 0);
    analogWrite(MotorRightForward, PowerRight > 0 ? PowerRight : 0);
    analogWrite(MotorRightBackward, PowerRight < 0 ? -PowerRight : 0);
  }
  Difference = micros() - Current;
  if (Difference > CalculationTime) CalculationTime = Difference;
}

void Interrupt()
{
  digitalWrite(LedBlauw, HIGH);
  run = 1;
}

void onRun()
{
  digitalWrite(LedBlauw, HIGH);
  run = 1;
}

void onStop()
{
  digitalWrite(LedBlauw, LOW);
  run = 0;
}

// Woord op de SerialPort = "calibrate"
void onCalibrate()
{
  char * Parameter = sCmd.next();

  if (strcmp(Parameter, "black") == 0)
  {
    SerialPort.println("Start calibrating black... ");
    for (int i = 0; i < 8; i++) Parameters.Black[i] = analogRead(Sensoren[i]);
    SerialPort.print("...Calibrating done");
  }

  if (strcmp(Parameter, "white") == 0)
  {
    SerialPort.println("Start calibrating white... ");
    for (int i = 0; i < 8; i++) Parameters.White[i] = analogRead(Sensoren[i]);
    SerialPort.print("...Calibrating done");
  }

  EEPROM_writeAnything(0, Parameters);
}

// Woord op de SerialPort = "set"
void onSet()
{
  char * Parameter = sCmd.next(); // 1ste argument
  char * Waarde = sCmd.next(); // 2de argument

  if (strcmp(Parameter, "cycle") == 0)
  {
    long NewCycleTime = atol(Waarde);
    float Ratio = ((float) NewCycleTime) / ((float) Parameters.CycleTime);

    Parameters.Ki *= Ratio;
    Parameters.Kd /= Ratio;

    Parameters.CycleTime = NewCycleTime;
  }
  /*
    else if (strcmp(Parameter, "ki") == 0)
    {
    float CycleTimeInSec = ((float) Parameters.CycleTime) / 1000000;
    Parameters.Ki = atof(Waarde) * CycleTimeInSec;
    }
    else if (strcmp(Parameter, "kd") == 0)
    {
    float CycleTimeInSec = ((float) Parameters.CycleTime) / 1000000;
    Parameters.Kd = atof(Waarde) * CycleTimeInSec;
    }
  */
  else if (strcmp(Parameter, "power") == 0) Parameters.Power = atol(Waarde);
  else if (strcmp(Parameter, "diff") == 0) Parameters.Diff = atof(Waarde);
  else if (strcmp(Parameter, "kp") == 0) Parameters.Kp = atof(Waarde);
  else if (strcmp(Parameter, "ki") == 0) Parameters.Ki = atof(Waarde);
  else if (strcmp(Parameter, "kd") == 0) Parameters.Kd = atof(Waarde);

  EEPROM_writeAnything(0, Parameters);
}

// Woord op de SerialPort = "debug"
void onDebug()
{
  SerialPort.print("Cycle Time: ");
  SerialPort.println(Parameters.CycleTime);

  /*
    // Zwarte waarden debuggen sensor
    SerialPort.print("Black: ");
    for (int i = 0; i < 8; i++)
    {
    SerialPort.print(Parameters.Black[i]);
    SerialPort.print(" ");
    }
    SerialPort.println(" ");

    // Witte waarden debuggen sensor
    SerialPort.print("White: ");
    for (int i = 0; i < 8; i++)
    {
    SerialPort.print(Parameters.White[i]);
    SerialPort.print(" ");
    }
    SerialPort.println(" ");
  */

  SerialPort.print("Error: ");
  SerialPort.println(Error);

  float CycleTimeInSec = ((float) Parameters.CycleTime) / 1000000;

  float Power = Parameters.Power;
  SerialPort.print("Power: ");
  SerialPort.println(Power);

  float Diff = Parameters.Diff;
  SerialPort.print("Diff: ");
  SerialPort.println(Diff);

  float Kp = Parameters.Kp;
  SerialPort.print("Kp: ");
  SerialPort.println(Kp);

  //float Ki = Parameters.Ki / CycleTimeInSec;
  float Ki = Parameters.Ki;
  SerialPort.print("Ki: ");
  SerialPort.println(Ki);

  //float Kd = Parameters.Kd * CycleTimeInSec;
  float Kd = Parameters.Kd;
  SerialPort.print("Kd: ");
  SerialPort.println(Kd);

  SerialPort.print("Normalised Values Sensoren: ");
  for (int i = 0; i < 8; i++)
  {
    normalised[i] = map(analogRead(Sensoren[i]), Parameters.Black[i], Parameters.White[i], 0, 1000);
    SerialPort.print(normalised[i]);
    SerialPort.print(" ");
  }
  SerialPort.println();

  SerialPort.print("Calculation Time: ");
  SerialPort.println(CalculationTime);
  CalculationTime = 0;
}

// Woord niet herkend op de SerialPort => Error
void onUnknownCommand(char * command)
{
  SerialPort.print("unknown command: \"");
  SerialPort.print(command);
  SerialPort.println("\"");
}
