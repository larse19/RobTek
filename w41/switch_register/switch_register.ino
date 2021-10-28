#include <LinkedList.h>

#define latchPin 4
#define dataPin 7
#define clockPin 5
//#define clearPin 6

#define dirA 12
#define spA 3
#define brA 9
#define dirB 13
#define spB 11
#define brB 8 

#define A 2
#define B 6
#define C 10

#define multiplexerIO A5

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
Button backwardButton(1);
Button leftButton(2);
Button rightButton(3);
Button playButton(4);
Button pauseButton(5);
Button stopButton(6);

LinkedList<String> commands;
String state = "input";
int commandIndex = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(multiplexerIO, INPUT_PULLUP);
  
  Serial.begin(9600);
  forwardButton.begin();
  backwardButton.begin();
  leftButton.begin();
  rightButton.begin();
  playButton.begin();
  pauseButton.begin();
  stopButton.begin();
}


void loop() {
  // Input state
  if(state == "input"){
    if(forwardButton.isReleased()){
      commands.add("forward");
      Serial.println("forward pressed");
    }
    if(backwardButton.isReleased()){
      commands.add("backward");
      Serial.println("backward pressed");
    }
    if(leftButton.isReleased()){
      commands.add("left");
      Serial.println("left pressed");
    }
    if(rightButton.isReleased()){
      commands.add("right");
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

void doCommand(String command){
  //Do command
  Serial.println(command);

  //Execute for x seconds and listen for pause/stop while doing command
  int starttime = millis();
  int endtime = starttime;
  
  while ((endtime - starttime) <= 1000) // do this loop for up to 1000mS
  {
    if(pauseButton.isReleased()){
        state = "pause";
    }
    if(stopButton.isReleased()){
      state = "input";
      commands.clear();
    }
    endtime = millis();
  }
}

void writeToRegister(byte data) {
  digitalWrite(latchPin, LOW);
  //digitalWrite(clearPin, HIGH);
  digitalWrite(latchPin, HIGH);
  //digitalWrite(clearPin, LOW);
  
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, data);
  digitalWrite(latchPin, HIGH);
}
