
/*
 * простой робот
 * на гусеницах
 */

// двигатели

#define pinLeftEng     9
#define pinLeftEngFrd  10
#define pinLeftEngBck  8

#define pinRightEng     11
#define pinRightEngFrd  12
#define pinRightEngBck  13

#define ENG_MIN_SPEED  150
#define ENG_MAX_SPEED  250


// эхо локатор 

#define pinTring 7
#define pinEcho  6


enum Engine {enLeft, enRight};
enum EngineCommand {ENG_FORWARD, ENG_BACKWARD, ENG_STOP};

// состояние робота.
enum StateBotType {stSleep, stRun, stStop, stEscape, stSearch, stTest};

class Move {
  char Step;
  char BeginValue;
  char NextState;
  char CurrentState;
  char Forward();
};

char Move::Forward() {
  return stStop;
};

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

int Distance()
{
  unsigned long duration; 
  unsigned long sum = 0; 
  unsigned long arrDur[3];
  int mm = 0;
  char c = 0;

   // Serial.println("Distance = ");
  
  for(char i= 0; i < 3; i++)
  {
    digitalWrite(pinTring, LOW); 
    delay(10); 
    digitalWrite(pinTring, HIGH); 
    delayMicroseconds(10); 
    digitalWrite(pinTring, LOW); 
    duration = pulseIn(pinEcho, HIGH); 
        
    if (duration > 11600)
    {
      arrDur[i] = 0; 
    } 
    else 
    {
      c++;
      arrDur[i] = duration; 
    }
  }
  
  // все числа больше 2 местров
  if(c == 0)
  {
    mm =  2000;  
  }
  else
  {
    for (char i = 0; i < 3;  i++)
    {
      sum +=  arrDur[i];
    }
    mm = (sum/c)/5.8;
  }
   // Serial.println(mm);
  return mm;
}

void setup() {
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


  // инициализация портов для эхо локации
  pinMode(pinTring, OUTPUT); 
  pinMode(pinEcho,  INPUT); 

  digitalWrite(pinTring, LOW); 
  digitalWrite(pinEcho,  LOW); 
  
  // for debug 
   // Serial.begin (9600); 
}

StateBotType StateRobot = stSleep;
int dist;

// время работы поиска
unsigned long TimeSearch    = 0;

unsigned char SpeedEngRight = 0;
unsigned char SpeedEngLeft  = 0;
EngineCommand CommandEngLeft  = ENG_STOP;
EngineCommand CommandEngRight = ENG_STOP;

void loop() {
  
   // Serial.print("State = ");
   // Serial.println(StateRobot);

  switch(StateRobot) {
    case stRun:
       // Serial.println("Run");
      
      if((SpeedEngRight != ENG_MAX_SPEED) || (CommandEngRight != ENG_FORWARD))
      {
        // Serial.println("Run Right On");
        SpeedEngRight   = ENG_MAX_SPEED;
        CommandEngRight = ENG_FORWARD;
        ManagerEngine(enRight,  CommandEngRight, SpeedEngRight);
      }
      if((SpeedEngLeft != ENG_MAX_SPEED) || (CommandEngLeft != ENG_FORWARD))
      {
       // Serial.println("Run Left On");
        SpeedEngLeft = ENG_MAX_SPEED;
        CommandEngLeft = ENG_FORWARD;
        ManagerEngine(enLeft,  CommandEngLeft, SpeedEngLeft);
      }
      
      dist = Distance();
      
      if(dist <= 250)
      {
        StateRobot = stStop;
      }
      else if (dist >= 1000)
      {
        StateRobot = stSearch;
      }
      break;

    case stStop:
       // Serial.println("Stop");
      
      ManagerEngine(enLeft,  ENG_STOP, 0);
      SpeedEngLeft   = 0;
      CommandEngLeft = ENG_STOP;
      
      ManagerEngine(enRight, ENG_STOP, 0);
      SpeedEngRight   = 0;
      CommandEngRight = ENG_STOP;

      StateRobot = stSleep;
      break;
      
    case stSleep:
       // Serial.println("Sleep");
      
      dist = Distance();      
  
      if(dist < 120)
      {
        StateRobot = stEscape;
      } 
      else if((dist >= 300) && (dist < 1000))
      {
        StateRobot = stRun;
      }
      else 
      {
        delay(1000);
      }
      break;

    case stEscape:
       // Serial.println("Escape");

      dist = Distance();

      if(dist < 250)
      {
        if((SpeedEngRight != ENG_MAX_SPEED) || (CommandEngRight != ENG_BACKWARD))
        {
          // Serial.println("Run Right On");

          SpeedEngRight   = ENG_MAX_SPEED;
          CommandEngRight = ENG_BACKWARD;
          ManagerEngine(enRight,  CommandEngRight, SpeedEngRight);
        }
        if((SpeedEngLeft != ENG_MAX_SPEED) ||( CommandEngLeft != ENG_BACKWARD))
        {
          // Serial.println("Run Left On");
          
          SpeedEngLeft   = ENG_MAX_SPEED;
          CommandEngLeft = ENG_BACKWARD;
          ManagerEngine(enLeft,  CommandEngLeft, SpeedEngLeft);
        }
      }
      else if((dist >= 250) && (dist <500))
      {
        StateRobot = stStop;  
      }
      else 
      {
        StateRobot = stSearch;
      }
      break;
      
    case stSearch:
       // Serial.println("Search");

      if(TimeSearch == 0)
      {
        TimeSearch = millis();
      }
      
      if((SpeedEngRight != ENG_MIN_SPEED) || (CommandEngRight != ENG_FORWARD))
      {
        // Serial.println("Run Right On");
        
        SpeedEngRight   = ENG_MIN_SPEED;
        CommandEngRight = ENG_FORWARD;
        ManagerEngine(enRight,  CommandEngRight, SpeedEngRight);
      }
      if((SpeedEngLeft != ENG_MIN_SPEED) || (CommandEngLeft != ENG_BACKWARD))
      {
        // Serial.println("Run Left On");
                  
        SpeedEngLeft   = ENG_MIN_SPEED;
        CommandEngLeft = ENG_BACKWARD;
        ManagerEngine(enLeft,  CommandEngLeft, SpeedEngLeft);
      }
          
      dist = Distance();

      if(dist < 500)
      {
        StateRobot = stStop;
        TimeSearch = 0;
      }
      else if((millis() - TimeSearch) >= 3000 )
      {
        StateRobot = stStop;
        TimeSearch = 0;
      }
      break;
     
    case stTest:
      dist = Distance();   
      if (dist <= 120 )
      {
        digitalWrite(13, HIGH); 
      }
      else
      {
        digitalWrite(13, LOW); 
      }

      ManagerEngine(enRight,  ENG_FORWARD, 200);
      delay(1000);
      ManagerEngine(enRight,  ENG_FORWARD, 120);
      delay(1000);
      break;
  }
}
