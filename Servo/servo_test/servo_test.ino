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
const char HeadForword = 80;


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

int SmatrDistance() {
  const int CountTry = 5;
  int sum = 0;
  int result = 0;
  for (int i = 0; i < CountTry; i++ ) {
    sum =+ Distance();
  }
  result = sum / CountTry;
  return result;
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
  mm = SmatrDistance();
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

 CurAngle = HeadForword;
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

int mm = 0;

/* mm = Radar();
 if (mm > 100) // 10 cm 
 {
   ManagerEngine(enLeft,  ENG_FORWARD, ENG_MIN_SPEED);
   ManagerEngine(enRight, ENG_FORWARD, ENG_MIN_SPEED);
 }
 else
 {
   ManagerEngine(enLeft,  ENG_STOP, 0);
   ManagerEngine(enRight, ENG_STOP, 0);
 }

*/


 /* ManagerEngine(enLeft, ENG_FORWARD, 128);
 ManagerEngine(enRight, ENG_FORWARD, 128);
*/

 /*digitalWrite(LED_BUILTIN, LOW); 

 delay(10);  
 mm = Distance();

 if (mm < 250)
 {
   digitalWrite(LED_BUILTIN, HIGH);   
   delay(2000);  
 }
*/
 /*
  if (Radar() <= 250) 
  {
     digitalWrite(LED_BUILTIN, HIGH);    
     delay(2000);  
  } 
*/

  //digitalWrite(LED_BUILTIN, LOW); 

void Blinc(int frequency) {
  for (int i = 0; i < 3; i++) {
    digitalWrite(LED_BUILTIN, LOW); 
    delay(frequency);      
    digitalWrite(LED_BUILTIN, HIGH);
    delay(frequency);         
  }
  digitalWrite(LED_BUILTIN, LOW); 
}
 
  
void loop() 
{
  static int min_distance = 1000;
  static int count_route_in_loop = 0;

  if (count_route_in_loop < 36 ) {
    int distance = Radar();
    if ( distance < min_distance ) {
      min_distance = distance;
    }
      count_route_in_loop++;
  } else {
//    Blinc( 100 );    
    count_route_in_loop = 0;
    if (min_distance > 70 ) { // 70 cm
//      Blinc( 100 );    
      ManagerEngine(enLeft,  ENG_FORWARD, ENG_MIN_SPEED + 30);
      ManagerEngine(enRight, ENG_FORWARD, ENG_MIN_SPEED + 30);
      min_distance = 0;
      delay(300);  
      ManagerEngine(enLeft,  ENG_STOP, 0);
      ManagerEngine(enRight, ENG_STOP, 0);
      count_route_in_loop = 0;
    }
    min_distance = 1000;
  }
}
