#include <SoftPWM.h>
int dir = 0;
#define L2 A0
#define L1 A1
#define M A2
#define R1 A3
#define R2 A9

#define TL 1
#define TR 2
#define C 3
#define S 4

int nSpeed = 160;
int count = 0;
int previousPos = 3000;

int command[] = {C,C,TR,C,TR,TR,S};

byte sensor[] = {0, 0, 0, 0, 0};

void setup() 
{
  SoftPWMBegin();
  Serial.begin(115200);
  
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(4, OUTPUT);

  pinMode(5,INPUT);

  pinMode(L2, INPUT);
  pinMode(L1, INPUT);
  pinMode(M, INPUT);
  pinMode(R1, INPUT);
  pinMode(R2, INPUT);

//  delay(2000);
//  MotorR(1,nSpeed);
//  MotorL(1,nSpeed);

while(digitalRead(5) != 0)
{
  Serial.println(digitalRead(5));
}

}

void loop() 
{
  if (count>=7)
  {
    return;
  }
  readSensor();
  //Serial.println(count);
  if (sensor[0]==1 || sensor[4]==1)
  {
    if (command[count] == TR)
    {
      turnRight();
    }
    else if (command[count] == TL)
    {
      turnLeft();
    }
    else if (command[count] == C)
    {
      MotorL(1,nSpeed);
      MotorR(1,nSpeed);
      delay(100);
    }
    else if (command[count] == S)
    {
      MotorL(1,0);
      MotorR(1,0);
      count++;
      return;
    }
    count++;
  }
  
  int Position = pos();
  if (Position == 0){
    MotorR(1,0);
    MotorL(1,0);
    return ;
  }

  int difference = 3000 - Position;
  //Serial.println(difference);
  if (difference > 0){
    int adj = nSpeed-(difference*0.2);
    if (adj<0)
    {
      adj = 0;
    }
    MotorL(1,adj);
    MotorR(1,nSpeed);
    dir = 1;
  }
  else if (difference < 0 ){
    int adj = nSpeed-(abs(difference)*0.2);
    if (adj<0)
    {
      adj = 0;
    }
    MotorR(1,adj);
    MotorL(1,nSpeed);
    dir = -1;
  }
  else{
    MotorR(1,nSpeed);
    MotorL(1,nSpeed);
    dir = 0;
  }

}

int pos()
{
  int a = 0;
  readSensor();
  int sum = sensor[1]+sensor[2]+sensor[3];
  if (sum == 0)
  {
    if (dir == 0)
    {
      return 0;
    }
    else if (dir == 1)
    {
      a = 5000;
    }
    else if (dir == -1)
    {
      a = 1000;
    }

  }
  else
  {
    a = (sensor[1]*1000 + sensor[2]*3000 + sensor[3]*5000)/sum;
  }
  if (a != previousPos)
  {
    int tmp = a;
    a = (a+previousPos)/2;
    previousPos = tmp;
  }
  return a;
}

void MotorL(int dir, int spd)
{
  digitalWrite(3, dir);
  SoftPWMSet(7,spd);
}

void MotorR(int dir, int spd)
{
  digitalWrite(2, dir);
  SoftPWMSet(4,spd);
}

void readSensor()
{
  sensor[0] = analogRead(L2) < 300 ? 1 : 0;
  sensor[1] = analogRead(L1) < 400 ? 1 : 0;
  sensor[2] = analogRead(M) < 600 ? 1 : 0;
  sensor[3] = analogRead(R1) < 400 ? 1 : 0;
  sensor[4] = analogRead(R2) < 250 ? 1 : 0;

//  Serial.print(sensor[0]);
//  Serial.print(sensor[1]);
//  Serial.print(sensor[2]);
//  Serial.print(sensor[3]);
//  Serial.print(sensor[4]);
//  Serial.println();

  Serial.print(analogRead(L2));
  Serial.print(" ");
  Serial.print(analogRead(L1));
  Serial.print(" ");
  Serial.print(analogRead(M));
  Serial.print(" ");
  Serial.print(analogRead(R1));
  Serial.print(" ");
  Serial.print(analogRead(R2));
  Serial.println();
}

void turnLeft()
{
  readSensor();
  MotorL(1,0);
  MotorR(1,0);
  delay(100);

  int state = 0;
  
  while (true)
  {
    MotorL(0,150);
    MotorR(1,150);
    readSensor();
    if (state == 0) //wait until white
    {
      if (sensor[1]==0 && sensor[2]==0 && sensor[3]==0)
      {
        state = 1;
      }
    }
    else if (state == 1)
    {
      if (sensor[2] == 1)
      {
        state = 2;
      }
    }
    else if (state == 2)
    {
      break;
    }
  }

  MotorL(1,0);
  MotorR(1,0);

}

void turnRight()
{
  readSensor();
  MotorL(1,0);
  MotorR(1,0);
  delay(100);

  int state = 0;
  
  while (true)
  {
    MotorL(1,150);
    MotorR(0,150);
    readSensor();
    if (state == 0) //wait until white
    {
      if (sensor[1]==0 && sensor[2]==0 && sensor[3]==0)
      {
        state = 1;
      }
    }
    else if (state == 1)
    {
      if (sensor[2] == 1)
      {
        state = 2;
      }
    }
    else if (state == 2)
    {
      break;
    }
  }

  MotorL(1,0);
  MotorR(1,0);

}


