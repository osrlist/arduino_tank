#include <Servo.h>

// двигатели

const char pinLeftEng    = 5;
const char pinLeftEngFrd = 6;
const char pinLeftEngBck = 7;

const char pinRightEng    = 9;
const char pinRightEngFrd = 10;
const char pinRightEngBck = 8;

const char ENG_MIN_SPEED = 150;
const char ENG_MAX_SPEED = 250;


const char PinServo = 2;
const char pinEcho  = 3;
const char pinTring = 4;

// управление двигателем 
enum Engine {enLeft, enRight};
enum EngineCommand {ENG_FORWARD, ENG_BACKWARD, ENG_STOP};


// текущий угол поварота радара
int CurAngle;


Servo ServoMotor;

int Distance()
{
  unsigned long duration; 
  int mm;

  digitalWrite(pinTring, LOW); 
  delay(10); 
  digitalWrite(pinTring, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(pinTring, LOW); 
  duration = pulseIn(pinEcho, HIGH); 
  
  mm = duration / 5.8;

  return mm;
}

void WriteAngle(int Angle)
{
  const float TimeToOneAngle = 3.0;
  ServoMotor.write(Angle);
  delay(int(TimeToOneAngle * abs(CurAngle - Angle)));
  CurAngle = Angle;
}

int Radar(void)
{
  static char Direction = 1; 
  int mm = 0;
  if (CurAngle >= 135) Direction *= (-1);
  if (CurAngle <= 45) Direction *= (-1);
  
  WriteAngle(CurAngle + 5 * Direction);
  mm = Distance();
  return mm;
}

void setup() {
 ServoMotor.attach(PinServo);
 CurAngle = 90;
 ServoMotor.write(CurAngle);
 delay(1000);  
 pinMode(LED_BUILTIN, OUTPUT);

 // инициализация портов для эхо локации
 pinMode(pinTring, OUTPUT); 
 pinMode(pinEcho,  INPUT); 

 digitalWrite(pinTring, LOW); 
 digitalWrite(pinEcho,  LOW); 
}

void loop() 
{
  digitalWrite(LED_BUILTIN, LOW); 
 /* if (Radar() <= 250) 
  {
     digitalWrite(LED_BUILTIN, HIGH);    
     delay(2000);  
  } */

}
