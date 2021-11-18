#include <EEPROM.h>
int arr[] = {1,2,3};
int test[3];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  writeIntArrayIntoEEPROM(10, arr, sizeof(arr));
  //int test = readIntArrayFromEEPROM(10, 9);
  //EEPROM.write(10, 1);
  //Serial.println(EEPROM.read(13));
  readIntArrayFromEEPROM(10, test, 3);
  Serial.print("test: " + sizeof(test));
  for(int i = 0; i < sizeof(test); i++){
    Serial.println(test[i]);
  }
  
}

void loop() {
  // put your main code here, to run repeatedly:
  
}

void writeIntArrayIntoEEPROM(int address, int numbers[], int arraySize)
{
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
