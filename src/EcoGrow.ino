#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>

// --- Pin Definitions ---
#define PUMP_PIN 25
#define FAN_PIN 26
#define DHTPIN 27
#define SOIL_PIN 34
#define WATER_PIN 35

#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

WebServer server(80);

// --- Network Settings ---
const char* ssid = "EcoGrow";
const char* password = "password123";

IPAddress local_IP(192, 168, 100, 1);
IPAddress gateway(192, 168, 100, 1);
IPAddress subnet(255, 255, 255, 0);

// --- State Variables & Caching ---
bool pumpState = false;
bool fanState = false;

float lastGoodTemp = 0.0;
float lastGoodHum = 0.0;

// --- HTML & CSS Dashboard ---
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>EcoGrow Dashboard</title>
  <style>
    body { font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; background-color: #f4f7f6; margin: 0; padding: 20px; color: #333; }
    h2 { text-align: center; color: #2c3e50; font-size: 28px; }
    .grid { display: grid; grid-template-columns: repeat(auto-fit, minmax(250px, 1fr)); gap: 20px; max-width: 900px; margin: 0 auto; }
    .card { background: white; padding: 20px; border-radius: 12px; box-shadow: 0 4px 6px rgba(0,0,0,0.05); text-align: center; }
    .value { font-size: 32px; font-weight: bold; color: #27ae60; margin: 10px 0; }
    .label { color: #7f8c8d; font-size: 14px; text-transform: uppercase; letter-spacing: 1px; }
    .btn { background-color: #e74c3c; border: none; color: white; padding: 15px 32px; text-align: center; text-decoration: none; display: inline-block; font-size: 16px; margin: 10px 2px; cursor: pointer; border-radius: 8px; width: 100%; transition: 0.3s; font-weight: bold;}
    .btn.on { background-color: #27ae60; }
    .controls { display: grid; grid-template-columns: 1fr 1fr; gap: 20px; max-width: 900px; margin: 20px auto 0 auto; }
  </style>
</head>
<body>
  <h2>EcoGrow System</h2>
  
  <div class="grid">
    <div class="card">
      <div class="label">Temperature</div>
      <div class="value" id="temp">-- &deg;C</div>
    </div>
    <div class="card">
      <div class="label">Humidity</div>
      <div class="value" id="hum">-- %</div>
    </div>
    <div class="card">
      <div class="label">Soil Moisture</div>
      <div class="value" id="soil">-- %</div>
    </div>
    <div class="card">
      <div class="label">Water Reservoir</div>
      <div class="value" id="water">-- %</div>
    </div>
  </div>

  <div class="controls">
    <div class="card">
      <div class="label">Water Pump</div>
      <button id="pumpBtn" class="btn" onclick="toggleRelay('pump')">OFF</button>
    </div>
    <div class="card">
      <div class="label">Cooling Fan</div>
      <button id="fanBtn" class="btn" onclick="toggleRelay('fan')">OFF</button>
    </div>
  </div>

<script>
  function updateData() {
    fetch('/data')
      .then(response => response.json())
      .then(data => {
        document.getElementById('temp').innerHTML = data.temp + ' &deg;C';
        document.getElementById('hum').innerHTML = data.hum + ' %';
        document.getElementById('soil').innerHTML = data.soil + ' %';
        document.getElementById('water').innerHTML = data.water + ' %';
        
        updateButton('pumpBtn', data.pump);
        updateButton('fanBtn', data.fan);
      });
  }

  function toggleRelay(device) {
    fetch('/toggle' + device)
      .then(response => response.json())
      .then(data => {
        updateButton('pumpBtn', data.pump);
        updateButton('fanBtn', data.fan);
      });
  }

  function updateButton(id, state) {
    var btn = document.getElementById(id);
    if(state) {
      btn.className = 'btn on';
      btn.innerHTML = 'ON';
    } else {
      btn.className = 'btn';
      btn.innerHTML = 'OFF';
    }
  }

  setInterval(updateData, 2000);
  window.onload = updateData;
</script>
</body>
</html>
)rawliteral";

// --- Route Handlers ---
void handleRoot() {
  server.send(200, "text/html", index_html);
}

void handleData() {
  // Read DHT11
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  
  // Software Recovery: If noise crashes the sensor, it returns NaN
  if (isnan(t) || isnan(h)) {
    // Force reset the sensor to recover it from the crash state
    dht.begin();
    // Fallback to the last known good values so the dashboard doesn't show 0 or NaN
    t = lastGoodTemp;
    h = lastGoodHum;
  } else {
    // Valid reading acquired, update our cache
    lastGoodTemp = t;
    lastGoodHum = h;
  }

  // Read Analog Sensors (ESP32 ADC is 0-4095)
  int soilRaw = analogRead(SOIL_PIN);
  int waterRaw = analogRead(WATER_PIN);

  // Convert to percentages 
  int soilPercent = map(soilRaw, 4095, 0, 0, 100); 
  soilPercent = constrain(soilPercent, 0, 100);

  int waterPercent = map(waterRaw, 0, 4095, 0, 100);
  waterPercent = constrain(waterPercent, 0, 100);

  // Build JSON response
  String json = "{";
  json += "\"temp\":" + String(t, 1) + ",";
  json += "\"hum\":" + String(h, 1) + ",";
  json += "\"soil\":" + String(soilPercent) + ",";
  json += "\"water\":" + String(waterPercent) + ",";
  json += "\"pump\":" + String(pumpState ? "true" : "false") + ",";
  json += "\"fan\":" + String(fanState ? "true" : "false");
  json += "}";
  
  server.send(200, "application/json", json);
}

void handleTogglePump() {
  pumpState = !pumpState;
  digitalWrite(PUMP_PIN, pumpState ? HIGH : LOW);
  handleData(); 
}

void handleToggleFan() {
  fanState = !fanState;
  digitalWrite(FAN_PIN, fanState ? HIGH : LOW);
  handleData(); 
}

void setup() {
  Serial.begin(115200);

  // Initialize Pins
  pinMode(PUMP_PIN, OUTPUT);
  pinMode(FAN_PIN, OUTPUT);
  digitalWrite(PUMP_PIN, LOW);
  digitalWrite(FAN_PIN, LOW);

  // Initialize Sensors
  dht.begin();

  // Setup Wi-Fi Access Point
  Serial.println("Configuring Access Point...");
  WiFi.softAPConfig(local_IP, gateway, subnet);
  WiFi.softAP(ssid, password);
  
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  // Setup Web Server Routes
  server.on("/", HTTP_GET, handleRoot);
  server.on("/data", HTTP_GET, handleData);
  server.on("/togglepump", HTTP_GET, handleTogglePump);
  server.on("/togglefan", HTTP_GET, handleToggleFan);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
