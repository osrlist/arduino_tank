#include <Servo.h>

// двигатели

const char pinLeftEng    = 5;
const char pinLeftEngFrd = 7;
const char pinLeftEngBck = 6;

const char pinRightEng    = 11;
const char pinRightEngFrd = 8;
const char pinRightEngBck = 9;

const char ENG_MIN_SPEED = 150;
const char ENG_MAX_SPEED = 250;


const char PinServo = 3;
const char pinEcho  = 2;
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


void ManagerEngine(Engine en, EngineCommand cmd, unsigned char Speed)
{
  char pinEn  = 0;
  char pinFrd = 0;
  char pinBck = 0;
  
  switch(en) {
    case enLeft:
      pinEn  = pinLeftEng;
      pinFrd = pinLeftEngFrd;
      pinBck = pinLeftEngBck;
      break;
    case enRight:
      pinEn  = pinRightEng;
      pinFrd = pinRightEngFrd;
      pinBck = pinRightEngBck;
      break;
  };

  switch(cmd) {
    case ENG_FORWARD:
      digitalWrite(pinFrd, HIGH); 
      digitalWrite(pinBck, LOW); 
      analogWrite(pinEn, Speed);
      break;
    case ENG_BACKWARD:
      digitalWrite(pinFrd, LOW); 
      digitalWrite(pinBck, HIGH); 
      analogWrite(pinEn, Speed);
      break;
    case ENG_STOP:
      digitalWrite(pinFrd, LOW); 
      digitalWrite(pinBck, LOW); 
      analogWrite(pinEn,     0);
      break;
  };
}


void setup() {
 ServoMotor.attach(PinServo);
 pinMode(LED_BUILTIN, OUTPUT);

 CurAngle = 90;
 ServoMotor.write(CurAngle);
 delay(1000);  

 
 // инициализация портов для эхо локации
 pinMode(pinTring, OUTPUT); 
 pinMode(pinEcho,  INPUT); 

 digitalWrite(pinTring, LOW); 
 digitalWrite(pinEcho,  LOW); 

  // инициализация портов для двигателей
 pinMode(pinLeftEng,     OUTPUT);  
 pinMode(pinLeftEngFrd,  OUTPUT);  
 pinMode(pinLeftEngBck,  OUTPUT);  
 pinMode(pinRightEng,    OUTPUT);  
 pinMode(pinRightEngFrd, OUTPUT);  
 pinMode(pinRightEngBck, OUTPUT);  

 digitalWrite(pinLeftEng,     LOW);
 digitalWrite(pinLeftEngFrd,  LOW);  
 digitalWrite(pinLeftEngBck,  LOW);  
 digitalWrite(pinRightEng,    LOW);  
 digitalWrite(pinRightEngFrd, LOW);  
 digitalWrite(pinRightEngBck, LOW);  
}

void loop() 
{
  digitalWrite(LED_BUILTIN, LOW); 


 ManagerEngine(enLeft, ENG_FORWARD, 128);
 
  
  if (Radar() <= 250) 
  {
     digitalWrite(LED_BUILTIN, HIGH);    
     delay(2000);  
  } 

}
