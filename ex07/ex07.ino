// ex07：Web网页端无极调光器

#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "你的WiFi名称";
const char* password = "你的WiFi密码";

WebServer server(80);

const int ledPin = 2;
const int pwmChannel = 0;
const int pwmFreq = 5000;
const int pwmResolution = 8;

int brightness = 0;

String htmlPage() {
  String page = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <title>ESP32 无极调光器</title>
  <style>
    body {
      font-family: Arial;
      text-align: center;
      margin-top: 60px;
      background-color: #f2f2f2;
    }
    h1 {
      color: #333;
    }
    input[type=range] {
      width: 80%;
    }
    .value {
      font-size: 30px;
      margin-top: 20px;
      color: #0066cc;
    }
  </style>
</head>
<body>
  <h1>ESP32 Web 无极调光器</h1>
  <input type="range" min="0" max="255" value="0" id="slider">
  <div class="value">亮度：<span id="val">0</span></div>

  <script>
    var slider = document.getElementById("slider");
    var val = document.getElementById("val");

    slider.oninput = function() {
      val.innerHTML = this.value;
      fetch("/set?value=" + this.value);
    }
  </script>
</body>
</html>
)rawliteral";

  return page;
}

void handleRoot() {
  server.send(200, "text/html", htmlPage());
}

void handleSet() {
  if (server.hasArg("value")) {
    brightness = server.arg("value").toInt();

    if (brightness < 0) brightness = 0;
    if (brightness > 255) brightness = 255;

    ledcWrite(pwmChannel, brightness);

    Serial.print("Brightness: ");
    Serial.println(brightness);

    server.send(200, "text/plain", "OK");
  } else {
    server.send(400, "text/plain", "Missing value");
  }
}

void setup() {
  Serial.begin(115200);

  ledcSetup(pwmChannel, pwmFreq, pwmResolution);
  ledcAttachPin(ledPin, pwmChannel);
  ledcWrite(pwmChannel, 0);

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
  server.on("/set", handleSet);

  server.begin();
  Serial.println("Web server started");
}

void loop() {
  server.handleClient();
}
