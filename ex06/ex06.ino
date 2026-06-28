// ex06：警车双闪灯效，双通道PWM

const int ledA = 16;
const int ledB = 17;

const int channelA = 0;
const int channelB = 1;

const int pwmFreq = 5000;
const int pwmResolution = 8;

void setup() {
  ledcSetup(channelA, pwmFreq, pwmResolution);
  ledcSetup(channelB, pwmFreq, pwmResolution);

  ledcAttachPin(ledA, channelA);
  ledcAttachPin(ledB, channelB);
}

void loop() {
  // LED A 逐渐变亮，LED B 逐渐变暗
  for (int duty = 0; duty <= 255; duty++) {
    ledcWrite(channelA, duty);
    ledcWrite(channelB, 255 - duty);
    delay(8);
  }

  // LED A 逐渐变暗，LED B 逐渐变亮
  for (int duty = 255; duty >= 0; duty--) {
    ledcWrite(channelA, duty);
    ledcWrite(channelB, 255 - duty);
    delay(8);
  }
}
