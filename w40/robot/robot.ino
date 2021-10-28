#define dirA 12
#define spA 3
#define brA 9

#define dirB 13
#define spB 11
#define brB 8 

#define A 5
#define B 4
#define C 2

#define multiplexerIO A0

bool isDriving = true; //true = farwads | false = backwards
bool direction = true; //true = left | false = right

void setup() {
  // put your setup code here, to run once:
  pinMode(dirA, OUTPUT);
  pinMode(brA, OUTPUT);
  pinMode(dirB, OUTPUT);
  pinMode(brB, OUTPUT);
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(multiplexerIO, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  //forward
  if(readFromMulti(0,0,1)){
    //isDriving = true;
    drive(HIGH, 100, HIGH, 100, 0);
  }
  //Backwards
  if(readFromMulti(0,0,0)){
    //isDriving = true;
    drive(LOW, 100, LOW, 100, 0);
  }

  //Stop if no buttons are pressed
  if(readFromMulti(0,0,0) == 0 && readFromMulti(0,0,1) == 0){
    Serial.println("no drive");
    drive(HIGH, 0, HIGH, 0, 0);
  }
  
  //readFromMulti(0,0,0);
}

bool readFromMulti(bool c, bool b, bool a){
  digitalWrite(A, a);
  digitalWrite(B, b);
  digitalWrite(C, c);
  Serial.println(digitalRead(multiplexerIO));
  return digitalRead(multiplexerIO);
}

void drive(boolean leftDirection, int leftSpeed, boolean rightDirection, int rightSpeed) {
  if(isDriving){
    //Motor A (Left)
    if(leftDirection){
      forwardA(leftSpeed);
    }else{
      reverseA(leftSpeed);
    }
    
    //Motor B (Right)
    if(rightDirection){
      forwardB(rightSpeed);
    }else{
      reverseB(rightSpeed);
    }
  }
}

void forwardA(int sp) {
  digitalWrite(dirA, HIGH);
  digitalWrite(brA, LOW);
  analogWrite(spA, sp);
}

void forwardB(int sp) {
  digitalWrite(dirB, HIGH);
  digitalWrite(brB, LOW);
  analogWrite(spB, sp);
}

void reverseA(int sp) {
  digitalWrite(dirA, LOW);
  digitalWrite(brA, LOW);
  analogWrite(spA, sp);
}

void reverseB(int sp) {
  digitalWrite(dirB, LOW);
  digitalWrite(brB, LOW);
  analogWrite(spB, sp);
}

void brakeA() {
  digitalWrite(brA, HIGH);
}

void brakeB() {
  digitalWrite(brB, HIGH);
}
