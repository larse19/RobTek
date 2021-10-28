#define pin1 3
#define pin2 5

void setup() {
  // put your setup code here, to run once:
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  //Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  forward(2000);
  reverse(2000);
}

void forward(int ms) {
  analogWrite(pin1, 125);
  analogWrite(pin2, 0);
  delay(ms);
}

void reverse(int ms) {
  analogWrite(pin2, 125);
  analogWrite(pin1, 0);
  delay(ms);
}
