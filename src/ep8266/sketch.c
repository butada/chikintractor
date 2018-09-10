#include <ESP8266WiFi.h>

#define OUT_PIN 13
#define IN_PIN 4

// ディープスリープ時間(マイクロ秒)
// モバイルバッテリーがOFFになる時間よりちょい短めに設定する
//const uint32_t DEEP_SLEEP_uS = 1000*1000*60;
const uint32_t DEEP_SLEEP_uS = 1000*1000*30;

// 何回起きたら実際に送信処理するか、の値
//const uint32_t SEND_INTERVAL = 55;
const uint32_t SEND_INTERVAL = 3;

void mainJob() {

  // 起動のお知らせ
  digitalWrite(OUT_PIN, HIGH );
  delay(1000);
  digitalWrite(OUT_PIN, LOW);
  delay(1000);
  digitalWrite(OUT_PIN, HIGH );
  delay(1000);

  // 光センサーで明度を取得する

  // もし、暗ければ deep sleep に戻る

  // リレーのSWをON
  Serial.println("リレーのSWをONにしました");
  digitalWrite(OUT_PIN, HIGH );

  // debug
  //return;

  Serial.println("delay 1分");
  delay(1*60*1000); // 1分（ラズパイが落ち着くまで待つ）

  // Raspberry Piからの終了合図を待つループ
  Serial.println("Raspberry Piからの終了合図を待つループです");
  for (int i=0; i<4*60; i++) { // 4分（＝5*60秒）
    // 終了合図を確認
    int ret = digitalRead(IN_PIN);
    if (ret == HIGH) {
      break;
    }
    delay(1000); // １秒
  }

  // Raspberry Piのshutdownが完了する相当の時間を待つ
  Serial.println("Raspberry Piのshutdownが完了する相当の時間を待ちます(30秒)");
  delay(30*1000); // 30秒
}

void setup() {
  Serial.begin(74880);  // 起動時に合わせて74880bpsにする

  //デバッグ用にシリアルを開く
  Serial.println("");
  Serial.println("program start");

  //LEDを点灯させるので、そのピンをOUTPUTに設定して、LOWに。
  pinMode(OUT_PIN, OUTPUT); // リレーのSW
  digitalWrite(OUT_PIN, LOW);
  pinMode(IN_PIN, INPUT); // ラズパイからの終了合図

  String resetReason = ESP.getResetReason();
  Serial.println(resetReason);

  uint32_t wakeupCounter;
  if ( resetReason == "Deep-Sleep Wake" ) {
    // deepsleepからの復帰だったら
    // RTCメモリからwakeupCounterを読み出して、＋１
    ESP.rtcUserMemoryRead(0, &wakeupCounter, sizeof(wakeupCounter));
    wakeupCounter++;
  }
  else {
    // 電源ONからの起動だったら、wakeupCounterを初期化（1発目は即実行）
    wakeupCounter = SEND_INTERVAL;
  }
  Serial.print("wakeupCounter : ");
  Serial.println(wakeupCounter);

  if ( SEND_INTERVAL <= wakeupCounter ) {
    // カウンタが規定値以上 → カウンタクリアして、仕事する
    wakeupCounter=0;
    mainJob();
  }
  else {
    // モバイルバッテリーがOFFにならないようにするために起きただけ
    // （機種によっては、電流を流したフラグを確実に立てさせるため、delay()を入れたほうがいいかも？）
    Serial.println("少し電気を消費します");
    delay(1000);
  }

  // RTCメモリにwakeupCounterを書き込む
  ESP.rtcUserMemoryWrite(0, &wakeupCounter, sizeof(wakeupCounter));

  // deep sleepモードに入る
  Serial.println("deep sleepモードに入ります");

  if ( SEND_INTERVAL-1 == wakeupCounter ) {
    ESP.deepSleep(DEEP_SLEEP_uS, WAKE_RF_DISABLED);
  }
  else {
    ESP.deepSleep(DEEP_SLEEP_uS, WAKE_NO_RFCAL);
  }

  //deepsleepモード移行までのダミー命令
  delay(100);

  //実際にはこの行は実行されない
  Serial.println("DEEP SLEEPing....");

}

void loop() {
  //
}


