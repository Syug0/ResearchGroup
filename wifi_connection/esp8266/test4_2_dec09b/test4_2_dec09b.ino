#include <ESP8266WiFi.h>        // ESP8266WiFi
#include <ESP8266WiFiMulti.h>   //  ESP8266WiFiMulti
#include <ESP8266WebServer.h>   //  ESP8266WebServer
#include <ArduinoJson.h>        //  ArduinoJson　ライブラリー
 
#define AP_SSID "CC-NSL" //wifi_ssid
#define AP_PSW  "kawaimakoto"//wifi_password
ESP8266WiFiMulti wifiMulti;     // Create wifiMulti object
ESP8266WebServer esp8266_server(80);  // Create web server object to respond to HTTP requests Listening port (80）
IPAddress local_IP(192, 168, 3, 7);   // Set the IP of ESP8266-NodeMCU after networking
IPAddress gateway(192, 168, 0, 1);    // Set gateway IP
IPAddress subnet(255, 255, 255, 0);   // Set the subnet mask
IPAddress dns(192,168,3,1);           // Set the IP of the LAN DNS
int i=0;
String RFID;
String Num;
         
void setup(){
  Serial.begin(9600);          

  // Set up the development board network environment
  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("Failed to Config ESP8266 IP"); 
  } 
 
  wifiMulti.addAP(AP_SSID, AP_PSW);           // Enter the SSID and password
  wifiMulti.addAP("404nofound", "z23456791"); // ESP8266-NodeMCU起動するとwifiをスキャン
  wifiMulti.addAP("ssid_from_AP_3", "your_password_for_AP_3"); 
  Serial.println("Connecting ...");  

  int i = 0;                                 
  while (wifiMulti.run() != WL_CONNECTED) {  // wifiMulti.run()。use wifiMulti.run()，
    delay(1000);                             // 
    Serial.print(i++); Serial.print(' ');    // 
  }                                          // if WiFI connect success，wifiMulti.run() will return “WL_CONNECTED”。
                                             // “WL_CONNECTED”を利用し、while文の条件分として利用する
  // WiFi　connected success return success message 
  Serial.println('\n');                     // WiFi　conneted success
  Serial.print("Connected to ");            // NodeMCUはメッセージを出力
  Serial.println(WiFi.SSID());              // wifi.ssid
  Serial.print("IP address:\t");            // and
  Serial.println(WiFi.localIP());           // NodeMCUのIPアドレス

  esp8266_server.begin();                  
  esp8266_server.on("/", handleRoot);          
 
  Serial.println("HTTP esp8266_server started");//  ESP8266 network service function has been activated
}


void loop(){
  // Handling http server access
  esp8266_server.handleClient(); 
}                                                                   
 
void handleRoot() {   //access request of the website directory "/" 
  esp8266_server.send(200, "application/json", rootJson()); 
  i++; 
}
 
//build json
String rootJson(){
  readArduino();
  const size_t capacity = JSON_OBJECT_SIZE(3) +140;
  DynamicJsonDocument doc(capacity);
  JsonObject info = doc.createNestedObject("info");
  info["COUNT"]=String(i);
  info["RFID"] =RFID;
  info["Num"] = Num;
  String jsonCode;  
  serializeJson(doc, jsonCode);
//  Serial.print("json Code: ");Serial.println(jsonCode); 
  return jsonCode;
}

void readArduino(){
  String Arduino_Json;
  if(Serial.available()){
  Arduino_Json=Serial.readString();//read arduino data
  }else{
    return;
  }
  const size_t capacity = JSON_OBJECT_SIZE(2) + 140;
  DynamicJsonDocument doc(capacity);
  deserializeJson(doc, Arduino_Json);
  
  JsonObject info = doc["info"];
  RFID=info["RFID"].as<String>();
  Num=info["Num"].as<String>();
  Serial.println(RFID);
  Serial.println(Num);
//  return data;
}
