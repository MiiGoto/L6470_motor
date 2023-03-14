#include <Arduino.h>
#include <SPI.h>  //(1)SPI通信をするための読み込み
 
//(2)ステッピングモーター用のピンの定義
#define PIN_SPI_SDO 1
#define PIN_SPI_CK 2
#define PIN_SPI_SDI 42
#define PIN_SPI_CS 41
 
void setup()
{
  delay(1000);
  Serial.begin(9600);
   
  //(3)ステッピングモーター用のピンの準備
  pinMode(PIN_SPI_MOSI, OUTPUT);
  pinMode(PIN_SPI_MISO, INPUT);
  pinMode(PIN_SPI_SCK, OUTPUT);
  pinMode(PIN_SPI_SS, OUTPUT);
  digitalWrite(PIN_SPI_SS, HIGH);
 
  //(4)SPI通信の開始宣言
  SPI.begin();
  SPI.setDataMode(SPI_MODE3);//SCKの立上りでテータを送受信、アイドル時はpinをHIGHに設定
  SPI.setBitOrder(MSBFIRST);//MSBから送信
 
  //(5)L6470の利用設定
  L6470_setup();
}
 
//**********************************************
//(6)SPI通信するための関数
//**********************************************
void L6470_send(unsigned char value){
  digitalWrite(PIN_SPI_SS, LOW); 
  SPI.transfer(value);  //制御信号をSPI通信で送る
  digitalWrite(PIN_SPI_SS, HIGH); 
}
 
//**********************************************
// (7)L6470のセットアップ
//**********************************************
void L6470_setup(){
  //デバイス設定
  L6470_send(0x00);
  L6470_send(0x00);
  L6470_send(0x00);
  L6470_send(0x00);  
  L6470_send(0xc0);
 
  //最大回転スピード設定
  L6470_send(0x07);//レジスタアドレス
  L6470_send(0x20);//値(10bit),デフォルト0x41
 
  //モータ停止中の電圧設定
  L6470_send(0x09);//レジスタアドレス
  L6470_send(0xFF);//値(8bit),デフォルト0x29
 
  //モータ定速回転時の電圧設定
  L6470_send(0x0a);//レジスタアドレス
  L6470_send(0xFF);//値(8bit),デフォルト0x29
 
  //加速中の電圧設定
  L6470_send(0x0b);//レジスタアドレス
  L6470_send(0xFF);//値(8bit),デフォルト0x29
 
  //減速中の電圧設定
  L6470_send(0x0c);//レジスタアドレス
  L6470_send(0xFF);//値(8bit),デフォルト0x29
 
  //フルステップ,ハーフステップ,1/4,1/8,…,1/128ステップの設定
  L6470_send(0x16);//レジスタアドレス
  L6470_send(0x00);//値(8bit)
}
 
//**********************************************
// (8)メイン処理 - loop()
//**********************************************
void loop(){
  //360度 - １回転させる処理
  L6470_send(0x50);//Run(DIR,SPD),0x51:正転,0x50:逆転 
  L6470_send(0x00);
  L6470_send(0x20);//回転スピードの設定
  L6470_send(0x00);
  delay(1604);//1604msで約1回転
  L6470_send(0xB8);//急停止(ハードストップ)
}
