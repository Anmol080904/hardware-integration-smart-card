#include <WiFi.h>
#include <HTTPClient.h>
#include <TinyGPSPlus.h>

// Create GPS object
TinyGPSPlus gps;

// GPS serial pins
#define RXD2 16
#define TXD2 17

// WiFi credentials (specific network only)
const char* ssid = "YourWiFiName";
const char* password = "YourWiFiPassword";

// Dummy API endpoint
String serverName = "http://jsonplaceholder.typicode.com/posts"; 

// Hardware serial for GPS
HardwareSerial neogps(1);

void setup() {
  Serial.begin(115200);
  neogps.begin(9600, SERIAL_8N1, RXD2, TXD2);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi!");
}

void loop() {
  while (neogps.available() > 0) {
    if (gps.encode(neogps.read())) {
      if (gps.location.isValid()) {
        float latitude = gps.location.lat();
        float longitude = gps.location.lng();

        Serial.print("Latitude: ");
        Serial.println(latitude, 6);
        Serial.print("Longitude: ");
        Serial.println(longitude, 6);

        // Send to API
        if (WiFi.status() == WL_CONNECTED) {
          HTTPClient http;
          http.begin(serverName);

          http.addHeader("Content-Type", "application/json");

          // Send coordinates as JSON
          String httpRequestData = "{\"lat\": " + String(latitude, 6) + ", \"lng\": " + String(longitude, 6) + "}";
          int httpResponseCode = http.POST(httpRequestData);

          Serial.print("HTTP Response code: ");
          Serial.println(httpResponseCode);

          http.end();
        }
        delay(5000); // send every 5 seconds
      }
    }
  }
}
