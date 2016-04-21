/*
   Weather Station

   Sketch for a Aberyswyth University, Computer Science, Major Project
   Retrieves meteorological information from an assorted range of sensors including;
   an anemometer, rain gauge and two DHT22 temperature/humidity sensors.

   This data is passed onto a database through connection via the university provided Wi-fi,
   where it is then displayed on a web page

   Created by Christopher Tsoi
*/

#include <DHT.h>

// Debug flag
#define DEBUG true

// DHT22 temperature / humidity sensor defines
#define DHT_TYPE DHT22
#define INWARD_DHT22_PIN D1
#define OUTWARD_DHT22_PIN D3

// Anemometer defines
#define ANEMOMETER_PIN D2
#define DATAGRAM_SIZE 41 + 5

// Initialise DHT sensors
DHT inwardDht22(INWARD_DHT22_PIN, DHT_TYPE, 11);
DHT outwardDht22(OUTWARD_DHT22_PIN, DHT_TYPE, 11);

// Initialise variables
const long interval = 10000; // Time interval between each reading
unsigned long currentTime = 0; // Current Time
unsigned long previousTime = 0; // Previous current time

double inwardDht22Temperature = 0; // Inward DHT22 temperature reading
double inwardDht22Humidity = 0; // Inward DHT22 humidity reading
double outwardDht22Temperature = 0; // Outward DHT22 temperature reading
double outwardDht22Humidity = 0; // Outward DHT22 humidity reading

enum dhtReadingType {
  TEMPERATURE,
  HUMIDITY
};

char anemometerDatagram[41];
unsigned int windDirectionNo = 0;
unsigned int windSpeed = 0;
unsigned int checksum = 0;

// Retrieves reading from a DHT22
// Parameter (sensor): The DHT22 sensor retrieving from
// Parameter (type): The type to retrieve [temperature or humidity]
double getDhtReading(DHT sensor, dhtReadingType type) {
  double tempReading = sensor.readTemperature();

  if (isnan(tempReading)) {
    if (type == TEMPERATURE) {
      Serial.print("\nFailed to read temperature from DHT\n");
    }
    else if (type == HUMIDITY) {
      Serial.print("\nFailed to read humidity from DHT\n");
    }
  }
  else {
    return tempReading;
  }
}

void displayLastReading() {
  Serial.println("\nInward DHT22 Temperature: " + String(inwardDht22Temperature) + "\260C");
  Serial.println("Inward DHT22 Humidity: " + String(inwardDht22Humidity) + "%");
  Serial.println("Outward DHT22 Temperature: " + String(outwardDht22Temperature) + "\260C");
  Serial.println("Outward DHT22 Humidity: " + String(outwardDht22Humidity) + "%");
}

void setup() {
  Serial.begin(115200);

  inwardDht22.begin();
  outwardDht22.begin();

  pinMode(ANEMOMETER_PIN, INPUT);
}

void loop() {
  currentTime = millis();

  if (currentTime - previousTime >= interval) {
    previousTime = currentTime;

    inwardDht22Temperature = getDhtReading(inwardDht22, TEMPERATURE);
    inwardDht22Humidity = getDhtReading(inwardDht22, HUMIDITY);
    outwardDht22Temperature = getDhtReading(outwardDht22, TEMPERATURE);
    outwardDht22Humidity = getDhtReading(outwardDht22, HUMIDITY);
    recieveDatagram();
    decomposeWindReading();
    
    if (DEBUG == true) {
      displayLastReading();
    }
  }
}
