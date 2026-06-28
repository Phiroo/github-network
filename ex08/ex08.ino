// ex08：物联网安防报警器模拟实验

#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "你的WiFi名称";
const char* password = "你的WiFi密码";

WebServer server(80);

const int ledPin = 2;
const int touchPin = 4;

int threshold = 30;

bool armed = false;       // 是否布防
bool alarmOn = false;     // 是否报警

unsigned long lastBlinkTime = 0;
bool ledState = false;

String htmlPage() {
  String stateText;

  if (!armed) {
    stateText = "当前状态：未布防";
  } else if (armed && !alarmOn) {
    stateText = "当前状态：已布防，等待触发";
  } else {
    stateText = "当前状态：报警中！";
  }

  String page = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <title>ESP32 安防报警器</title>
  <style>
    body {
      font-family: Arial;
      text-align: center;
      margin-top: 60px;
      background-color: #f7f7f7;
    }
    h1 {
      color: #333;
    }
    button {
      font-size: 24px;
      padding: 15px 30px;
      margin: 20px;
      border: none;
      border-radius: 8px;
    }
    .arm {
      background-color: #ff9900;
      color: white;
    }
    .disarm {
      background-color: #009966;
      color: white;
    }
    .state {
      font-size: 28px;
      margin-top: 30px;
      color: #cc0000;
    }
  </style>
</head>
<body>
  <h1>ESP32 物联网安防报警器</h1>
)rawliteral";

  page += "<div class='state'>" + stateText + "</div>";

  page += R"rawliteral(
  <br>
  <button class="arm" onclick="location.href='/arm'">布防 Arm</button>
  <button class="disarm" onclick="location.href='/disarm'">撤防 Disarm</button>
</body>
</html>
)rawliteral";

  return page;
}

void handleRoot() {
  server.send(200, "text/html", htmlPage());
}

void handleArm() {
  armed = true;
  alarmOn = false;
  digitalWrite(ledPin, LOW);
  server.sendHeader("Location", "/");
  server.send(302, "text/plain", "");
}

void handleDisarm() {
  armed = false;
  alarmOn = false;
  ledState = false;
  digitalWrite(ledPin, LOW);
  server.sendHeader("Location", "/");
  server.send(302, "text/plain", "");
}

void setup() {
  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/arm", handleArm);
  server.on("/disarm", handleDisarm);

  server.begin();
  Serial.println("Web server started");
}

void loop() {
  server.handleClient();

  int touchValue = touchRead(touchPin);
  bool touched = touchValue < threshold;

  if (armed && touched) {
    alarmOn = true;
  }

  if (alarmOn) {
    unsigned long now = millis();

    if (now - lastBlinkTime >= 100) {
      lastBlinkTime = now;
      ledState = !ledState;
      digitalWrite(ledPin, ledState ? HIGH : LOW);
    }
  }
}
