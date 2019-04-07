#include <Keypad.h>

const byte ROWS = 4; 
const byte COLS = 3; 

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

byte rowPins[ROWS] = {D0, D5, D6, D7}; 
byte colPins[COLS] = {D8, D1, D2}; 

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void mykeypadsetup(){
  Serial.begin(115200);
}
//  
//int mykeypadloop(){
//  char customKey = customKeypad.getKey();
//  
//  if (customKey){
//    Serial.println(customKey);
//    return 1;
//  }
//  
//  return 0;
//}
