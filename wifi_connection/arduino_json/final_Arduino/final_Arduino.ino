#include <ArduinoJson.h>        //  ArduinoJson　ライブラリー

String RFID;
String Num;
int i=0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  RFID=String(i);
  Num=String(i);
  Serial.println(rootJson(RFID,Num));
  delay(3000);
  i++;
}

String rootJson(String rfid,String num){
  const size_t capacity = JSON_OBJECT_SIZE(2) +140;
  DynamicJsonDocument doc(capacity);
  JsonObject info = doc.createNestedObject("info");
  info["RFID"] =rfid;
  info["Num"] = num;
  String jsonCode;  
  serializeJson(doc, jsonCode);
  return jsonCode;
}
