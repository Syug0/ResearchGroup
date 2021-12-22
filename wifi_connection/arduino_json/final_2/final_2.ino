#include <ArduinoJson.h>        //  ArduinoJson　ライブラリー

String RFID;
String Num;
String COUNT;
int i=0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  RFID=String(i);
  Num=String(i);
  COUNT=String(i);
  Serial.println(rootJson(COUNT,RFID,Num));
  delay(2000);
  i++;
}

String rootJson(String COUNT,String RFID,String num){
  const size_t capacity = JSON_OBJECT_SIZE(3) +140;
  DynamicJsonDocument doc(capacity);
  JsonObject info = doc.createNestedObject("info");
  info["COUNT"]=COUNT;
  info["RFID"] =RFID;
  info["Num"] = num;
  String jsonCode;  
  serializeJson(doc, jsonCode);
  return jsonCode;
}
