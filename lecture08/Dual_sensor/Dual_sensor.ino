#include <LinkedList.h>

#define dirRight 13
#define spRight 11
#define dirLeft 12
#define spLeft 6

#define latchPin 4
#define dataPin 7
#define clockPin 5

#define trigPin 3
#define echoPin A5
int distance;

#define forwardLED B00000010
#define uTurnLED   B00000100
#define leftLED    B00001000
#define rightLED   B00010000
#define stopLED    B00100000

#define rightIRS A2
#define leftIRS A4
#define frontIRS A3

int speedRight = 125;
int speedLeft = 115;

int leftIRSCountBlack = 0;
int rightIRSCountBlack = 0;
int frontIRSCountBlack = 0;
int leftIRSCountWhite = 0;
int rightIRSCountWhite = 0;
int frontIRSCountWhite = 0;
bool onNode = false;
bool commandFinished = false;
bool isDarkLeft = false;
bool isDarkRight = false;
bool isDarkFront = false;
bool wasDarkFront = false;

bool isBlocked = false;

int uTurnCount = 0;

LinkedList<String> commands;
String currentCommand = "";
String nextCommand;
bool hasSeenLineRight = false;
bool hasSeenLineLeft = false;

void setup() {
  // put your setup code here, to run once:
  pinMode(leftIRS, INPUT);
  pinMode(rightIRS, INPUT);
  pinMode(frontIRS, INPUT);
  
  pinMode(dirRight, OUTPUT);
  pinMode(dirLeft, OUTPUT);
  pinMode(spRight, OUTPUT);
  pinMode(spLeft, OUTPUT);

  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);

  pinMode(trigPin, OUTPUT); //sets pin as OUTPUT
  pinMode(echoPin, INPUT);  //sets pin as INPUT

  //commands.add("forward");
  //commands.add("left");
  //commands.add("forward");
  //commands.add("left");
  //commands.add("forward");
  //commands.add("right");
  commands.add("forward");
  //commands.add("uturn");
  //commands.add("forward");
  currentCommand = commands.shift();
  nextCommand = commands.get(0);
  
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println(currentCommand);
  
  //Serial.println(currentCommand);

  // Count how many times an IRS has been on black in a row
  if(digitalRead(leftIRS)){
    leftIRSCountBlack += 1;
    leftIRSCountWhite = 0;
  }else{
    leftIRSCountWhite += 1;
    leftIRSCountBlack = 0;
  }
  if(digitalRead(rightIRS)){
    rightIRSCountBlack += 1;
    rightIRSCountWhite = 0;
  }else{
    rightIRSCountWhite += 1;
    rightIRSCountBlack = 0;
  }
  if(digitalRead(frontIRS)){
    Serial.println("front");
    frontIRSCountBlack += 1;
    frontIRSCountWhite = 0;
  }else{
    frontIRSCountWhite += 1;
    frontIRSCountBlack = 0;
  }

  //If an IRS has been on black more than 3 times, it is considered as being on the line
  if(leftIRSCountBlack >= 1){
    isDarkLeft = true;
  }
  if(leftIRSCountWhite >= 1){
    isDarkLeft = false;
  }
  if(rightIRSCountBlack >= 1){
    isDarkRight = true;
  }
  if(rightIRSCountWhite >= 1){
    isDarkRight = false;
  }
  if(frontIRSCountBlack >= 10){
    wasDarkFront = isDarkFront;
    isDarkFront = true;
  }
  if(frontIRSCountWhite >= 10){
    wasDarkFront = isDarkFront;
    isDarkFront = false;
  }

  if(isDarkFront){
      onNode = true;
  }else{
    if(onNode){
      commandFinished = true;
      onNode = false;
    }
  }

  if(isBlocked){
    drive(0, 0, 0, 0);
  }else{
    
    if(currentCommand == "forward"){
      //too much to the right, turn left
      if(isDarkLeft && !isDarkRight){
        drive(0, 0, 1, speedRight);
      }
      // too much to the left, turn right
      else if(!isDarkLeft && isDarkRight){
        drive(1, speedLeft, 0, 0);
      }
      // at a cross section
      else if(commandFinished){
        commandCompleted();
      }
      // in the clear
      else if(!isDarkLeft && !isDarkRight){
          drive(1, speedLeft, 1, speedRight);
      } 
    }
    //turn right
    else if(currentCommand == "right"){
      if(isDarkRight){
        drive(1, speedLeft, 0, 0);
      }else{
        commandCompleted();
      }
    }
    //turn left
    else if(currentCommand == "left"){
      if(isDarkLeft){
        drive(0, 0, 1, speedRight);
      }else{
        commandCompleted();
      }
    }

    // "u'turn" (actually 3 point turn)
    else if(currentCommand == "uturn"){
      Serial.println(uTurnCount);
      if(isDarkFront){
        if(!wasDarkFront){
          uTurnCount += 1;
        }
      }else{
        if(uTurnCount >= 2){
          uTurnCount = 0;
          commandCompleted();
        }
      }
      if(uTurnCount <= 2){
        drive(1, speedLeft + 50, 0, speedRight);
      }
    }
  }

  //LED indicator for next command
  byte nextCommandByte;
  if(nextCommand == "forward"){
    nextCommandByte = forwardLED;
  }
  else if(nextCommand == "uturn"){
    nextCommandByte = uTurnLED;
  }
  else if (nextCommand == "right"){
    nextCommandByte = rightLED;
  }
  else if(nextCommand == "left"){
    nextCommandByte = leftLED;
  }else{
    nextCommandByte = stopLED;
  }
  if(commands.size() == 0 && currentCommand == ""){
    nextCommandByte = stopLED;
    drive(0,0,0,0);
  }


  // Stops the robot if there is something blocking it
  distance = getDistance();
  //Serial.println(nextCommand);
  if(distance <= 30){
    isBlocked = true;
  }else{
    isBlocked = false;
  }

  if(isBlocked && currentCommand != ""){
    writeToRegister(nextCommandByte | stopLED);
  }else{
    writeToRegister(nextCommandByte);
  }
  
}

bool commandCompleted(){
  drive(0,0,0,0);
  Serial.println(isDarkFront);
  Serial.println(onNode);
  Serial.println(frontIRSCountBlack);
  Serial.println(frontIRSCountWhite);
  Serial.println("Finished: " + currentCommand);
  currentCommand = commands.shift();
  nextCommand = commands.get(0);
  commandFinished = false;
  delay(1000);
}

// Write a byte to shift register
void writeToRegister(byte data) {  
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, data);
  digitalWrite(latchPin, HIGH);
}

void drive(boolean directionLeft, int speedLeft, boolean directionRight, int speedRight){
    analogWrite(spLeft, speedLeft);
    digitalWrite(dirLeft, directionLeft);
    analogWrite(spRight, speedRight);
    digitalWrite(dirRight, directionRight);
}

int getDistance() {
  //sends out a trigger sound
  digitalWrite(trigPin, LOW);
  delayMicroseconds(10);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  //returns the received echo in centimeter
  return pulseIn(echoPin, HIGH)* 0.034/2;
}
