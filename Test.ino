#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#define FIREBASE_HOST "iot1-4847b.firebaseio.com"
//資料庫網址 勿包含”http://"和"/"
#define FIREBASE_AUTH "rgNIp2txXgUGW3u6UCd7jkI7QL5i2VRsxZoKP3wx" // 資料庫密鑰
#define WIFI_SSID "wifi-223"     //wifi ssid
#define WIFI_PASSWORD "26878119" //wifi password
 int EMESbutton = 5;
 int EMESled = 12;
 int EMESs = 0;
 int EMESoff = 0;
void setup() {
  Serial.begin(115200);  //啟用通訊
  pinMode(LED_BUILTIN,OUTPUT);  //設置腳位
  pinMode(EMESbutton,INPUT);
  pinMode(EMESled,OUTPUT);
  Serial.print("connecting");
  while (WiFi.status()!=WL_CONNECTED){ //尚未連接成功
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected:");
  Serial.println(WiFi.localIP()); //輸出ip位址
  Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH); //連線至資料庫
  Firebase.set("test","0"); //設置資料庫中標籤為test的值為0
  Firebase.set("EMES","0");
}


void loop() {
 int EMESs = digitalRead(EMESbutton);
 if (EMESs == HIGH)
 {
    digitalWrite(EMESled,HIGH);              
    Serial.println("EMES ON");
    Firebase.set("EMES","1");
  } 
 if (Firebase.failed()) {//連線至資料庫失敗
      Serial.print("setting number failed:");
      Serial.println(Firebase.error());
      firebasereconnect();//呼叫重新連接
      return;
  }
  int r=Firebase.getString("test").toInt();//讀取資料庫中標籤為test的值
  int EMESinside=Firebase.getString("EMES").toInt();//讀取資料庫中標籤為test的值
  delay(50);
  Serial.print("*****");
    Serial.println((long)"EMES",EMESinside);
  Serial.println(r);
  Serial.print("*****");

  digitalWrite(LED_BUILTIN,r);
 if (EMESinside == 0)
 {
    digitalWrite(EMESled,LOW);
    Serial.println("EMES OFF");
 }
 else
 {
 }
}

void firebasereconnect(){//重新連接
  Serial.println("Trying to reconnect");
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}
