// ex09：实时传感器Web仪表盘

#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "Yuuko";
const char* password = "Swjsx601";

WebServer server(80);

const int touchPin = 4;

String htmlPage() {
  String page = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <title>ESP32 实时传感器仪表盘</title>
  <style>
    body {
      font-family: Arial;
      text-align: center;
      background-color: #101820;
      color: white;
      margin-top: 80px;
    }
    h1 {
      font-size: 36px;
    }
    .value {
      font-size: 90px;
      margin-top: 40px;
      color: #00ffcc;
    }
    .text {
      font-size: 22px;
      color: #cccccc;
    }
  </style>
</head>
<body>
  <h1>ESP32 实时触摸传感器仪表盘</h1>
  <div class="text">当前 touchRead 数值：</div>
  <div class="value" id="touchValue">--</div>
  <div class="text">手靠近或触摸引脚时，数值通常会变小</div>

  <script>
    function updateTouchValue() {
      fetch("/touch")
        .then(response => response.text())
        .then(data => {
          document.getElementById("touchValue").innerHTML = data;
        });
    }

    setInterval(updateTouchValue, 300);
    updateTouchValue();
  </script>
</body>
</html>
)rawliteral";

  return page;
}

void handleRoot() {
  server.send(200, "text/html", htmlPage());
}

void handleTouch() {
  int value = touchRead(touchPin);
  server.send(200, "text/plain", String(value));
}

void setup() {
  Serial.begin(115200);

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
  server.on("/touch", handleTouch);

  server.begin();
  Serial.println("Web server started");
}

void loop() {
  server.handleClient();
}
