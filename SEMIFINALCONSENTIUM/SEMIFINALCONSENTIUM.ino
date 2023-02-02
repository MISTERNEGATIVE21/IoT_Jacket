#include <TinyGPSPlus.h>

//rx2 tx2 pin 16 , 17 WROOM ESP32

#include <ConsentiumThings.h>
#include "DHT.h"
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
TinyGPSPlus gps;
float lat,lng=0;
ConsentiumThings board;   // create ConsentiumThing object

const char *ssid = "Rickswifi"; // add WiFi SSID
const char *pass = "ricksuvo101"; // add WiFi password
const long interval = 5; // take 5 seconds of delay 
const char *key = "TDYPO5YGBUBESFTWVQUXXG";       // Write api key
void setup() {

  Serial.begin(9600);

  Serial2.begin(9600);

  delay(10000);

  Serial.println(F("DHTxx test!"));

  dht.begin();
  board.begin();   // init. IoT boad
  board.initWiFi(ssid, pass);  // begin WiFi connection

}
void loop() {

  delay(2000);
   int touch =touchRead(T9);// get value using T9 pin 32 (value depends upon the initial condition)

   Serial.println(touch);  
  delay(1000);
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));
  float data_0 = board.busRead(0, THRES_5);  // read voltage data
  
  float sensor_val[] = {t,h,touch,lat,lng};  // sensor data array
  String info_buff[] = {"Temperature","Humidity","Touch","Latitude","Longitude"}; // sensor info. array
  
  int sensor_num = sizeof(sensor_val)/sizeof(sensor_val[0]); // number of sensors connected 
  
  board.sendREST(key, sensor_num, info_buff, sensor_val, HIGH_PRE, interval);

  while (Serial2.available() > 0)

    if (gps.encode(Serial2.read()))

      displayInfo();

  if (millis() > 5000 && gps.charsProcessed() < 10)

  {
    Serial.println(F("No GPS detected: check wiring."));
    while (true);
  }


}

void displayInfo()

{

  Serial.print(F("Location: "));


  if (gps.location.isValid()){


    Serial.print("Lat: ");


    Serial.print(gps.location.lat(), 6);
     lat=gps.location.lat();

    Serial.print(F(","));


    Serial.print("Lng: ");


    Serial.print(gps.location.lng(), 6);
    lng=gps.location.lng();
    Serial.println();
    Serial.print("Speed in KM");
    Serial.print(gps.speed.kmph()); 
    Serial.println();
    Serial.print("speed valid " );
    Serial.print(gps.speed.isValid()); 
     Serial.println();
  }  
  else

  {
    Serial.print(F("INVALID"));
  }
}
void updateSerial()

{
  delay(10000);
  while (Serial.available())

  {

    Serial2.write(Serial.read());//Forward what Serial received to Software Serial Port
  }

  while (Serial2.available())

  {
    Serial.write(Serial2.read());//Forward what Software Serial received to Serial Port
   

  }

}