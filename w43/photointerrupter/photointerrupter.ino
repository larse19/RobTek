const byte ledPin = 4;
const byte interruptPin = 3;
volatile byte state = LOW;
volatile int num = 0;

#define dirA 13
#define spA 11

void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), increment, RISING);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  while(num < 20){
    analogWrite(spA, 90);
    digitalWrite(dirA, HIGH);
  }
  analogWrite(spA, 0);
  
}

void increment(){
  num += 1;
}
