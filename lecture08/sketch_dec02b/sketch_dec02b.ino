#define dirRight 13
#define spRight 11
#define dirLeft 12
#define spLeft 6

void setup() {
  // put your setup code here, to run once:
  pinMode(dirRight, OUTPUT);
  pinMode(dirLeft, OUTPUT);
  pinMode(spRight, OUTPUT);
  pinMode(spLeft, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  //drive(1, 120, 1, 120);
}

void drive(boolean directionLeft, int speedLeft, boolean directionRight, int speedRight){
    analogWrite(spLeft, speedLeft);
    digitalWrite(dirLeft, directionLeft);
    analogWrite(spRight, speedRight);
    digitalWrite(dirRight, directionRight);
}
