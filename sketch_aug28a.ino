#include <TinyGPS++.h>
#include <HardwareSerial.h>
#include <WiFi.h>
// ==================================================================
//                    HARDWARE CONFIGURATION
// ==================================================================
HardwareSerial gpsSerial(1);  
#define GPS_RX 16             
#define GPS_TX 17             
#define GPS_BAUD 9600         
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";
TinyGPSPlus gps;  // GPS parser object

// ==================================================================
//                    INITIALIZATION SETUP
// ==================================================================
void setup() {
  Serial.begin(115200);
  gpsSerial.begin(GPS_BAUD, SERIAL_8N1, GPS_RX, GPS_TX);
  Serial.println("Connecting to WiFi...");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

// ==================================================================
//                   MAIN PROGRAM LOOP
// ==================================================================
void loop() {
  // Read data from GPS module
  while (gpsSerial.available() > 0) {
    gps.encode(gpsSerial.read());
  }

  // If new location data is available
  if (gps.location.isUpdated()) {
    Serial.print("Latitude: ");
    Serial.println(gps.location.lat(), 6);

    Serial.print("Longitude: ");
    Serial.println(gps.location.lng(), 6);

    Serial.print("Satellites: ");
    Serial.println(gps.satellites.value());

    Serial.print("HDOP: ");
    Serial.println(gps.hdop.value());

    Serial.println("---------------------------");
  }

  delay(1000); // update every second
}  