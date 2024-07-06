#include <Arduino_FreeRTOS.h>
#include <IRremote.h>
#include <semphr.h>

#define Pin 3
#define POWER 0x00FF629D
#define A 0x00FF22DD
#define B 0x00FF02FD
#define C 0x00FFC23D
#define UP 0x00FF9867
#define DOWN 0x00FF38C7
#define LEFT 0x00FF30CF
#define RIGHT 0x00FF7A85
#define SELECT 0x00FF18E7

IRrecv IR_recve(Pin);
decode_results results;

int RW = 10;
int RWforward = 9;
int RWbackward = 8;
int LWforward = 7;
int LWbackward = 6;
int LW = 5;

const int trig = 2;
const int echo = A0;
long timing;
int distance; // distance
int limit = 20; // minimum distance to allow the car movement
int speed = 100; // speed of the car 
int block = 0; // flag to stop the car 
int upp = 0;



void setup()
{
  pinMode(LWbackward, OUTPUT);
  pinMode(LWforward, OUTPUT);
  pinMode(RWbackward, OUTPUT);
  pinMode(RWforward, OUTPUT);
  pinMode(RW, OUTPUT);
  pinMode(LW, OUTPUT);  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT); 
  Serial.begin(9600);
  IR_recve.enableIRIn();

}

void check()
{
  if (distance < limit)
    block = true;
  else
    block = false;
}

void getDistance()
{
  digitalWrite(trig, LOW);
  delayMicroseconds(10);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  timing = pulseIn(echo, HIGH);
  distance = timing * 0.034 / 2;
  Serial.print("Distance: ");
  Serial.println(distance);
}


void readIR()
{
    getDistance();
    check();
    if(block && upp)
    {
     Stop();
    }
    if(IR_recve.decode(&results))
     {
      Serial.println(results.value);
      switch (results.value)     
       {
          case UP:        // FORWARD Button
            Forward();
            Serial.println("up");
            upp = 1;
            break;

          case LEFT:      // LEFT Button
            Left();
            upp = 0;
            break;

          case RIGHT:     // RIGHT Button
            Right();
            upp = 0;
            break;

          case DOWN:      // BACK Button
            Back();
            Serial.println("down");
            upp = 0;
            break;

          case A:
            speed = 75;  // A 30%
            break;

          case B:
            speed = 150; // B 60%
            break;

          case C:
           speed = 250; // C 100%
           break;

         case SELECT:    // STOP Button
            Stop();
           break;
      }
      IR_recve.resume(); 
 }
}
  


void loop()
{
  readIR();
}

void Back()
{
  analogWrite(LW, speed);
  analogWrite(RW, speed);
  digitalWrite(LWbackward, HIGH);
  digitalWrite(RWbackward, HIGH);
  digitalWrite(LWforward, LOW);
  digitalWrite(RWforward, LOW);
}

void Forward()
{
  if (block && upp)
  {
    Stop();
  }
  else
  {
    analogWrite(LW, speed);
    analogWrite(RW, speed);
    digitalWrite(LWforward, HIGH);
    digitalWrite(RWforward, HIGH);
    digitalWrite(LWbackward, LOW);
    digitalWrite(RWbackward, LOW);
  }
}

void Left()
{
  digitalWrite(RW, HIGH);
  digitalWrite(LWforward, LOW);
  digitalWrite(RWforward, HIGH);
  //delay(100);
}

void Right()
{
  digitalWrite(LW, HIGH);
  digitalWrite(RWforward, LOW);
  digitalWrite(LWforward, HIGH);
  //delay(100);
}

void Stop()
{
  digitalWrite(LW, LOW);
  digitalWrite(RW, LOW);
  digitalWrite(LWbackward, LOW);
  digitalWrite(LWforward, LOW);
  digitalWrite(RWbackward, LOW);
  digitalWrite(RWforward, LOW);
}
