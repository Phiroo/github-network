// ex06：警车双闪灯效，双通道PWM
// 适用于新版 ESP32 Arduino Core

const int ledA = 16;
const int ledB = 17;

const int pwmFreq = 5000;
const int pwmResolution = 8; // 8位PWM，占空比范围0~255

void setup() {
  // 新版 ESP32 PWM 写法
  ledcAttach(ledA, pwmFreq, pwmResolution);
  ledcAttach(ledB, pwmFreq, pwmResolution);
}

void loop() {
  // LED A 逐渐变亮，LED B 逐渐变暗
  for (int duty = 0; duty <= 255; duty++) {
    ledcWrite(ledA, duty);
    ledcWrite(ledB, 255 - duty);
    delay(8);
  }

  // LED A 逐渐变暗，LED B 逐渐变亮
  for (int duty = 255; duty >= 0; duty--) {
    ledcWrite(ledA, duty);
    ledcWrite(ledB, 255 - duty);
    delay(8);
  }
}
