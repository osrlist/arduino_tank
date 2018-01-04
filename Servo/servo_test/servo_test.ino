#include <Servo.h>

const char PinServo = 2;
const char pinEcho  = 4;
const char pinTring = 3;



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


void setup() {
  // put your setup code here, to run once:
 ServoMotor.attach(PinServo);
 pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  static int CurAngle = 0;
  digitalWrite(LED_BUILTIN, LOW); 
  ServoMotor.write(CurAngle);
  delay(1000);  
  CurAngle = 180;
  
  ServoMotor.write(CurAngle);
  while (ServoMotor.read() <= (CurAngle-10))
  {
    delay(10);  
  }
  digitalWrite(LED_BUILTIN, HIGH); 
  
 /* 
  ServoMotor.write(180);
  delay(350);
  */
 
}
