#define AIN1 P8_3
#define AIN2 P1_5
#define BIN1 P1_6
#define BIN2 P1_4
#define FORWARD 0   //正方向标记
#define BACKWARD 1  //反方向标记

#define TRACK_LEFT1 P5_1  
#define TRACK_LEFT2 P5_0  
#define TRACK_RIGHT1 P5_2  
#define TRACK_RIGHT2 P5_3  

#define buttonpin P2_5

#include "LCD_Launchpad.h"

Servo myservo;
IRrecv irrecv(P2_7);
decode_results results;
LCD_LAUNCHPAD myLCD;
int L1,L2,R1,R2;
int pos = 0;

void RightMoto(int sp, int dr); //右侧电机驱动函数，sp为转速，0-255范围, dr为转动方向，1为前，0为后（前后方向可根据自己的结构搭建情况自定义）
void LeftMoto(int sp, int dr);  //左侧电机驱动函数，sp为转速，0-255范围, dr为转动方向，1为前，0为后（前后方向可根据自己的结构搭建情况自定义）
void xunji();
void brake();
void run();

void setup() {
  myLCD.init();
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(P1_2, INPUT);
  pinMode(TRACK_LEFT1, INPUT_PULLUP);   
  pinMode(TRACK_LEFT2, INPUT_PULLUP);   
  pinMode(TRACK_RIGHT1, INPUT_PULLUP);   
  pinMode(TRACK_RIGHT2, INPUT_PULLUP);
  pinMode(buttonpin, INPUT_PULLUP);
}
 
void loop()
{
  if (digitalRead(buttonpin)==LOW)
    {
      while (digitalRead(buttonpin)==LOW)//避障,若前方有障碍物自动停下,直到障碍物被移除
        {
          RightMoto(0,FORWARD);  //设置右侧电机转速为100
          LeftMoto(0,FORWARD);
        }
      run(60,60,200);
      xunji();
    }
  xunji();  
}
void xunji()
{
  L1= digitalRead(TRACK_LEFT1);
  L2= digitalRead(TRACK_LEFT2);
  R1= digitalRead(TRACK_RIGHT1);
  R2= digitalRead(TRACK_RIGHT2);
  myLCD.print(L2);myLCD.print(L1); myLCD.print(R1); myLCD.println(R2);
  if (L2==1&&L1==1&&R1==1&&R2==1) return;
  if (line()) return;
  else if (L2==0&&L1==0&&R1==0&&R2==0)
    {
      brake();
      myLCD.print(2);myLCD.print(3); myLCD.print(3); myLCD.println(3);
      delay(500);
      run(60,60,500);
    }
  else if (L2==0)//&&R1==0&&R2==1
    {
      run(0,60,200);
      L1= digitalRead(TRACK_LEFT1);
      L2= digitalRead(TRACK_LEFT2);
      R1= digitalRead(TRACK_RIGHT1);
      R2= digitalRead(TRACK_RIGHT2);
      myLCD.print(L2);myLCD.print(L1); myLCD.print(R1); myLCD.println(R2);
      run(60,60,200);
      if (!(L2==0&&L1==0&&R1==0&&R2==0))
        {//run(60,60,200);
        run(0,0,500);
        run(0,60,1000);
        run(0,0,500);}
      else
        {
          RightMoto(60,BACKWARD);
          LeftMoto(60,BACKWARD);
          delay(200);
        }
    }
  else if (R2==0)//L2==1&&L1==0&&
    {
      run(0,60,200);
      L1= digitalRead(TRACK_LEFT1);
      L2= digitalRead(TRACK_LEFT2);
      R1= digitalRead(TRACK_RIGHT1);
      R2= digitalRead(TRACK_RIGHT2);
      myLCD.print(L2);myLCD.print(L1); myLCD.print(R1); myLCD.println(R2);
      run(60,60,200);
      if (!(L2==0&&L1==0&&R1==0&&R2==0))
        {//run(60,60,200);
         run(0,0,500);
         run(60,0,1000);
         run(0,0,500);}
      else
        {
          RightMoto(60,BACKWARD);
          LeftMoto(60,BACKWARD);
          delay(200);
        }
    }
    //else if (line()) return;
    //line();
}
bool line()
{
  L1= digitalRead(TRACK_LEFT1);
  L2= digitalRead(TRACK_LEFT2);
  R1= digitalRead(TRACK_RIGHT1);
  R2= digitalRead(TRACK_RIGHT2);
  myLCD.print(L2);myLCD.print(L1); myLCD.print(R1); myLCD.println(R2);
  if (L1==0&&R1==0) {run(60,60,1); return true;}
  else if (L1==0&&R1==1)
    {
      while(!(L1==0&&R1==0))
        {
          L1= digitalRead(TRACK_LEFT1);
          L2= digitalRead(TRACK_LEFT2);
          R1= digitalRead(TRACK_RIGHT1);
          R2= digitalRead(TRACK_RIGHT2);
          myLCD.print(L2);myLCD.print(L1); myLCD.print(R1); myLCD.println(R2);
          run(0,60,1);
        }
       return true;
    }
  else if (L1==1&&R1==0)
    {
      while(!(L1==0&&R1==0))
        {
          L1= digitalRead(TRACK_LEFT1);
          L2= digitalRead(TRACK_LEFT2);
          R1= digitalRead(TRACK_RIGHT1);
          R2= digitalRead(TRACK_RIGHT2);
          myLCD.print(L2);myLCD.print(L1); myLCD.print(R1); myLCD.println(R2);
          run(60,0,1);
        }
      return true;
    }
}
void run(int l,int r,int t)//run(左电机转速,右电机转速,转动时间)
{
  LeftMoto(l,FORWARD);
  RightMoto(r,FORWARD);
  delay(t);
}
void brake()
{
  digitalWrite(AIN1,HIGH);
  digitalWrite(AIN2,HIGH);
  digitalWrite(BIN1,HIGH);
  digitalWrite(BIN2,HIGH);
  delay(500);
}
void RightMoto(int sp,int dr)
{
  if(sp<0) sp=0;
  if(sp>255) sp=255;  //防止输入范围超限
  if(dr==FORWARD) analogWrite(AIN1,sp); //设置右侧电机转速
  if(dr==BACKWARD) analogWrite(AIN1,255-sp);
  digitalWrite(AIN2,dr);  //设置右侧电机方向
}
void LeftMoto(int sp, int dr)
{
  if(sp<0) sp=0;
  if(sp>255) sp=255;  //防止输入范围超限
  if(dr==FORWARD) analogWrite(BIN1,sp); //设置左侧电机转速     
  if(dr==BACKWARD) analogWrite(BIN1,255-sp); //设置左侧电机转速
  digitalWrite(BIN2,dr);   //设置左侧电机方向
}
