#include <ESP8266WiFi.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
#include <DHT.h>

// Temperature / Humidity defines
#define DHT_TYPE DHT22
#define DHT_PIN D1
#define DHT_PIN2 D3

// Wind Sensor defines
#define WIND_PIN D2
#define WIND_NO_OF_BITS 41 + 5

// Rain guage defines
#define RAIN_PIN D4
#define RAIN_INTERRUPT 2

// Debug mode
#define DEBUG false

// Initialise DHT sensors
DHT dht_in(DHT_PIN, DHT_TYPE, 11);
DHT dht_out(DHT_PIN2, DHT_TYPE, 11);

// Initialise variables
float temperature_in, temperature_out; // Temperatures
float humidity_in, humidity_out; // Humidities
unsigned char record_bits[WIND_NO_OF_BITS]; // Wind bits
unsigned int wind_direction_no = 0; // Wind direction number
unsigned int wind_speed = 0; // Wind Speed
String wind_direction = ""; // Wind direction
volatile int rain_counter = 0; // Counts guage 'flips'
volatile int old_rain_counter = 0; // Allows for counter reset
float rainfall = 0.0; // Rainfall
unsigned long current_millis = 0; // Current time
unsigned long previous_millis = 0; // Time since last reading
const long interval = 10000; // Time between readings
long debounce_time = 100;
unsigned int iteration = 0; // Each reading for debugging

char INSERT_SQL[] = "INSERT INTO weather_prototype.weather_reading (Temperature_In, Temperature_Out, Humidity_In, Humidity_Out, Wind_Speed, Wind_Direction, Rainfall) VALUES (%s, %s, %s, %s, %d, '%s', %s)";
char query[256];

// Wifi variables
const char* wifi_ssid = "******"; // Wifi information
const char* wifi_password = "******";

IPAddress db_address(******); // Database information
const int db_port = ******;
char db_user[] = "******";
char db_password[] = "******";

IPAddress router_address(******); // Router Address

WiFiClient client; // Wifi Client connection
MySQL_Connection conn((Client *)&client); //SQL Client connection

// Gets temperature and humidity readings from the two DHTs
void getDHTReadings() {
  // Get readings from DHTs
  temperature_in = dht_in.readTemperature(); // Read temperatures as Celsius
  temperature_out = dht_out.readTemperature();
  humidity_in = dht_in.readHumidity(); // Read humidities
  humidity_out = dht_out.readHumidity();

  // Check if any reads failed and exit early (to try again).
  if (isnan(temperature_in) || isnan(humidity_in)) {
    Serial.println("Failed to read from Inward DHT sensor!");
    return;
    }
  if (isnan(temperature_out) || isnan(humidity_out)) {
    Serial.println("Failed to read from Outward DHT sensor!");
    return;
  }
}

// Gets wind speed and direction from the wind sensor
// **Modified from work by Pete Todd cht35@aber.ac.uk**
void getWindReadings() {
  unsigned long duration = 0;
  
  while(digitalRead(WIND_PIN) == HIGH) {
    delayMicroseconds(1);
    yield();
   }
  while(digitalRead(WIND_PIN) == LOW) {
    delayMicroseconds(1);
    yield();
    duration++;
  }
  if(duration > 100000){
    delayMicroseconds(600);
    for(int b = 0;b<WIND_NO_OF_BITS;b++)
      {
        record_bits[b] = digitalRead(WIND_PIN);
        delayMicroseconds(1200);
      }
  wind_direction_no = (record_bits[8]<<3) + (record_bits[7]<<2) + (record_bits[6]<<1) + record_bits[5];
      wind_direction_no = (~wind_direction_no) & 0x0f;

      wind_speed = (record_bits[20]<<11) + (record_bits[19]<<10) + (record_bits[18]<<9) + (record_bits[17]<<8) + (record_bits[16]<<7) + (record_bits[15]<<6) + 
                                (record_bits[14]<<5) + (record_bits[13]<<4) + (record_bits[12]<<3) + (record_bits[11]<<2) + (record_bits[10]<<1) + record_bits[9];
                              
      wind_speed = (~wind_speed) & 0x1ff;
      
  unsigned int checksum = (record_bits[24]<<3) + (record_bits[23]<<2) + (record_bits[22]<<1) + record_bits[21];
      checksum = (~checksum) & 0x0f;

  unsigned char ws_nibble1 = (record_bits[12]<<3) + (record_bits[11]<<2) + (record_bits[10]<<1) + record_bits[9];
      ws_nibble1 = (~ws_nibble1) & 0x0f;
      unsigned char ws_nibble2 = (record_bits[16]<<3) + (record_bits[15]<<2) + (record_bits[14]<<1) + record_bits[13];
      ws_nibble2 = (~ws_nibble2) & 0x0f;
      unsigned char ws_nibble3 = (record_bits[20]<<3) + (record_bits[19]<<2) + (record_bits[18]<<1) + record_bits[17];
      ws_nibble3 = (~ws_nibble3) & 0x0f;

   unsigned int ws_nibble_sum = ws_nibble1 + ws_nibble2 + ws_nibble3;

   unsigned char calc_checksum = (wind_direction_no + ws_nibble_sum) & 0x0f;
   
   if(checksum == calc_checksum) {
    return;
   }
   else {
    Serial.println("Failed to read from wind sensor *Checksum error*");
    Serial.println("Checksum: " + String(checksum) + " != " + String(calc_checksum));
    return;
   }
  }
  else {
    return ;
  }
}

// Determines the wind direction from the corresponding number
void retrieveWindDirection() {
  if(DEBUG) {Serial.println("|DEBUG| Wind direction number: " + String(wind_direction_no)); }
  if(wind_direction_no == 0) { wind_direction = "N"; }
  else if(wind_direction_no == 1) { wind_direction = "NNE"; }
  else if(wind_direction_no == 2) { wind_direction = "NE"; }
  else if(wind_direction_no == 3) { wind_direction = "ENE"; }
  else if(wind_direction_no == 4) { wind_direction = "E"; }
  else if(wind_direction_no == 5) { wind_direction = "ESE"; }
  else if(wind_direction_no == 6) { wind_direction = "SE"; }
  else if(wind_direction_no == 7) { wind_direction = "SSE"; }
  else if(wind_direction_no == 8) { wind_direction = "S"; }
  else if(wind_direction_no == 9) { wind_direction = "SSW"; }
  else if(wind_direction_no == 10) { wind_direction = "SW"; }
  else if(wind_direction_no == 11) { wind_direction = "WSW"; }
  else if(wind_direction_no == 12) { wind_direction = "W"; }
  else if(wind_direction_no == 13) { wind_direction = "WNW"; }
  else if(wind_direction_no == 14) { wind_direction = "NW"; }
  else if(wind_direction_no == 15) { wind_direction = "NNW"; }
}

// Calculates the rainfall in mm from the counter
void getRainReading() {
  if(DEBUG) {Serial.println("|DEBUG| Rain Counter: " + String(rain_counter)); }
  rainfall = rain_counter * 0.5;
  old_rain_counter = rain_counter;
}

// ISR - Increases the rain counter by one when fired
void rainCounter() {
  current_millis = millis();
  if(current_millis - previous_millis >= debounce_time) {
      rain_counter++;
      previous_millis = current_millis;
  }
}

void connectToDatabase() {
  if(conn.connect(db_address, db_port, db_user, db_password)) {
    Serial.println("Connected to database.");
  }
  else {
    Serial.println("Connection to database failed");
   }
}

// Inserts the sensor information into the mySQL Database
void insertData() {
  if(conn.connected()) {
      char new_temperature_in[10];
      char new_temperature_out[10];
      char new_humidity_in[10];
      char new_humidity_out[10];
      char new_rainfall[10];
      MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
      dtostrf(temperature_in, 1, 1, new_temperature_in);
      dtostrf(temperature_out, 1, 1, new_temperature_out);
      dtostrf(humidity_in, 1, 1, new_humidity_in);
      dtostrf(humidity_out, 1, 1, new_humidity_out);
      dtostrf(rainfall, 1, 1, new_rainfall);
      sprintf(query, INSERT_SQL, new_temperature_in, new_temperature_out, new_humidity_in, new_humidity_out, wind_speed, wind_direction.c_str(), new_rainfall);
      cur_mem->execute(query);
      delete cur_mem;
      if(DEBUG == true) { Serial.println(query); }
      Serial.println("Data inserted.");
    }
    else {
      connectToDatabase();
      insertData();
  }
}

// System set-up
void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println("\nSerial started:");
  if(DEBUG) {
    Serial.println("DEBUG MODE ENABLED");
    Serial.setDebugOutput(true);
    Serial.println("");
    delay(5000);
  }
  delay(1000);
  Serial.print("Connecting to: ");
  Serial.println(wifi_ssid);
  WiFi.begin(wifi_ssid, wifi_password);
  delay(1000);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("WiFi connection failed, retrying.");
  }
  Serial.println("WiFi connected");  
  Serial.println("IP address: " + WiFi.localIP());
  dht_in.begin();
  dht_out.begin();
  pinMode(WIND_PIN, INPUT);
  pinMode(RAIN_PIN, INPUT);
  pinMode(RAIN_INTERRUPT, INPUT);
  digitalWrite(RAIN_INTERRUPT,HIGH);
  attachInterrupt(RAIN_INTERRUPT, rainCounter, FALLING);
  Serial.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
}

// Main Loop
void loop() {  
  current_millis = millis();

  if(current_millis - previous_millis >= interval) {
    previous_millis = current_millis;
    
    getDHTReadings();
    getWindReadings();
    retrieveWindDirection();
    yield();
    getRainReading();
    Serial.println("");
    if(DEBUG) { Serial.println("Iteration: " + String(iteration)); iteration ++; }
    Serial.print("Temperature In: " + String((int)temperature_in) + "\260C, ");
    Serial.print("Temperature Out: " + String((int)temperature_out) + "\260C.\n");
    Serial.print("Humidity In: " + String((int)humidity_in) + "%, ");
    Serial.print("Humidity Out: " + String((int)humidity_out) + "%.\n");
    Serial.print("Wind Speed: " + String(wind_speed) + " mps, ");
    Serial.print("Wind Direction: " + wind_direction + ".\n");
    Serial.println("Rainfall: " + String(rainfall) + " mm");
    Serial.println("");
    insertData();
    rain_counter = rain_counter - old_rain_counter; // Reset counter
    Serial.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
  }
  delay(10);
}
