#include <SPI.h>  
#include "RF24.h"

RF24 myRadio (7, 8);
byte addresses[][6] = {"0"};

struct package
{
  int id=1; // redni broj poslatih paketa
  float temperature = 0; // promenljiva koja će prihvatiti temperaturu
  char  text[100] = "test";//možemo poslati tekst po potrebi
};


typedef struct package Package;
Package data;

int ntcPin = A0;
int Vout;
float R1 = 10000;
float R2, Tk, Tc;
float Ac = 1.009249522e-03, Bc = 2.378405444e-04, Cc = 2.019202697e-07;

void setup()
{
  Serial.begin(9600);
  delay(1000);
  myRadio.begin();  
  myRadio.setChannel(115); 
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate( RF24_250KBPS ) ; 
  myRadio.openWritingPipe( addresses[0]);
  delay(1000);
}

void loop()
{
  Vout=analogRead(ntcPin);
  R2 = R1 * (1023.0 / (float)Vout - 1.0);
  Tk = (1.0 / (Ac + Bc*log(R2) + Cc*log(R2)*log(R2)*log(R2)));
  data.temperature = Tk - 273.15;
  
  myRadio.write(&data, sizeof(data)); 
  Serial.print("\nPackage:");
  Serial.print(data.id);
  Serial.print("\n");
  Serial.println(data.temperature);
  Serial.println(data.text);

  //Serial.println(Tc);
  
  data.id = data.id + 1;
  //data.temperature = data.temperature+0.1;
  delay(1000);

}
