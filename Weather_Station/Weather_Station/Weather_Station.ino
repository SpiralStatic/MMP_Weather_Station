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
#define DATAGRAM_SIZE 41

// Initialise DHT sensors
DHT inwardDht22(INWARD_DHT22_PIN, DHT_TYPE, 11);
DHT outwardDht22(OUTWARD_DHT22_PIN, DHT_TYPE, 11);

// Initialise variables
const long interval = 2000; // Time interval between each reading
unsigned long currentTime = 0; // Current Time
unsigned long previousTime = 0; // Previous current time

double inwardDht22Temperature = 0; // Inward DHT22 temperature reading
double inwardDht22Humidity = 0; // Inward DHT22 humidity reading
double outwardDht22Temperature = 0; // Outward DHT22 temperature reading
double outwardDht22Humidity = 0; // Outward DHT22 humidity reading

// Enumeration containing the types of data the DHT22s can retrieve
enum dhtReadingType {
  TEMPERATURE,
  HUMIDITY
};

unsigned char anemometerDatagram[DATAGRAM_SIZE]; // Array to hold anemometer datagram
unsigned int windDirectionNo = 0; // Wind direction in bit value
String windDirection = ""; // Wind direction as compass direction
double windSpeed = 0; // Wind speed in metres per second

// Function that retrieves a reading from a DHT22
// Parameter (sensor): The DHT22 sensor retrieving from
// Parameter (type): The type to retrieve [temperature or humidity]
double getDhtReading(DHT sensor, dhtReadingType type) {
  double tempReading;

  if (type == TEMPERATURE) {
    if (isnan(tempReading)) {
      Serial.print("\nFailed to read temperature from DHT\n");
    }
    else {
      tempReading = sensor.readTemperature();
      return tempReading;
    }
  }
  else if (type == HUMIDITY) {
    if (isnan(tempReading)) {
      Serial.print("\nFailed to read humidity from DHT\n");
    }
    else {
      tempReading = sensor.readHumidity();
      return tempReading;
    }
  }
}

// Function that retrieves the datagram from the amemometer and places it into a char array
void recieveDatagram() {
  unsigned long duration = 0;

  while (digitalRead(ANEMOMETER_PIN) == HIGH) {
    delayMicroseconds(1);
    yield();
  }
  while (digitalRead(ANEMOMETER_PIN) == LOW) {
    delayMicroseconds(1);
    yield();
    duration++;
  }
  // Waits two cycles as one datagram takes 49.2 msec [1.2 x 41], in order to ensure it registers a full message
  if (duration > 100000) {
    delayMicroseconds(600); // First 5 bits provide no information so skip [1.2msec x 5]
    for (int i = 0; i < DATAGRAM_SIZE; i++) {
      anemometerDatagram[i] = digitalRead(ANEMOMETER_PIN);
      delayMicroseconds(1200);
    }
  }
}

// Function that decomposes the datagram of an anemometer into its composed varaibles wind direction and wind speed
// **Modified from work by Pete Todd cht35@aber.ac.uk**
void decomposeWindReading() {
  // Determines wind direction from the supplied 4 bit value. Requires inverting and needs its endinness swapped
  windDirectionNo = (anemometerDatagram[8] << 3 ) + (anemometerDatagram[7] << 2 ) + (anemometerDatagram[6] << 1 ) + anemometerDatagram[5];
  windDirectionNo = (~windDirectionNo) & 0x0f;

  // Determines wind speed from the supplied 12 bit value. Requires inverting and needs its endinness swapped
  unsigned int windSpeedValue = (anemometerDatagram[20] << 11) + (anemometerDatagram[19] << 10) + (anemometerDatagram[18] << 9) + (anemometerDatagram[17] << 8) +
                                (anemometerDatagram[16] << 7) + (anemometerDatagram[15] << 6) + (anemometerDatagram[14] << 5) + (anemometerDatagram[13] << 4) +
                                (anemometerDatagram[12] << 3) + (anemometerDatagram[11] << 2) + (anemometerDatagram[10] << 1) + anemometerDatagram[9];

  windSpeedValue = (~windSpeedValue) & 0x1ff;

  windSpeed = double(windSpeedValue) / 10;
  // Calculates the checksum from the wind direction and three nibbles of the 12 bit wind speed by doing a sum calculation
  unsigned char windSpeedNibble1to4 = (anemometerDatagram[12] << 3) + (anemometerDatagram[11] << 2) + (anemometerDatagram[10] << 1) + anemometerDatagram[9];
  windSpeedNibble1to4 = (~windSpeedNibble1to4) & 0x0f;
  unsigned char windSpeedNibble5to8 = (anemometerDatagram[16] << 3) + (anemometerDatagram[15] << 2) + (anemometerDatagram[14] << 1) + anemometerDatagram[13];
  windSpeedNibble5to8 = (~windSpeedNibble5to8) & 0x0f;
  unsigned char windSpeedNibble9to12 = (anemometerDatagram[20] << 3) + (anemometerDatagram[19] << 2) + (anemometerDatagram[18] << 1) + anemometerDatagram[17];
  windSpeedNibble9to12 = (~windSpeedNibble9to12) & 0x0f;

  // SUM Calculation of nibbles
  unsigned int windSpeedNibleSum = windSpeedNibble1to4 + windSpeedNibble5to8 + windSpeedNibble9to12;

  unsigned char calculatedChecksum = (windDirectionNo + windSpeedNibleSum) & 0x0f;

  // Determines the checksum given by the anemometer
  unsigned int checksum = (anemometerDatagram[24] << 3) + (anemometerDatagram[23] << 2) + (anemometerDatagram[22] << 1) + anemometerDatagram[21];
  checksum = (~checksum) & 0x0f;

  // Checks to see if both the checksum and the calculated checksum match
  if (checksum != calculatedChecksum) {
  Serial.println("Failed to read from anemometer |Checksum Error|");
    Serial.println("Checksum: " + String(checksum) + " != " + String(calculatedChecksum));
  }
}

// Determines the wind direction as a compass measurement from the corresponding number
void translateWindDirection() {
  switch (windDirectionNo) {
    case 0:
      windDirection = "N";
      break;
    case 1:
      windDirection = "NNE";
      break;
    case 2:
      windDirection = "NE";
      break;
    case 3:
      windDirection = "ENE";
      break;
    case 4:
      windDirection = "E";
      break;
    case 5:
      windDirection = "ESE";
      break;
    case 6:
      windDirection = "SE";
      break;
    case 7:
      windDirection = "SSE";
      break;
    case 8:
      windDirection = "S";
      break;
    case 9:
      windDirection = "SSW";
      break;
    case 10:
      windDirection = "SW";
      break;
    case 11:
      windDirection = "WSW";
      break;
    case 12:
      windDirection = "W";
      break;
    case 13:
      windDirection = "WNW";
      break;
    case 14:
      windDirection = "NW";
      break;
    case 15:
      windDirection = "NNW";
      break;
  }
}

// Outputs the most recent meteorological data to the serial monitor
void displayLastReading() {
  Serial.println("\nInward DHT22 Temperature: " + String(inwardDht22Temperature) + "\260C");
  Serial.println("Inward DHT22 Humidity: " + String(inwardDht22Humidity) + "%");
  Serial.println("Outward DHT22 Temperature: " + String(outwardDht22Temperature) + "\260C");
  Serial.println("Outward DHT22 Humidity: " + String(outwardDht22Humidity) + "%");
  Serial.println("Wind Direction: " + String(windDirection));
  Serial.println("Wind Speed: " + String(windSpeed) + " m/s");
}

// System set-up
void setup() {
  Serial.begin(115200);

  inwardDht22.begin(); // Initialise inward DHT22
  outwardDht22.begin(); // Initialise outward DHT22

  pinMode(ANEMOMETER_PIN, INPUT); // Initialise anemometer pin as 'input'
}

// Main system loop
void loop() {
  currentTime = millis(); // Set current time (from when the board began)

  // If time since last reading is more than set interval, perform next reading
  if (currentTime - previousTime >= interval) {
    previousTime = currentTime;

    inwardDht22Temperature = getDhtReading(inwardDht22, TEMPERATURE); // Retrieve temperature from inward DHT22
    outwardDht22Temperature = getDhtReading(outwardDht22, TEMPERATURE); // Retrieve temperature from outward DHT22
    inwardDht22Humidity = getDhtReading(inwardDht22, HUMIDITY); // Retrieve humidity from inward DHT22
    outwardDht22Humidity = getDhtReading(outwardDht22, HUMIDITY); // Retrieve humidity from outward DHT22

    recieveDatagram(); // Retrieves anemometer datagram
    decomposeWindReading(); // Decomposed datagram into its parts: WindDirection and WindSpeed
    translateWindDirection();

    // Perform when DEBUG flag is set to 'true'
    if (DEBUG == true) {
      displayLastReading();
    }
  }
}
