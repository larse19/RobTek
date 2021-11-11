#include <LinkedList.h>

#define latchPin 4
#define dataPin 7
#define clockPin 5
//#define clearPin 6

const byte interruptPinA = 3;
const byte interruptPinB = 2;

#define dirA 12
#define spA 6
#define dirB 13
#define spB 11

#define A 8
#define B 9
#define C 10

#define multiplexerIO A5

volatile int numA = 0;
volatile int numB = 0;

bool readFromMulti(int chnl){
  int a = bitRead(chnl,0); //Take first bit from binary value of i channel.
  int b = bitRead(chnl,1); //Take second bit from binary value of i channel.
  int c = bitRead(chnl,2); //Take third bit from value of i channel.
  
  digitalWrite(A, a);
  digitalWrite(B, b);
  digitalWrite(C, c);
  //Serial.println(digitalRead(multiplexerIO));
  return digitalRead(multiplexerIO);
}

class Button {
    private:
        bool _state;
        uint8_t _pin;

    public:
        Button(uint8_t pin) : _pin(pin) {}

        void begin() {
            _state = readFromMulti(_pin);
        }

        bool isReleased() {
            bool v = readFromMulti(_pin);
            if (v != _state) {
                _state = v;
                if (_state) {
                    return true;
                }
            }
            return false;
        }
};

Button forwardButton(0);
Button reverseButton(1);
Button leftButton(2);
Button rightButton(3);
Button playButton(4);
Button pauseButton(5);
Button stopButton(6);

LinkedList<int> commands;
String state = "input";
int commandIndex = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(multiplexerIO, INPUT_PULLUP);
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  
  forwardButton.begin();
  reverseButton.begin();
  leftButton.begin();
  rightButton.begin();
  playButton.begin();
  pauseButton.begin();
  stopButton.begin();

  pinMode(interruptPinA, INPUT_PULLUP);
  pinMode(interruptPinB, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPinA), incrementA, RISING);
  attachInterrupt(digitalPinToInterrupt(interruptPinB), incrementB, RISING);
  Serial.begin(9600);
}


void loop() {
  // Input state
  if(state == "input"){
    if(forwardButton.isReleased()){
      commands.add(0);
      Serial.println("forward pressed");
    }
    if(reverseButton.isReleased()){
      commands.add(1);
      Serial.println("reverse pressed");
    }
    if(leftButton.isReleased()){
      commands.add(2);
      Serial.println("left pressed");
    }
    if(rightButton.isReleased()){
      commands.add(3);
      Serial.println("right pressed");
    }
    
    if(playButton.isReleased()){
      state = "active";
      Serial.println("play pressed");
    }


  // Active state
  }else if(state == "active"){
    if (commands.size() > 0){
      doCommand(commands.shift());
    }else{
      state = "input";
      commandIndex = 0;
      Serial.println("input mode:");
      Serial.println(commands.size());
    }

   
  // Pause state
  }else if(state == "pause"){
    Serial.println("paused");
    if(playButton.isReleased()){
       state = "active";
      }
    if(stopButton.isReleased()){
      state = "input";
      commands.clear();
    }
  }
    //small delay to avoid bouncing
    delay(50);
}

void doCommand(int command){
  switch(command){
      case 0:
        forward();
        break;
       case 1:
        reverse();
        break;
       case 2:
        left();
        break;
       case 3:
        right();
        break;
    }
}

// General drive function with pause/stop listener
void drive(int ticksA, boolean directionA, int ticksB, boolean directionB){
  while(numA <= ticksA || numB <= ticksB){
    if(numA < ticksA){
      analogWrite(spA, 120);
      digitalWrite(dirA, directionA);
      
    }else{
      analogWrite(spA, 0);
    }
    if(numB < ticksB){
      analogWrite(spB, 120);
      digitalWrite(dirB, directionB);
      
    }else{
      analogWrite(spB, 0);
    }

    if(pauseButton.isReleased()){
        state = "pause";
    }
    if(stopButton.isReleased()){
      state = "input";
      commands.clear();
    }
    
  }
  analogWrite(spA, 0);
  analogWrite(spB, 0);
  numA = 0;
  numB = 0;
}

// Incrementers for tachometer
void incrementA(){
  numA += 1;
}

void incrementB(){
  numB += 1;

}

// Directional drive functions
void forward(){
  Serial.println("forward");
  drive(40, HIGH, 40, HIGH);
  Serial.println("done");
}

void left(){
  Serial.println("left");
  drive(40, HIGH, 0, HIGH);
  Serial.println("done");
}

void right(){
  Serial.println("right");
  drive(0, HIGH, 40, HIGH);
  Serial.println("done");
}

void reverse(){
  Serial.println("reverse");
  drive(40, LOW, 40, LOW);
  Serial.println("done");
}

// Write a byte to shift register
void writeToRegister(byte data) {
  digitalWrite(latchPin, LOW);
  //digitalWrite(clearPin, HIGH);
  digitalWrite(latchPin, HIGH);
  //digitalWrite(clearPin, LOW);
  
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, data);
  digitalWrite(latchPin, HIGH);
}
