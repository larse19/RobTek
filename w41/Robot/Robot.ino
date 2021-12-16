#include <LinkedList.h>
#include <EEPROM.h>

#define latchPin 4
#define dataPin 7
#define clockPin 5

#define forwardLED B00000010
#define reverseLED B00001000
#define leftLED B00100000
#define rightLED B10000000
#define inputLED B00000100
#define activeLED B00010000
#define pauseLED B01000000

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
volatile int speedA = 150;
volatile int speedB = 150;
int interval = 1000/20; // Interval between each tick of the tachometer (1000 millis / 20 slits)
volatile int oldMilisA = 0;
volatile int oldMilisB = 0;

int listAddr = 10;
int listSizeAddr = 9;

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
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  
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
    writeToRegister(inputLED);
    if(forwardButton.isReleased()){
      commands.add(0);
      Serial.println("forward pressed");
      writeToRegister(forwardLED | inputLED);
    }
    if(reverseButton.isReleased()){
      commands.add(1);
      Serial.println("reverse pressed");
      writeToRegister(reverseLED | inputLED);
    }
    if(leftButton.isReleased()){
      commands.add(2);
      Serial.println("left pressed");
      writeToRegister(leftLED | inputLED);
    }
    if(rightButton.isReleased()){
      commands.add(3);
      Serial.println("right pressed");
      writeToRegister(rightLED | inputLED);
    }
    
    if(playButton.isReleased()){
      if(commands.size() == 0){
        // If no command is imputted, run the last command from EEPROM
        int commandArr[EEPROM.read(listSizeAddr)];
        readIntArrayFromEEPROM(listAddr, commandArr, sizeof(commandArr));
        addArrayToCommands(commandArr);
        Serial.println("reading from EEPROM");
      }else{
        // If a command is inputted, write it to EEPROM
        writeIntArrayIntoEEPROM(listAddr, commandsToArray(), listSizeAddr);
        Serial.println("writing to EEPROM");
      }
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
    }


  // Pause state
  }else if(state == "pause"){
    Serial.println("paused");
    writeToRegister(pauseLED);
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
        writeToRegister(forwardLED | activeLED);
        forward();
        break;
       case 1:
        writeToRegister(reverseLED | activeLED);
        reverse();
        break;
       case 2:
        writeToRegister(leftLED | activeLED);
        left();
        break;
       case 3:
        writeToRegister(rightLED | activeLED);
        right();
        break;
    }
}

// General drive function with pause/stop listener
void drive(int ticksA, boolean directionA, int ticksB, boolean directionB){
  numA = 0;
  numB = 0;
  while(numA < ticksA || numB < ticksB){
    if(numA < ticksA){
      analogWrite(spA, speedA);
      digitalWrite(dirA, directionA);
    }else{
      analogWrite(spA, 0);
    }
    if(numB < ticksB){
      analogWrite(spB, speedB);
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
}

// Incrementers for tachometer
void incrementA(){
  numA += 1;
  if(millis() - oldMilisA > interval){
    if(speedA < 250){
      speedA += 5;
    }
  }else if(millis() - oldMilisA < interval){
    if(speedA > 100){
      speedA -= 5;
    }
  }
  oldMilisA = millis();
}

void incrementB(){
  numB += 1;
  if(millis() - oldMilisB > interval - 20){
    if(speedB < 250){
      speedB += 5;
    }
  }else if(millis() - oldMilisB < interval + 20){
    if(speedB > 100){
      speedB -= 5;
    }
  }
  oldMilisB = millis();
}

// Directional drive functions
void forward(){
  Serial.println("forward");
  drive(40, HIGH, 40, HIGH);
  Serial.println("done");
}

void left(){
  Serial.println("left");
  drive(15, HIGH, 0, HIGH);
  Serial.println("done");
}

void right(){
  Serial.println("right");
  drive(0, HIGH, 15, HIGH);
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
  shiftOut(dataPin, clockPin, MSBFIRST, data);
  digitalWrite(latchPin, HIGH);
}

void writeIntArrayIntoEEPROM(int address, int numbers[], int arraySizeAddr)
{
  arraySize = sizeof(numbers);
  EEPROM.write(arraySizeAddr, arraySize)
  for (int i = 0; i < arraySize; i++) 
  {
    EEPROM.write(address + i, numbers[i]);
  }
}

void readIntArrayFromEEPROM(int address, int numbers[], int arraySize)
{
  for (int i = 0; i < arraySize; i++)
  {
    numbers[i] = (EEPROM.read(address + i));
  }
}

void addArrayToCommands(int arr[]){
  for(int i = 0; i < sizeof(arr); i++){
    commands.add(arr[i]);
  }
}

int * commandsToArray(){
  int arr[commands.size()];
  for(int i = 0; i < commands.size(); i++){
    arr[i] = commands.get(i);
  }
  return arr;
}
