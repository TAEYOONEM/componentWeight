unsigned char cmd = 0;
volatile bool motorFlag = false;
volatile bool reverseFlag = false;

//  스텝모터
const int PUL1=26;
const int DIR1=25;
const int ENA1=24;

const int PUL_Z=42;
const int DIR_Z=41;
const int ENA_Z=40;
long Step_Pos_Z = 0;


//  릴레이
const int Relay_small = 4;
const int Relay_big = 5;
const int Relay_vacuum = 6;
const int Relay_v1 = 30;
const int Relay_v2 = 31;
const int Relay_v3 = 32;
const int Relay_v4 = 34;


//  사용함수
/////////////////////////////////////////////////////
void Zero_point();                          //  피스톤들 초기위치잡는 함수
void RELAY(int piston, bool zero_or_one);   //  piston에 원하는 피스톤, 0=LOW 1=HIGH
  //  small은 저울 위아래, big은 저울 앞뒤, vacuum은 그리퍼
  //  Relay_small은 HIGH가 상승, Relay_big은 LOW가 전진, Relay_vacuum은 LOW가 잡기
void motor_step(int mm, int SPEED);         //  mm에 원하는 거리, SPEED에 속도(25~125)
void Z_step_motor(int mm);


void setup() {
  //  스텝모터
  pinMode (PUL1, OUTPUT);
  pinMode (DIR1, OUTPUT);
  pinMode (ENA1, OUTPUT);
  pinMode (PUL_Z, OUTPUT);
  pinMode (DIR_Z, OUTPUT);
  pinMode (ENA_Z, OUTPUT);
 
  //  릴레이
  pinMode(Relay_small,OUTPUT);
  pinMode(Relay_big,OUTPUT);
  pinMode(Relay_vacuum,OUTPUT);
  pinMode(Relay_v1,OUTPUT);
  pinMode(Relay_v2,OUTPUT);
  pinMode(Relay_v3,OUTPUT);
  pinMode(Relay_v4,OUTPUT);

  Zero_point();
  digitalWrite(PUL1,HIGH);

  Serial.begin(9600);
  Serial.println("g");
}

void loop() {
      if(motorFlag == true){
      Z_step_motor(-205);
      delay(1000);
      RELAY(Relay_vacuum,0);
      delay(1000);
      Z_step_motor(205);
      delay(500);
      RELAY(Relay_big,0);
      delay(1000);
      Z_step_motor(-10);
      delay(500);
      RELAY(Relay_vacuum,1);
      delay(1000);
      Z_step_motor(10);
      RELAY(Relay_small,0);
      delay(1000);
      Serial.println("e");
      motorFlag = false;
    }

    else if(reverseFlag == true){
      RELAY(Relay_small,1);
      delay(1000);
      Z_step_motor(-10);
      delay(1000);
      RELAY(Relay_vacuum,0);
      delay(1000);
      Z_step_motor(10);
      delay(1000);
      RELAY(Relay_big,1);
      delay(1000);
      Z_step_motor(-205);
      RELAY(Relay_vacuum,1);
      delay(1000);
      Z_step_motor(205);
      Serial.println("e");
      reverseFlag = false;
     }
}


void serialEvent(){
  if(Serial.available()){
    cmd = (char)Serial.read();
      switch (cmd){        
        case 'm' :
          // 1111111111
          motorFlag = true;
          break;
        case 'r' :
          // 1111111111
          reverseFlag = true;
          break;
          }          
   } 
}
void Zero_point()
{
  RELAY(Relay_big,1);
  RELAY(Relay_small,1);
  RELAY(Relay_vacuum,1);
  digitalWrite(DIR_Z,LOW);
  digitalWrite(ENA_Z,HIGH);  
  digitalWrite(PUL_Z,HIGH);
  delayMicroseconds(75);
  digitalWrite(PUL_Z,LOW);
  delayMicroseconds(75);
}

void RELAY(int piston,bool zero_or_one)
{
  if(zero_or_one ==1)
  {
    digitalWrite(piston,HIGH);
  }
  else
  {
    digitalWrite(piston,LOW);
  }
}



void motor_step(int mm, int SPEED)
{
  long iter = abs(mm * 108.70);
  if(mm >= 0)
  {
    digitalWrite(DIR1,HIGH);
    digitalWrite(ENA1,HIGH);
    for (int i=0; i<iter; i++)
    {
      digitalWrite(PUL1,HIGH);
      delayMicroseconds(SPEED);
      digitalWrite(PUL1,LOW);
      delayMicroseconds(SPEED);
      Step_Pos_Z = Step_Pos_Z + 1;
    }
  }
  else
  {
    digitalWrite(DIR1,LOW);
    digitalWrite(ENA1,HIGH);
    for (int i=0; i<iter; i++)
    {
      digitalWrite(PUL1,HIGH);
      delayMicroseconds(SPEED);
      digitalWrite(PUL1,LOW);
      delayMicroseconds(SPEED);
      Step_Pos_Z = Step_Pos_Z - 1;
    }
  }
}

void Z_step_motor(int mm)
{
  int iter = abs(mm * 108.70);
  if(mm >= 0)
  {

      digitalWrite(DIR_Z,HIGH);
      digitalWrite(ENA_Z,HIGH);

    for (int i=0; i<iter; i++)
    {
//      if(Step_Pos_Z>=0)
//      {
//        break;
//      }
      if(i <= iter/2)
      {
        digitalWrite(PUL_Z,HIGH);
        delayMicroseconds(max((500-i),50));
        digitalWrite(PUL_Z,LOW);
        delayMicroseconds(max((500-i),50));
//        Step_Pos_Z = Step_Pos_Z + 1;
      }
      else
      {
        int j = iter - i;
        digitalWrite(PUL_Z,HIGH);
        delayMicroseconds(max((500-j),50));
        digitalWrite(PUL_Z,LOW);
        delayMicroseconds(max((500-j),50));
//        Step_Pos_Z = Step_Pos_Z + 1;
        
      }
    }
  }
  else
  {
    digitalWrite(DIR_Z,LOW);
    digitalWrite(ENA_Z,HIGH);    
    for (int i=0; i<iter; i++)
    {
      if(i <= iter/2)
      {
        digitalWrite(PUL_Z,HIGH);
        delayMicroseconds(max((500-i),50));
        digitalWrite(PUL_Z,LOW);
        delayMicroseconds(max((500-i),50));
//        Step_Pos_Z = Step_Pos_Z - 1;
      }
      else
      {
        int j = iter - i;
        digitalWrite(PUL_Z,HIGH);
        delayMicroseconds(max((500-j),50));
        digitalWrite(PUL_Z,LOW);
        delayMicroseconds(max((500-j),50));
//        Step_Pos_Z = Step_Pos_Z - 1;
      }
    }
  }
}
