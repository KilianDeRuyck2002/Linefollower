
#include <SoftwareSerial.h>
SoftwareSerial HC_05(1, 0); // RX | TX (Arduino)
String Text;
char Text2;
void setup()
{
  Serial.begin(9600);
  HC_05.begin(9600);
}
void loop()
{
  if (HC_05.available())
  {
    Text = Serial.write(HC_05.read()); 
    Serial.println(Text);
  }
  if (Serial.available())
  {
    Text2 = HC_05.write(Serial.read());
    Serial.find("Hallo");
    Serial.println("Hallo");
  }
}
