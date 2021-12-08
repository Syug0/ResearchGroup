#include <SPI.h>
#include <MFRC522.h>
#include <Key.h>
#include <Keypad.h>

//RFID
constexpr uint8_t RST_PIN = 9;          // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = 10;         // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

//Keypad
const byte ROWS = 4; //4行のキーパッドを使用
const byte COLS = 4; //4列のキーパッドを使用
 
char keys[ROWS][COLS] = {
  //配列を表す
  {'1', '2', '3', 'A'},  //{s13, s9, s5, s1},
  {'4', '5', '6', 'B'},  //{s14, s10, s6, s2},
  {'7', '8', '9', 'C'},  //{s15, s11, s7, s3},
  {'*', '0', '#', 'D'}   //{s16, s12, s8, s4}に対応
};
 
byte rowPins[ROWS] = {6, 7, A0, A1}; //接続するピン番号
byte colPins[COLS] = {2, 3, 4, 5}; 

Keypad customKeypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS); //キーパッドの初期化

void setup() {
    Serial.begin(9600);   // Initialize serial communications with the PC
    while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
    SPI.begin();      // Init SPI bus
    mfrc522.PCD_Init();   // Init MFRC522
    mfrc522.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details
    Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
}

void loop() {
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
      return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
      return;
  }

  if(mfrc522.uid.uidByte[0] == 0x03 && mfrc522.uid.uidByte[1] == 0x28 //UID
  && mfrc522.uid.uidByte[2] == 0x46 && mfrc522.uid.uidByte[3] == 0x18){
    Serial.println("Pass");
    while(Serial.available() == 0){
      char customKey = customKeypad.getKey();//押されたキーを検出
     if (customKey){
     Serial.println(customKey);
     break;
     }
    }
    
  }else{
    Serial.println("fail");
    delay(2000);
  }
}
