#include <Key.h>
#include <Keypad.h>
 
const byte ROWS = 4; //4行のキーパッドを使用
const byte COLS = 4; //4列のキーパッドを使用
 
char keys[ROWS][COLS] = {
  //配列を表す
  {'1', '2', '3', 'A'},  //{s13, s9, s5, s1},
  {'4', '5', '6', 'B'},  //{s14, s10, s6, s2},
  {'7', '8', '9', 'C'},  //{s15, s11, s7, s3},
  {'*', '0', '#', 'D'}   //{s16, s12, s8, s4}に対応
};
 
byte rowPins[ROWS] = {6, 7, 8, 9}; //接続するピン番号
byte colPins[COLS] = {2, 3, 4, 5}; 
 
Keypad customKeypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS); //キーパッドの初期化
 
void setup(){
  Serial.begin(9600);
}
  
void loop(){
  char customKey = customKeypad.getKey();//押されたキーを検出
  
  if (customKey){
    Serial.println(customKey);
  }
}

/*
 * keypad connect to arduino in order, s13 side to digital2, s1 side to digital9              
 */
