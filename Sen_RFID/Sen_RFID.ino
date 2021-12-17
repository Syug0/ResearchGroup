#include <SPI.h>
#include <MFRC522.h>
//#include <Key.h>
#include <Keypad.h>
//#include <Keyboard.h>
#include <ArduinoJson.h>
#include <time.h>
#include <FIRFilter.h>

//RFID
constexpr uint8_t RST_PIN = 9;          // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = 10;         // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

//Keypad
const byte ROWS = 4; //4行のキーパッドを使用
const byte COLS = 4; //4列のキーパッドを使用

//Sensor
int interval = 50;

//int HIGH_LED = 13; //red
//int LOW_LED = 2; //blue
//int SOUND = 3;

int motionSensor = 0;
int motionSensorBefore = 0;
int sensorTilt = 0;
int sensorTiltBefore = 0;

//degital filter by degital filter design services
const double a[] = {
    6.237074932031003e-19,  1.203468256554930e-03,  2.789059944207731e-03,  4.234500608097008e-03,
    3.949464324766553e-03,  -2.416866536162014e-18, -8.270810074278890e-03, -1.861479317048980e-02,
    -2.543297162983507e-02, -2.127139910497268e-02, 6.003184622079840e-18,  3.965539443147850e-02,
    9.204504649186659e-02,  1.453456837490316e-01,  1.852171297065770e-01,  2.000000000000000e-01,
    1.852171297065770e-01,  1.453456837490316e-01,  9.204504649186659e-02,  3.965539443147850e-02,
    6.003184622079840e-18,  -2.127139910497268e-02, -2.543297162983507e-02, -1.861479317048980e-02,
    -8.270810074278890e-03, -2.416866536162014e-18, 3.949464324766553e-03,  4.234500608097008e-03,
    2.789059944207731e-03,  1.203468256554930e-03,  6.237074932031003e-19};

//FIRFilter
FIRFilter fir(a); 

int person = 0;
int sensorTiltNum = 0;

int motionTilt[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

/*
 * motion[]
 * plus = 2
 * minus = 1
 * zero = 0
 */

void updateMotionSensor () {
  motionSensorBefore = motionSensor;
  sensorTiltBefore = sensorTilt;
  motionSensor = analogRead(A2);
  motionSensor = fir.filter(motionSensor);
  sensorTilt = (motionSensor - motionSensorBefore) / 5;
  delay(interval);
}

int Sensor(){
    updateMotionSensor();
  
  //Serial.println(motionSensor);

  //make vertical line per one sec
  //Serial.print(millis()/1000%2*1600);
  //Serial.print(",");

  /*
  if (motionSensor > 600){
    digitalWrite(HIGH_LED, HIGH);
  } else {
    digitalWrite(HIGH_LED, LOW);
  }

  if (motionSensor < 300){
    digitalWrite(LOW_LED, HIGH);
  } else {
    digitalWrite(LOW_LED, LOW);
  }
  */

  //motion sensor's gragh tilt
  //int sensorTilt = (motionSensor - motionSensorBefore) / 5;

  //Serial.println(sensorTilt);

  /*
  if (motionSensor > 600 || motionSensor < 300){
    //process when detected motion peak
    return;
  }
  */

  if (sensorTilt != 0) {

    for (int i = 0; i < 10; i++) {
      
      if (sensorTilt > 0) {
        motionTilt[i] = 2;
        
      } else if (sensorTilt < 0) {
        motionTilt[i] = 1;
        
      } else {
        
        for (int j = 0; j < 10; j++) {
          updateMotionSensor();
          if (sensorTilt == 0){
            sensorTiltNum++;
          }
        }
        
        if (sensorTiltNum > 9){
          break;
        }
        
      }

      updateMotionSensor();

      int loopNum = 0;
      while ((sensorTiltBefore >= 0) == (sensorTilt >= 0)){
        updateMotionSensor();
        loopNum++;
        if (loopNum > 10) {
          break;
        }
      }
    }

    int sameCount = 1;

    //remove zero signal
    while (sameCount){
      sameCount = 0;
      for (int i = 0; i < 10; i++) {
        if (motionTilt[i] == 0) {
          for (int j = i; j < 10; j++){
            motionTilt[j] = motionTilt[j+1];
            if (motionTilt[j] != 0) {
              sameCount = 1;
            }
          }
        }
      }
    }

    sameCount = 1;

    while (sameCount) {
      sameCount = 0;
      for (int i = 0; i < 10; i++){
        if (motionTilt[i] == motionTilt[i+1]){
          for (int j = i; j < 10; j++){
            motionTilt[j] = motionTilt[j+1];
            if (motionTilt[j] != 0){
              sameCount = 1;
            }
          }
        }
      }
    }

    int exNumber = 0;
    for (int i = 0; i < 10; i++){
      if (motionTilt[i] != 0){
        exNumber++;
      }
    }

    if (exNumber <= 4){
      return 1;
//      digitalWrite(LOW_LED, HIGH);
//      delay(interval);
//      digitalWrite(LOW_LED, LOW);
    } else {
      Serial.println("WTF");
      return 2;
//      digitalWrite(SOUND, HIGH);
//      delay(interval);
//      digitalWrite(SOUND, LOW);
    }
  
    
    for (int i = 0; i < 10; i++) {
      Serial.print(motionTilt[i]);
      motionTilt[i] = 0;
    }
    Serial.println(";");

  }
  sensorTiltNum = 0;
  return 0;
}


 
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

    //Sensor
    //pinMode(SENSOR_INPUT, INPUT);
}

void loop() {
  int RFIDflag = 0;
  int AbleToPassTime;
  int PassNum = 0;
  time_t start_time, end_time;
  char ctoi;
  int val;
  val = Sensor();

//  Serial.println(val);
  
  //Serial.println("RFIDflag");
  //Serial.println(RFIDflag);
  if( val >= 1){
    Serial.println("Alert");
  }

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
    RFIDflag = 1;
    while(1){
     char customkey = customKeypad.getKey();//押されたキーを検出
     if (customkey){
     Serial.println(customkey);
     ctoi = customkey;
     AbleToPassTime =0;
     break; //whileを抜けれているのか　or loop関数を抜けている!?
     }
     //Serial.print("wtf");
    }
    //Serial.print("WTF");
  }else{
    Serial.println("fail");
    RFIDflag = 0;
    delay(2000);
  }
//  Serial.println("RFIDflag");
//  Serial.println(RFIDflag);
  int AbleToPassNum = ctoi - '0';
//  Serial.println("AbleToPassNum");
//  Serial.println(AbleToPassNum);
//  Serial.println(AbleToPassTime);
  
  if(RFIDflag == 1){
    AbleToPassTime = 0;
    int startTime = millis();
    while( AbleToPassTime <= 100000 && PassNum < AbleToPassNum){
      AbleToPassTime = millis() - startTime;
      //Serial.println("a");
      val = Sensor();
//      Serial.println(val);
      if( val ==  1){
        Serial.println("if did");
        PassNum++;
        AbleToPassTime = 0;
        startTime = millis();
        val = Sensor();
//        Serial.println(val);
        while( val == 1){
          val = Sensor();
          if (val == 0) {
            break;
          }
        }
      }
    }
     Serial.println("Success!");
     Serial.println("PassNum");
     Serial.println(PassNum);
     RFIDflag = 0;
  }
}
  
