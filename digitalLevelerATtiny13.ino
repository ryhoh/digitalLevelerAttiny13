// Copyright (c) 2016 Tetsuya Hori
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

/*
 *   
 *   設計全般について参考にしたwikiページ
 *    http://www26.atwiki.jp/mylte/pages/13.html
 *   参考にしたソースコード
 *    http://www26.atwiki.jp/mylte/pages/14.html
 * 
 *            ATMEL / ATtiny13a
 *                 +-\/-+
 * ADC0 (D 5) PB5 1|    |8 Vcc
 * ADC3 (D 3) PB3 2|    |7 PB2 (D 2) ADC1
 * ADC2 (D 4) PB4 3|    |6 PB1 (D 1) PWM1
 *            GND 4|    |5 PB0 (D 0) PWM0
 *                 +----+
 * 
 * 電源（コイン電池）電圧の変化によりアナログ入力の値も微妙に変化する
 * 基板にはソケットを実装し、随時焼き直しをできる状態を整えること
 */

uint16_t xval;        //センサーデータの一時的な保管場所

#define analogPin 3                                                //使用するアナログピンの番号
#define midLevel 508                                               //中間レベル
#define err1 3                                                     //誤差の閾値その1
#define err2 7                                                     //誤差の閾値その2

#define ledPort0 0
#define ledPort1 1
#define ledPort2 2

void reset(){
  pinMode(ledPort0, INPUT);
  pinMode(ledPort1, INPUT);
  pinMode(ledPort2, INPUT);
  digitalWrite(ledPort1, LOW);
  digitalWrite(ledPort1, LOW);
  digitalWrite(ledPort2, LOW);
}

void put0(){
  pinMode(ledPort0, INPUT);
  pinMode(ledPort1, OUTPUT);
  pinMode(ledPort2, OUTPUT);
  digitalWrite(ledPort1, HIGH);
  digitalWrite(ledPort2, LOW);
}

void put1(){
  pinMode(ledPort0, INPUT);
  pinMode(ledPort1, OUTPUT);
  pinMode(ledPort2, OUTPUT);
  digitalWrite(ledPort1, LOW);
  digitalWrite(ledPort2, HIGH);
}

void put2(){
  pinMode(ledPort0, OUTPUT);
  pinMode(ledPort1, INPUT);
  pinMode(ledPort2, OUTPUT);
  digitalWrite(ledPort0, HIGH);
  digitalWrite(ledPort1, LOW);
}

void put3(){
  pinMode(ledPort0, OUTPUT);
  pinMode(ledPort1, OUTPUT);
  pinMode(ledPort2, INPUT);
  digitalWrite(ledPort0, LOW);
  digitalWrite(ledPort1, HIGH);
}

void put4(){
  pinMode(ledPort0, OUTPUT);
  pinMode(ledPort1, OUTPUT);
  pinMode(ledPort2, INPUT);
  digitalWrite(ledPort0, HIGH);
  digitalWrite(ledPort1, LOW);
}

void setup() {
  /* 消費電力低減措置 */
  ACSR  = 0b10000000; //アナログ比較器停止
  DIDR0 =   0b111111; //デジタル入力緩衝部停止
  
  analogReference(EXTERNAL);  //電源電圧を基準にアナログ入力を行う
}

void loop() {
  xval = analogRead(analogPin);                                     //センシング

  /*
   * 3つのポートで5つのLEDを駆動する
   * 3つのポートの内、1つをReadに、他の1つをHIGHに、残りをLOWにする
   */
  reset();
  
  if(xval < midLevel - err2) put1();
  else if(xval < midLevel - err1) put0();
  else if(xval < midLevel + err1) put2();
  else if(xval < midLevel + err2) put3();
  else put4();

  delay(4);
}
