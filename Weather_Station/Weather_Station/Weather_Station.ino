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
#include <TimeLib.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

// Debug flag
#define DEBUG true

// DHT22 temperature / humidity sensor defines
#define DHT_TYPE DHT22
#define INWARD_DHT22_PIN D1
#define OUTWARD_DHT22_PIN D3

// Anemometer defines
#define ANEMOMETER_PIN D2
#define DATAGRAM_SIZE 41 + 5

// Rain guage defines
#define RAIN_PIN D4
#define RAIN_INTERRUPT 2

// Initialise DHT sensors
DHT inwardDht22(INWARD_DHT22_PIN, DHT_TYPE, 11);
DHT outwardDht22(OUTWARD_DHT22_PIN, DHT_TYPE, 11);

// Initialise variables
const long interval = 20000; // Time interval between each reading
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

volatile int rainCount = 0; // Counts the flips of thr internal rain gauge seesaw
volatile int oldRainCount = 0; // Allows for counter reset
double rainfall = 0; // Total rainfall within interval
long debounceTime = 100; // Set time to disregard consecutive interrupts from rain gauge

const int queueSize = 30; // Size of queue

// Wifi variables
const char* wifi_ssid = "Fairview Wi-fi"; // Wifi information
const char* wifi_password = "c64be3dcb2"; // Password information

WiFiUDP Udp;
IPAddress timeServer;
const char* host = "0.uk.pool.ntp.org"; // Address to retrieve current time from
unsigned int localPort = 8888;  // local port to listen for UDP packets

String readingTime = ""; // Time of reading
const double forwardDst = 3.31; // When daylight savings pushes clocks forwards an hour
const double backwardDst = 10.31; // When daylight savings rewinds clocks backwards an hour
const int timeZone = 0; // Current time-zone (GMT)

// Class: Is a set of readings taken at the same time
class Reading {
  private:
    double readingInwardDht22Temperature; // Inward DHT22 temperature reading
    double readingInwardDht22Humidity; // Inward DHT22 humidity reading
    double readingOutwardDht22Temperature; // Outward DHT22 temperature reading
    double readingOutwardDht22Humidity; // Outward DHT22 humidity reading

    String readingWindDirection; // Wind direction as compass direction
    double readingWindSpeed; // Wind speed in metres per second

    double readingRainfall; // Total rainfall within interval

  public:
    Reading() {
      // Empty Object
    }
    Reading(double inputInwardDht22Temperature, double inputInwardht22Humidity, double inputOutwardDht22Temperature,
            double inputOutwardDht22Humidity, String inputWindDirection, double inputWindSpeed, double inputRainfall) {

      readingInwardDht22Temperature = inputInwardDht22Temperature;
      readingInwardDht22Humidity = inputInwardht22Humidity;
      readingOutwardDht22Temperature = inputOutwardDht22Temperature;
      readingOutwardDht22Humidity = inputOutwardDht22Humidity;
      readingWindDirection = inputWindDirection;
      readingWindSpeed = inputWindSpeed;
      readingRainfall = inputRainfall;
    }

    double getReadingInwardDht22Temperature();
    double getReadingInwardDht22Humidity();
    double getReadingOutwardDht22Temperature();
    double getReadingOutwardDht22Humidity();
    String getReadingWindDirection();
    double getReadingWindSpeed();
    double getReadingRainfall();
};


double Reading::getReadingInwardDht22Temperature() {
  return readingInwardDht22Temperature;
}

double Reading::getReadingInwardDht22Humidity() {
  return readingInwardDht22Humidity;
}

double Reading::getReadingOutwardDht22Temperature() {
  return readingOutwardDht22Temperature;
}

double Reading::getReadingOutwardDht22Humidity() {
  return readingOutwardDht22Humidity;
}

String Reading::getReadingWindDirection() {
  return readingWindDirection;
}

double Reading::getReadingWindSpeed() {
  return readingWindSpeed;
}

double Reading::getReadingRainfall() {
  return readingRainfall;
}

// Class: Implements the abstract data type 'Queue'
class Queue {
  private:
    unsigned int queueHead;
    unsigned int queueTail;
    Reading queue[queueSize];

  public:
    Queue() {
      queueHead = 0;
      queueTail = 0;
    }

    void enqueue(Reading);
    Reading dequeue();
    Reading getQueueItem(int);
    int getQueueLength();
};

// Places an item into the queue and updates queue tail
void Queue::enqueue(Reading input) {
  int newQueueTail = (queueTail + 1) % queueSize;
  if (queueHead == newQueueTail) {
    Serial.println("\nFailed to enqueue, queue is full");
  }
  else {
    queue[queueTail] = input;
    queueTail = newQueueTail;
  }
}

// Takes an item off the queue and creates the new queue head
Reading Queue::dequeue() {
  if (queueHead == queueTail) {
    Serial.println("\nFailed to dequeue, queue is empty");
  }
  else {
    Reading output = queue[queueHead];
    queueHead  = (queueHead + 1) % queueSize;
    return output;
  }
}

// Returns an item from the queue
// Parameter input: Is the lcoation of the item to retrieve
Reading Queue::getQueueItem(int input) {
  return queue[input];
}

// Returns the queue length
int Queue::getQueueLength() {
  return queueTail - queueHead + (queueHead > queueTail ? queueSize : 0);
}

Queue readingQueue = Queue(); // Initialise Queue

// Function that retrieves a reading from a DHT22
// Parameter (sensor): The DHT22 sensor retrieving from
// Parameter (type): The type to retrieve [temperature or humidity]
// Returns double: The reading
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
boolean recieveDatagram() {
  unsigned long duration = 0;

  while (digitalRead(ANEMOMETER_PIN) == HIGH) {
    delayMicroseconds(1);
    //yield();
  }
  while (digitalRead(ANEMOMETER_PIN) == LOW) {
    delayMicroseconds(1);
    //yield();
    duration++;
  }
  // Anemometer transmits data every two seconds
  if (duration > 200000) {
    delayMicroseconds(600); // Go to middle of first bit
    for (int i = 0; i < DATAGRAM_SIZE; i++) {
      anemometerDatagram[i] = digitalRead(ANEMOMETER_PIN);
      delayMicroseconds(1200);
    }
    return true;
  }
  else {
    return false;
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
    Serial.println("\nFailed to read from anemometer |Checksum Error|");
    Serial.println("Checksum: " + String(checksum) + " != " + String(calculatedChecksum));
    windSpeed = -1; // Sets windSpeed to -1 for database to discard
  }
  else {
    windSpeed = double(windSpeedValue) / 10;
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

// ISR - Increases the rain counter by one when fired
void rainCounter() {
  currentTime = millis();
  if (currentTime - previousTime >= debounceTime) {
    previousTime = currentTime;

    rainCount++;
  }
}

// Calulates the rainfall in mm from the rain count since last interval
double getRainReading() {
  oldRainCount = rainCount;
  return rainCount * 0.5;
}

const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

// Sends a NTP request to the time server at the given address
// **Modified from time library example TimeNTP_ESP8266WiFi**
void sendNTPpacket(IPAddress &address) {
  // Set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;
  // All NTP fields have been given values, now
  // You can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); // NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}

// Retrieves time from the time server
// **Modified from time library example TimeNTP_ESP8266WiFi**
time_t getNtpTime() {
  while (Udp.parsePacket() > 0) ; // Discard any previously received packets
  Serial.println("Transmit NTP Request");
  sendNTPpacket(timeServer);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      Serial.println("Receive NTP Response");
      Udp.read(packetBuffer, NTP_PACKET_SIZE);  // Read packet into the buffer
      unsigned long secsSince1900;
      // Convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
    }
  }
  Serial.println("No NTP Response");
  return 0; // Return 0 if unable to get the time
}

// Function that returns whether it is daylight saving time or not
// Returns boolean: true = is daylight saving, false = not
boolean isDaylightSaving() {
  String currentString = String(month()) + "." + String(day());
  float currentNo = currentString.toFloat();
  if (currentNo >= forwardDst && currentNo <= backwardDst) {
    return true;
  }
  else {
    return false;
  }
}

// Function that returns the time in dateTime format, taking into account daylight savings
String getTime() {
  String actualTime = "";
  if (timeStatus() != timeNotSet) {
    int newHour;
    if (isDaylightSaving() == true) {
      newHour = hour() + 1;
    }
    else {
      newHour = hour();
    }
    actualTime = String(year()) + "-" + String(month()) + "-" + String(day()) + " " + String(newHour) +
                 ":" + String(minute()) + ":" + String(second());
    return actualTime;
  }
  else {
    WiFi.hostByName(host, timeServer);
    setSyncProvider(getNtpTime);
  }
}

// Outputs the most recent meteorological data to the serial monitor
void displayLastReading() {
  Serial.println("\nDate Time: " + readingTime);
  Serial.println("Inward DHT22 Temperature: " + String(inwardDht22Temperature) + "\260C");
  Serial.println("Inward DHT22 Humidity: " + String(inwardDht22Humidity) + "%");
  Serial.println("Outward DHT22 Temperature: " + String(outwardDht22Temperature) + "\260C");
  Serial.println("Outward DHT22 Humidity: " + String(outwardDht22Humidity) + "%");
  Serial.println("Wind Direction: " + String(windDirection));
  Serial.println("Wind Speed: " + String(windSpeed) + " m/s");
  Serial.println("Rainfall: " + String(rainfall) + " mm");
}

// System set-up
void setup() {
  Serial.begin(115200);
  Serial.println("\nConnecting to: ");
  Serial.println(wifi_ssid);
  WiFi.begin(wifi_ssid, wifi_password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("WiFi connection failed, retrying.");
  }
  Serial.println("WiFi connected");
  Serial.println("IP address: " + WiFi.localIP());

  inwardDht22.begin(); // Initialise inward DHT22
  outwardDht22.begin(); // Initialise outward DHT22

  pinMode(ANEMOMETER_PIN, INPUT); // Initialise anemometer pin as 'input'

  pinMode(RAIN_PIN, INPUT);
  pinMode(RAIN_INTERRUPT, INPUT);
  digitalWrite(RAIN_INTERRUPT, HIGH);
  attachInterrupt(RAIN_INTERRUPT, rainCounter, FALLING);

  Udp.begin(localPort);
  WiFi.hostByName(host, timeServer);
  setSyncProvider(getNtpTime);
  delay(1000);
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

    if (recieveDatagram() == true) { // Retrieves anemometer datagram
      decomposeWindReading(); // Decomposed datagram into its parts: WindDirection and WindSpeed
      translateWindDirection(); // Gets the associated compass direction from its number
    }
    rainfall = getRainReading(); // Gets total rainfall

    readingTime = getTime();

    // Create new reading set from retrieved information
    Reading newReading = Reading(inwardDht22Temperature, outwardDht22Temperature, inwardDht22Humidity, outwardDht22Humidity, windDirection, windSpeed, rainfall);
    readingQueue.enqueue(newReading); // Adds it to queue
    //for (int i = 0; i < readingQueue.getQueueLength(); i++) {
     // Reading temp = readingQueue.getQueueItem(i);
     // Serial.println(temp.getReadingInwardDht22Temperature());
   // }
    // Perform when DEBUG flag is set to 'true'
    if (DEBUG == true) {
      displayLastReading();
    }

    rainCount = rainCount - oldRainCount;
  }
}
