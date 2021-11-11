const byte ledPin = 4;
const byte interruptPinA = 3;
const byte interruptPinB = 2;
volatile byte state = LOW;
volatile int numa = 0;
volatile int numb = 0;
volatile bool AIsDriving = true;
volatile bool BIsDriving = true;
int list [4] = {0, 1, 0, 1};a
// forward = 0
// left = 1
// right = 2


#define dirB 13
#define spB 11
#define dirA 12
#define spA 6

void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin, OUTPUT);
  pinMode(interruptPinA, INPUT_PULLUP);
  pinMode(interruptPinB, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPinA), incrementA, RISING);
  attachInterrupt(digitalPinToInterrupt(interruptPinB), incrementB, RISING);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int command : list){
    switch(command){
      case 0:
        forward();
        break;
       case 1:
        left();
        break;
    }
  }
  STOP();
    
}

void incrementA(){
  numa += 1;
}

void incrementB(){
  numb += 1;

}
// ||
void forward(){
  drive(40, 40);
}

void left(){
  drive(40, 0);
}

void right(){
  drive(0, 40);
}

// ||
void drive(int ticksA, int ticksB){
  while(numa <= ticksA || numb <= ticksB){
    if(numa < ticksA){
      analogWrite(spA, 120);
      digitalWrite(dirA, HIGH);
      
    }else{
      analogWrite(spA, 0);
    }
    if(numb < ticksB){
      analogWrite(spB, 120);
      digitalWrite(dirB, HIGH);
      
    }else{
      analogWrite(spB, 0);
    }
    
  }
  analogWrite(spA, 0);
  analogWrite(spB, 0);
  numa = 0;
  numb = 0;
}

void STOP(){
  analogWrite(spA, 0);
  analogWrite(spB, 0);
}
