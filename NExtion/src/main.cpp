#include <Arduino.h>
#include <Nextion.h>
#include "DHT.h"
#include <Nextion.h>
//#include <TEA5767.h>
#include <Adafruit_Sensor.h>
//Radio
TEA5767 radio;
double list[10] = {106.9,88.2,97.5,89.1,105.3,106.5,87.7,106.2,104.0,107.2};
int pozycja = 0;
int relayRadio = 7;
int glosnosc = 7;
bool wlaczoneRadio = false;
//Led
int relayLedPort = 4, Ledport = 5;
//Jakosc 
int sensorThres = 400;
int smokeA0 = A3;
bool zle = false;
//Ekran
int Led1 = 0,CurrentPage = 0;
bool LedStatus = false;
NexButton b0 = NexButton(1, 2, "b0");
NexButton b4 = NexButton(1, 5, "b4");
NexButton b5 = NexButton(1, 6, "b5");
//Radio
NexButton b7 = NexButton(2, 5, "b7");
NexButton b8 = NexButton(2, 6, "b8");
NexButton b6 = NexButton(2, 4, "b6");
NexButton b9 = NexButton(2, 12, "b9");
NexButton b10 = NexButton(2, 13, "b10");
NexText t10 = NexText(2, 2, "t10");
NexText t11 = NexText(2, 8, "t11");
//
NexPage page0    = NexPage(0, 0, "page0");
NexPage page1 = NexPage(1, 0, "page1");
NexPage page2 = NexPage(2, 0, "page2");
NexText t20 = NexText(0, 13, "t20");
NexText t6 = NexText(1, 8, "t6");
NexText t0 = NexText(0, 3, "t0");
NexText t2 = NexText(0, 9, "t2");
NexText t7 = NexText(1, 3, "t7");
NexTouch *nex_listen_list[] = 
{
  &b0,
  &b4,
  &b5,
  &b6,
  &b7,
  &b8,
  &b9,
  &b10,
  &t6,
  &t0,
  &t2,
  &t7,
  &t10,
  &t11,
  &t20,
  &page0,
  &page1,
  &page2,
  NULL  // String terminated
};
void b6PopCallback(void *ptr)  // Press event for button b1
{
  if(pozycja<9){
    pozycja++;
    radio.set_frequency(list[pozycja]);
    char str[8];
    itoa( list[pozycja], str, 10 );
    t10.setText(str);
  }
}
void b7PopCallback(void *ptr)  // Press event for button b1
{
  if(pozycja>0){
    pozycja--;
    radio.set_frequency(list[pozycja]);
    char str[8];
    itoa( list[pozycja], str, 10 );
    t10.setText(str);
  }
}
void b8PopCallback(void *ptr)  // Press event for button b1
{
  if(!wlaczoneRadio){
    digitalWrite(relayRadio, LOW);
    t11.setText("Wlaczone");
  }else{
    digitalWrite(relayRadio, HIGH);
    t11.setText("Wylaczone");
  }
}
void b9PopCallback(void *ptr)  // Press event for button b1
{
  
}
void b10PopCallback(void *ptr)  // Press event for button b1
{
  
}
void b0PopCallback(void *ptr)  // Press event for button b1
{
  if(LedStatus){
    digitalWrite(4, LOW);
    LedStatus = false;
    t7.setText("Wlaczone");
    b0.setText("Wyłącz");
  }else{
    digitalWrite(4, HIGH);
    LedStatus = true;
    t7.setText("Wylaczone");
    b0.setText("Włącz");
  }
}
void b4PopCallback(void *ptr)  // Press event for button b1
{
  if(Led1<10){
    Led1++;
    char str[8];
    itoa( Led1, str, 10 );
    t6.setText(str);
    switch (Led1)
    {
    case 1:
      analogWrite(Ledport, 25);
      break;
    case 2:
      analogWrite(Ledport, 50);
      break;
    case 3:
      analogWrite(Ledport, 75);
      break;
    case 4:
      analogWrite(Ledport, 100);
      break;
    case 5:
      analogWrite(Ledport, 125);
      break;
    case 6:
      analogWrite(Ledport, 150);
      break;
    case 7:
      analogWrite(Ledport, 175);
      break;
    case 8:
      analogWrite(Ledport, 200);
      break;
    case 9:
      analogWrite(Ledport, 225);
      break;
    case 10:
      analogWrite(Ledport, 255);
      break;
    default:
      break;
    }
  } 
}
void b5PopCallback(void *ptr)  // Press event for button b1
{
  if(Led1>0){
    Led1--;
    char str[8];
    itoa( Led1, str, 10 );
    t6.setText(str);
    switch (Led1)
    {
    case 0:
      analogWrite(relayLedPort, HIGH);
      break;
    case 1:
      analogWrite(Ledport, 25);
      break;
    case 2:
      analogWrite(Ledport, 50);
      break;
    case 3:
      analogWrite(Ledport, 75);
      break;
    case 4:
      analogWrite(Ledport, 100);
      break;
    case 5:
      analogWrite(Ledport, 125);
      break;
    case 6:
      analogWrite(Ledport, 150);
      break;
    case 7:
      analogWrite(Ledport, 175);
      break;
    case 8:
      analogWrite(Ledport, 200);
      break;
    case 9:
      analogWrite(Ledport, 225);
      break;
    default:
      break;
    }
  }
}
void page0PushCallback(void *ptr) 
{
  CurrentPage = 0;  
} 
void page1PushCallback(void *ptr)  
{
  CurrentPage = 1;  
} 
void page2PushCallback(void *ptr) 
{
  CurrentPage = 2;  
} 
//Temp
#define DHTPIN 3
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
void setup() {
  dht.begin();
  nexInit();
  b0.attachPop(b0PopCallback,&b0);
  b7.attachPop(b7PopCallback,&b7);
  b8.attachPop(b8PopCallback,&b8);
  b9.attachPop(b9PopCallback,&b9);
  b6.attachPop(b6PopCallback,&b6);
  b10.attachPop(b10PopCallback,&b10);
  b4.attachPop(b4PopCallback,&b4);
  b5.attachPop(b5PopCallback,&b5);
  page0.attachPush(page0PushCallback);
  page1.attachPush(page1PushCallback);
  page2.attachPush(page2PushCallback);
  pinMode(relayLedPort, OUTPUT);
  pinMode(Ledport, OUTPUT);
  pinMode(relayRadio, OUTPUT);
  digitalWrite(relayLedPort, LOW);
  digitalWrite(relayRadio, LOW);
  pinMode(smokeA0, INPUT);
  radio.init();
  radio.set_frequency(106.9);
}

void loop() {
  delay(500);
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  if(CurrentPage==0){
    int analogSensor = analogRead(smokeA0);
      String temperatura = "t0.txt=\""+String(t,1)+"\"";
      String wilgotnosc = "t2.txt=\""+String(h,1)+"\"";
      Serial.print(temperatura); 
      Serial.write(0xff);
      Serial.write(0xff);
      Serial.write(0xff);
      Serial.print(wilgotnosc); 
      Serial.write(0xff);
      Serial.write(0xff);
      Serial.write(0xff);
      if (analogSensor > sensorThres){
        if(zle){
          t20.setText("Słaba");
          zle = false;
        }
      }else{
        if(!zle){
          t20.setText("Dobra");
          zle = true;
        }
      }
  }

   nexLoop(nex_listen_list);
}