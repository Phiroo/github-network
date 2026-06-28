// ex05：多档位触摸调速呼吸灯

const int ledPin = 2;
const int touchPin = 4;

int threshold = 30;

int speedLevel = 1;          // 速度档位：1、2、3
bool lastTouched = false;

unsigned long lastTriggerTime = 0;
const unsigned long debounceDelay = 300;

// PWM参数
const int pwmChannel = 0;
const int pwmFreq = 5000;
const int pwmResolution = 8; // 8位，范围0~255

int brightness = 0;
int fadeDirection = 1;

void setup() {
  Serial.begin(115200);

  ledcSetup(pwmChannel, pwmFreq, pwmResolution);
  ledcAttachPin(ledPin, pwmChannel);

  Serial.println("ex05 touch speed breathing LED start");
}

void loop() {
  handleTouch();
  updateBreathingLED();
}

void handleTouch() {
  int touchValue = touchRead(touchPin);
  bool currentTouched = touchValue < threshold;

  if (currentTouched && !lastTouched) {
    unsigned long now = millis();

    if (now - lastTriggerTime > debounceDelay) {
      speedLevel++;

      if (speedLevel > 3) {
        speedLevel = 1;
      }

      Serial.print("Speed level: ");
      Serial.println(speedLevel);

      lastTriggerTime = now;
    }
  }

  lastTouched = currentTouched;
}

void updateBreathingLED() {
  int stepValue;
  int delayTime;

  if (speedLevel == 1) {
    stepValue = 1;
    delayTime = 15;
  } else if (speedLevel == 2) {
    stepValue = 3;
    delayTime = 10;
  } else {
    stepValue = 6;
    delayTime = 5;
  }

  brightness += fadeDirection * stepValue;

  if (brightness >= 255) {
    brightness = 255;
    fadeDirection = -1;
  }

  if (brightness <= 0) {
    brightness = 0;
    fadeDirection = 1;
  }

  ledcWrite(pwmChannel, brightness);

  delay(delayTime);
}
