#include <WiFiS3.h>

char server[] = "3.144.250.192";
int port = 8080;
char ssid[] = "YOUR_WIFI_NAME";
char pass[] = "YOUR_WIFI_PASSWORD";

char server[] = "https://pa3-340.onrender.com/";
int port = 443;

const int temperaturePin = A0;

WiFiSSLClient client;

void setup() {
  Serial.begin(9600);

  // Connect to Wi-Fi
  Serial.print("Connecting to Wi-Fi");

  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    delay(5000);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  int sensorValue = analogRead(temperaturePin);

  float voltage = sensorValue * (5.0 / 1023.0);

  float temperatureC = (voltage - 0.5) * 100.0;
  float temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;

  Serial.print("Temperature: ");
  Serial.print(temperatureF);
  Serial.println(" F");

  // Create JSON data
  String jsonData = "{\"temperature\":" + String(temperatureF, 2) + "}";

  // Connect to Render
  if (client.connect(server, port)) {

    Serial.println("Connected to server.");

    // Send HTTP POST request
    client.println("POST /api/sensor HTTP/1.1");
    client.print("Host: ");
    client.println(server);
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
    client.println(jsonData.length());
    client.println();
    client.println(jsonData);

    // Read server response
    while (client.connected()) {
      if (client.available()) {
        String response = client.readStringUntil('\n');
        Serial.println(response);

        if (response == "\r") {
          break;
        }
      }
    }

    client.stop();
    Serial.println("Data sent.");
  } else {
    Serial.println("Connection to server failed.");
  }

  delay(10000);
}