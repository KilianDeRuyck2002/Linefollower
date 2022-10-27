
// Library
#include "SerialCommand.h"
#include "EEPROMAnything.h"

// Serial
#define SerialPort Serial
#define Baudrate 115200
SerialCommand sCmd(SerialPort);

// PinOut
const int Sensoren[] = {A7, A6, A5, A4, A3, A2, A1, A0};

// Variabelen
bool debug;
unsigned long Current, Previous, Difference, CalculationTime;
unsigned long CycleTime = 1000000;

struct Parameter_t
{
  unsigned long CycleTime;
  int Black[8];
  int White[8];
} Parameters;

void setup()
{
  SerialPort.begin(Baudrate);

  // Set commando
  sCmd.addCommand("set", onSet);
  sCmd.addCommand("debug", onDebug);
  sCmd.addCommand("calibrate", onCalibrate);
  sCmd.setDefaultHandler(onUnknownCommand);

  EEPROM_readAnything(0, Parameters);

  SerialPort.println("Ready To Rumble");
}

void loop()
{
  sCmd.readSerial();

  Current = micros();
  if (Current - Previous >= Parameters.CycleTime)
  {
    Previous = Current;

    int normalised[8];

    SerialPort.print("Normalised Values Sensoren: ");
    for (int i = 0; i < 8; i++)
    {
      normalised[i] = map(analogRead(Sensoren[i]), Parameters.Black[i], Parameters.White[i], 0, 1000);
      SerialPort.print(normalised[i]);
      SerialPort.print(" ");
    }
    SerialPort.println();

    int index = 0;
    for (int i = 1; i < 8; i++) if (normalised[i] < normalised[index]) index = i;

    float Position;
    if (index == 0) Position = -45;
    else if (index == 7) Position = 45;
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

      Position *= 15;
    }
    
    SerialPort.print("Position: ");
    SerialPort.println(Position);
  }

  Difference = micros() - Current;
  if (Difference > CalculationTime) CalculationTime = Difference;
}

// Kalibratie Sensoren
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

  if (strcmp(Parameter, "cycle") == 0) Parameters.CycleTime = atol(Waarde);

  EEPROM_writeAnything(0, Parameters);
}

// Woord op de SerialPort = "debug"
void onDebug()
{
  SerialPort.print("Cycle Time: ");
  SerialPort.println(Parameters.CycleTime);

  SerialPort.print("Black: ");
  for (int i = 0; i < 8; i++)
  {
    SerialPort.print(Parameters.Black[i]);
    SerialPort.print(" ");
  }
  SerialPort.println(" ");

  SerialPort.print("White: ");
  for (int i = 0; i < 8; i++)
  {
    SerialPort.print(Parameters.White[i]);
    SerialPort.print(" ");
  }
  SerialPort.println(" ");

  SerialPort.print("Calculation Time: ");
  SerialPort.println(CalculationTime);
  CalculationTime = 0;
}

// Woord niet herkend op de SerialPort = Error
void onUnknownCommand(char * command)
{
  SerialPort.print("unknown command: \"");
  SerialPort.print(command);
  SerialPort.println("\"");
}
