#include <ArduinoJson.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <time.h>
#include <FirebaseArduino.h>
#define FIREBASE_HOST "iot1-4847b.firebaseio.com"
//資料庫網址 勿包含”http://"和"/"
#define FIREBASE_AUTH "rgNIp2txXgUGW3u6UCd7jkI7QL5i2VRsxZoKP3wx" // 資料庫密鑰
#define WIFI_SSID "wifi-223"     //wifi ssid
#define WIFI_PASSWORD "26878119" //wifi password
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
 int EMESbutton = 16;
 int EMESled = 12;
 int EMESs = 0;
 int EMESoff = 0;
 int timezone = 4;
 int dst = 0;
 int c =0;

void setup() {
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Booting.");
  delay(500);
    lcd.clear();
    lcd.print("Booting..");
    delay(500);
    lcd.clear();
      lcd.print("Booting...");
      delay(500);
   
  Serial.begin(115200);  //啟用通訊
  pinMode(LED_BUILTIN,OUTPUT);  //設置腳位
  pinMode(EMESbutton,INPUT);
  pinMode(EMESled,OUTPUT);
  digitalWrite(EMESbutton,LOW);
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
  digitalWrite(EMESled,LOW);
  configTime(8 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  Serial.println("\nWaiting for time");
  while (!time(nullptr)) {
    Serial.print(".");
    delay(1000);
  }
}

void loop() {
  emes();
  lcd11();
}

void emes(){
  
 int r=Firebase.getString("test").toInt();//讀取資料庫中標籤為test的值
 int EMESinside=Firebase.getString("EMES").toInt();//讀取資料庫中標籤為test的值
  int EMESs = digitalRead(EMESbutton); // EMES trigger
 if (EMESs == HIGH)
 {

    Firebase.set("EMES","1");
  }
  
  time_t now = time(nullptr);
  Serial.println(ctime(&now));
  Serial.print("TEST CODE :");
  Serial.print(r);
  Serial.print("\n");
  digitalWrite(LED_BUILTIN,r);


  delay(500);
  Serial.print("delay 500\n");
  
  if (EMESinside == 0) // EMES trigger off
 {
    digitalWrite(EMESled,LOW);
    Serial.println("EMES OFF");
 }
 else
 {
  digitalWrite(EMESled,HIGH);
    Serial.println("EMES ON");
 }
 if (Firebase.failed()) {//連線至資料庫失敗
      Serial.print("setting number failed:");
      Serial.println(Firebase.error());
      firebasereconnect();//呼叫重新連接
      return;
  }
}
void lcd11(){
   int EMESinside=Firebase.getString("EMES").toInt();//讀取資料庫中標籤為test的值

  lcd.setCursor(0, 0);
  time_t now = time(nullptr);
  lcd.print(ctime(&now));
  if (EMESinside==1)
  {
    lcd.setCursor(0,1);
    lcd.print("EMES ON ");
  }
  else
  {
    lcd.setCursor(0,1);
    lcd.print("EMES OFF");
  }
}

void firebasereconnect(){//重新連接
  Serial.println("Trying to reconnect");
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}
