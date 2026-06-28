// ex04：触摸传感器自锁开关

const int ledPin = 2;        // 板载LED
const int touchPin = 4;      // T0，对应GPIO4

int threshold = 30;          // 触摸阈值，可根据串口数值调整

bool ledState = false;       // LED当前状态
bool lastTouched = false;    // 上一次是否触摸

unsigned long lastTriggerTime = 0;
const unsigned long debounceDelay = 300; // 防抖时间

void setup() {
  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  Serial.println("ex04 touch toggle switch start");
}

void loop() {
  int touchValue = touchRead(touchPin);

  bool currentTouched = touchValue < threshold;

  Serial.print("Touch value: ");
  Serial.println(touchValue);

  if (currentTouched && !lastTouched) {
    unsigned long now = millis();

    if (now - lastTriggerTime > debounceDelay) {
      ledState = !ledState;
      digitalWrite(ledPin, ledState ? HIGH : LOW);

      lastTriggerTime = now;

      Serial.print("LED state changed to: ");
      Serial.println(ledState ? "ON" : "OFF");
    }
  }

  lastTouched = currentTouched;

  delay(50);
}
