/*
 *Skeč za potrebe članka u Svetu Kompjutera 
 *Author: Petrović Dejan
 *Date: 01/01/2019
 */
#include <SPI.h>
#include <RF24.h>
#include <Wire.h>;
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

RF24 myRadio (7, 8);
struct package
{
  int id=0;
  float temperature = 0.0;
  char  text[100] ="empty";
};
byte addresses[][6] = {"0"}; 
typedef struct package Package;
Package data;
int rLed = 3;
int gLed = 5;
int bLed = 6;
void setup() 
{
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();
  delay(1000);
  pinMode(rLed, OUTPUT);
  pinMode(gLed, OUTPUT);
  pinMode(bLed, OUTPUT);
  digitalWrite(rLed, LOW);
  digitalWrite(gLed, LOW);
  digitalWrite(bLed, LOW);

  myRadio.begin(); 
  myRadio.setChannel(115); 
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate( RF24_250KBPS ) ; 
  myRadio.openReadingPipe(1, addresses[0]);
  myRadio.startListening();
}



void loop()  
{

  if ( myRadio.available()) 
  {
    while (myRadio.available())
    {
      myRadio.read( &data, sizeof(data) );
    }
    display.clearDisplay();
    display.setTextSize(4);
    display.setTextColor(WHITE);
    display.setCursor(5,5);
    display.print(data.temperature,0);
    display.print(" ");
    display.print((char)247);
    display.print("C");
    display.display();
    //Serial.print("\nPackage:");
    //Serial.print(data.id);
    //Serial.print("\n");
    Serial.println(data.temperature);
    //Serial.println(data.text);
    if(data.temperature < 45){
      analogWrite(rLed, 81);
      analogWrite(gLed, 123);
      analogWrite(bLed, 150);
      Serial.print("blue");
    }else if(data.temperature >= 45 && data.temperature < 65){
      analogWrite(rLed, 75);
      analogWrite(gLed, 151);
      analogWrite(bLed, 0);
      Serial.println("green");
    }else if(data.temperature >= 65 && data.temperature < 75){
      analogWrite(rLed, 153);
      analogWrite(gLed, 153);
      analogWrite(bLed, 0);
      Serial.println("yellow");
    }else if(data.temperature >=75){
      analogWrite(rLed, 153);
      analogWrite(gLed, 0);
      analogWrite(bLed, 0);
      Serial.println("red");
        } 
  }

}
