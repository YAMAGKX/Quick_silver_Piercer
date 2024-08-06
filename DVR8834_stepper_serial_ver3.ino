const int stepPin = 10; // ステッピングモーターのステップピン
const int dirPin = 11;  // ステッピングモーターの方向ピン
int customDelay = 4000; // 初期遅延時間
const int interruptPin=8;//photointerrupter
const int pushsensorPin=A1;
int Step = 1000; //20*32
float limitforce=500.0;
float pushsensor=1024;

void setup() {
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(interruptPin,INPUT);
  Serial.begin(9600); // シリアル通信を初期化
}

void loop() {digitalRead(interruptPin);
  if (Serial.available()) {
    char command = Serial.read(); // シリアルからコマンドを読み取る
    switch (command) {
      case 'F': // 正回転
      
        Step = Serial.parseInt(); // シリアルから整数値を読み取る
        
        for(int i=0; i <=Step; i++){
        digitalWrite(dirPin, HIGH);
        digitalWrite(stepPin, HIGH); // ステップ信号を送信
        delayMicroseconds(customDelay); // カスタム遅延時間
        digitalWrite(stepPin, LOW);
        
        if (pushsensor>analogRead(pushsensorPin))
        {
          pushsensor=analogRead(pushsensorPin);
         }
        if (analogRead(pushsensorPin)<= limitforce) 
        {
            Serial.println(analogRead(pushsensorPin));
            break; // Stop rotation if pushsensor is limitforce or less
          }
        }
        
        Serial.println(pushsensor);
        pushsensor=analogRead(pushsensorPin);
        break;

        
      case 'R': // 逆回転
        Step = Serial.parseInt(); // シリアルから整数値を読み取る
        
         for(int i=0; i <=Step; i++){
        digitalWrite(dirPin, LOW);
        digitalWrite(stepPin, HIGH); // ステップ信号を送信
        delayMicroseconds(customDelay); // カスタム遅延時間
        digitalWrite(stepPin, LOW);
        
        if (pushsensor>analogRead(pushsensorPin))
        {
          pushsensor=analogRead(pushsensorPin);
          }
          
        if (analogRead(pushsensorPin)<= limitforce) {
            Serial.println(analogRead(pushsensorPin));
            break; // Stop rotation if pushsensor is limitforce or less
          }
        }
        
        Serial.println(pushsensor);
        pushsensor=analogRead(pushsensorPin);
        break;

      case 'S': // push_sensor
        analogRead(pushsensorPin);
        Serial.println(analogRead(pushsensorPin));
        break;
        
      case 'H': // homing
        while (digitalRead(interruptPin) == HIGH)
        {
        digitalWrite(dirPin, LOW);
        digitalWrite(stepPin, HIGH);
        delayMicroseconds(customDelay);
        digitalWrite(stepPin, LOW);
      }
      digitalWrite(stepPin, LOW); // Stop the motor
      Serial.println("now homing_position");
      pushsensor=analogRead(pushsensorPin);
      break;
          
        
      case 'D': // step数の設定
        customDelay = Serial.parseInt(); // シリアルから整数値を読み取る
        break;

      case 'L': // limitforceの設定
        limitforce = Serial.parseInt(); // シリアルから整数値を読み取る
        break;
        
      default:
        break;
    }

  }
}
