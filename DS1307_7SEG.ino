#include "RTClib.h"
#include <Wire.h>
RTC_DS1307 rtc;
#define MAX_LED_USE_TIME      6
unsigned long last1s, last2ms; 
long time1 = 0 ;
char daysOfTheWeek[7][12] = {"CN", "T2", "T3", "T4", "T5", "T6", "T7"};
int Hour, Minute, Second, Day, Month, Year;
uint8_t ch_Hour, dv_Hour, ch_Minute, dv_Minute, ch_Second, dv_Second;
int latchPin = 8;
int clockPin = 12;
int dataPin = 11;
unsigned char Arr_Src[] = {
  0x80, 0x40, 0x20, 0x10, 0x08, 0x04 // Sang tu phai sang trai
};
unsigned char Arr_Src2[] = {
  0x04, 0x08, 0x10, 0x20, 0x40, 0x80 // Sang tu trai sang phai
};
unsigned char Arr_Data[] = {
  0xFC, 0x61, 0xDB, 0xF2, 0x67, 0xB7, 0xBE, 0xE0, 0xFF, 0xF7,  // 0 -> 9
};
char Time_7SEG[MAX_LED_USE_TIME+1]; // mảng lưu giá trị thời gian
void setup_RTC()
{
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
  }
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));//Ham cap nhat thoi gian
}
void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  Serial.begin(9600);
  setup_RTC();
  last1s = millis();
  last2ms = millis();
}
unsigned char i, j;
void loop() {
  if(millis() - last2ms >= 1)// task 2ms
  {
    last2ms = millis();
    Shiftbyte(Arr_Data[Time_7SEG[MAX_LED_USE_TIME-1-i]-0x30]);
    Shiftbyte(Arr_Src[i]);
    digitalWrite(latchPin, LOW);
    digitalWrite(latchPin, HIGH);
    i++;
    if(i >= MAX_LED_USE_TIME) i = 0;
  }
  if(millis() - last1s >= 1000)// task 1s
  {
      last1s = millis();
      Time();
  }
}

void Time()
{
  DateTime now = rtc.now();
  Hour   = now.hour();
  Minute = now.minute();
  Second = now.second();
  Day    = now.day();
  Month  = now.month();
  Year   = now.year();
  sprintf(Time_7SEG,"%.2d%.2d%.2d", Hour, Minute, Second); // chuyển sang kiểu kí tự
  Serial.println(String(Time_7SEG[0]) + "_" + String(Time_7SEG[1]) + "_" + String(Time_7SEG[2]) + "_" + String(Time_7SEG[3]));
}

void Shiftbyte(register byte dataOut)
{
  register unsigned char i;
  for (i = 0; i < 8; i++)
  {
    if (dataOut & 0x80)
    {
      digitalWrite(dataPin, 1);
    }
    else
    {
      digitalWrite(dataPin, 0);
    }
    digitalWrite(clockPin, 0);
    dataOut = dataOut << 1;
    digitalWrite(clockPin, 1);
  }
}
void ShiftOut(byte dataOut)
{
  unsigned char i;
  for (i = 0; i < 8; i ++)
  {
    if (dataOut & 0x01)
    {
      digitalWrite(dataPin, 1);
    }
    else {
      digitalWrite(dataPin, 0);
    }
    digitalWrite(clockPin, 0);
    dataOut = dataOut >> 1;
    digitalWrite(clockPin, 1);
  }
}
void SEG_7() {
  //Chạy tuần tự
  //  for(i = 0; i < 6; i++)
  //  {
  //    for(j = 0; j <= 9; j++)
  //    {
  //      Serial.println(i);
  //      Serial.println(j);
  //      delay(500);
  //      Shiftbyte(Array2[j]);
  //      Shiftbyte(Array[i]);
  //      digitalWrite(latchPin, LOW);
  //      digitalWrite(latchPin, HIGH);
  //      delay(4);
  //    }
  //  }
  //Chạy tuần tự
  digitalWrite(latchPin, LOW);
  Shiftbyte(Arr_Data[ch_Hour]);
  Shiftbyte(Arr_Src[5]);
  digitalWrite(latchPin, HIGH);
  delay(2);


  digitalWrite(latchPin, LOW);
  Shiftbyte(Arr_Data[dv_Hour]);
  Shiftbyte(Arr_Src[4]);
  digitalWrite(latchPin, HIGH);
  delay(2);

  digitalWrite(latchPin, LOW);
  Shiftbyte(Arr_Data[ch_Minute]);
  Shiftbyte(Arr_Src[3]);
  digitalWrite(latchPin, HIGH);
  delay(2);

  digitalWrite(latchPin, LOW);
  Shiftbyte(Arr_Data[dv_Minute]);
  Shiftbyte(Arr_Src[2]);
  digitalWrite(latchPin, HIGH);
  delay(2);

  digitalWrite(latchPin, LOW);
  Shiftbyte(Arr_Data[ch_Second]);
  Shiftbyte(Arr_Src[1]);
  digitalWrite(latchPin, HIGH);
  delay(2);

  digitalWrite(latchPin, LOW);
  Shiftbyte(Arr_Data[dv_Second]);
  Shiftbyte(Arr_Src[0]);
  digitalWrite(latchPin, HIGH);
  delay(2);
}
