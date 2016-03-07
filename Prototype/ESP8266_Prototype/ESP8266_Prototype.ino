#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <DHT.h>

#define DHT_TYPE DHT22
#define DHT_PIN D2

const char* ssid = "Fairview Wi-fi"; // Wifi information
const char* password = "c64be3dcb2";

ESP8266WebServer server(80); // Set-up server
DHT dht(DHT_PIN, DHT_TYPE, 11); // Initialise DHT sensor
 
String weatherString= "";
float temperature, humidity, windSpeed, pressure;
String windDirection, visibility = "";
unsigned long previousMillis = 0;
const long interval = 2000;

void handleRoot() {
  digitalWrite(BUILTIN_LED, HIGH);
  server.send(200, "text/plain", "This is a message from Esp8266! \nI provide weather data, add /weather to the URL to view.");
  digitalWrite(BUILTIN_LED, LOW);
}

void handleServerNotFound(){
  digitalWrite(BUILTIN_LED, HIGH);
  String message = "Server Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(BUILTIN_LED, LOW);
}

void getDHTReading() {
  unsigned long currentMillis = millis();
 
  if(currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    temperature = dht.readTemperature(); // Read temperature as Celsius
    humidity = dht.readHumidity(); // Read humitidy

    // Check if any reads failed and exit early (to try again).
    if (isnan(humidity) || isnan(temperature)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }
  }
}

void sendData() {
 getDHTReading();
 weatherString += "<html><head><meta http-equiv='refresh' content='5'/>";
 weatherString += "<title>ESP8266 Demo</title>";
 weatherString += "<style>body { background-color: #FFFFFF; font-family: Sans-Serif; Color: #000088; }</style></head>";
 weatherString += "<body><h1>The current weather for this location</h1>";
 weatherString += "<p>Temperature is: " + String((int)temperature) + " C</p>";
 weatherString += "<p>Humidity is: " + String((int)humidity) + " %</p></html>";
 server.send(200, "text/html",  weatherString);
}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, LOW);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  dht.begin();
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("WiFi failed, retrying.");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started.");
  }

  server.on("/", handleRoot);

  server.on("/weather", sendData);

  server.onNotFound(handleServerNotFound);
  
  server.begin(); 
  Serial.println("HTTP server started.");
}

void loop(void) {
   server.handleClient();
   delay(1);
}
