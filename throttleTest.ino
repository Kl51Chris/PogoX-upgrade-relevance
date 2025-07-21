#include <Arduino.h>
#include <Servo.h>

Servo esc;

/* 固定引脚与脉宽 */
const int estop_pin   = 6;      // 常闭 e-stop，拉低触发
const int esc_pin     = 9;      // pwm 输出
const int neutral_us  = 1500;   // 0 % 推力
const int max_us      = 1900;   // 100 % 推力

/* 步进测试参数 */
const int start_pct   = 30;     // 起步 30 %
const int step_pct    = 5;      // 每档 +5 %
const int hold_ms     = 3000;   // 每档保持 3 s

/* —— 工具函数 —— */
void check_estop() {
  if (digitalRead(estop_pin) == HIGH) {   // 按下立即停机
    esc.writeMicroseconds(neutral_us);
    Serial.println(F(">>> e-stop triggered <<<"));
    while (true);                        // 锁死程序，等待复位
  }
}

/* —— 推力阶梯测试 —— */
void thrust_step_test() {

  for (int pct = start_pct; pct <= 100; pct += step_pct) {
    check_estop();

    int pulse = neutral_us - pct * 4;    // 4 µs ≈ 1 %
    esc.writeMicroseconds(pulse);
    Serial.println(pulse);

    unsigned long t0 = millis();
    while (millis() - t0 < hold_ms) {  
      Serial.println(pulse);  // 持续保持 3 s
      check_estop();
    }
  }

  esc.writeMicroseconds(neutral_us);     // 归零
  Serial.println(F("test finished, motor stopped."));
}

/* —— setup 与 loop —— */
void setup() {
  Serial.begin(9600);
  while (!Serial);    
  pinMode(estop_pin, INPUT_PULLUP);

  esc.attach(esc_pin, neutral_us, max_us);
  delay(2000);                           // esc 上电自检
  Serial.println(F("waiting for GO..."));  
  while (true) {
    if (Serial.available()) {
      String cmd = Serial.readStringUntil('\n');
      if (cmd.indexOf("GO") != -1) {
        break;   // 收到“GO”后开始测试
      }
    }
    check_estop();  // 空闲时也监听 e-stop
  }
  thrust_step_test();                    // 上电自动跑一次
}

void loop() {
  check_estop();                         // 结束后仍监听 e-stop
}
