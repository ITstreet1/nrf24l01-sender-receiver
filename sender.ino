/*
 *Skeč za potrebe članka u Svetu Kompjutera 
 *Author: Petrović Dejan
 *Date: 01/01/2019
 */
#include <SPI.h>  
#include "RF24.h"

RF24 myRadio (7, 8);
byte addresses[][6] = {"0"};
// struktura paketa
struct package
{
  int id=1; // redni broj poslatih paketa
  float temperature = 0; // promenljiva koja će prihvatiti temperaturu
  char  text[100] = "test";//možemo poslati tekst po potrebi
};


typedef struct package Package;
Package data;
//neophodno za termistor
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
  myRadio.setPALevel(RF24_PA_MAX); //predajnu snagu stavljamo na MAX
  myRadio.setDataRate( RF24_250KBPS ) ; //data rate ide na najmanje radi povećanja dometa
  myRadio.openWritingPipe( addresses[0]);
  delay(1000);
}

void loop()
{
  //računamo temperaturu
  Vout=analogRead(ntcPin);
  R2 = R1 * (1023.0 / (float)Vout - 1.0);
  Tk = (1.0 / (Ac + Bc*log(R2) + Cc*log(R2)*log(R2)*log(R2)));
  data.temperature = Tk - 273.15;
  //šaljemo paket, pri tom ispisujemo podatke na serijskom monitoru radi debagovanja, ovo se može isključiti
  myRadio.write(&data, sizeof(data)); 
  Serial.print("\nPackage:");
  Serial.print(data.id);
  Serial.print("\n");
  Serial.println(data.temperature);
  Serial.println(data.text);

  //Serial.println(Tc);
  //uvećavamo id u svakom loop-u za 1
  data.id = data.id + 1;
  delay(1000);

}
