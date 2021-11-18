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

void setup() {
  // put your setup code here, to run once:
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  writeToRegister(reverseLED | inputLED);

}

// Write a byte to shift register
void writeToRegister(byte data) {
  //digitalWrite(latchPin, LOW);
  //digitalWrite(clearPin, HIGH);
  //digitalWrite(latchPin, HIGH);
  //digitalWrite(clearPin, LOW);
  
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, data);
  digitalWrite(latchPin, HIGH);
}
