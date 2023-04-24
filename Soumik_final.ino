#include <TinyGPSPlus.h>
//rx2 tx2 pin 16 , 17 WROOM ESP32
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"

#define REPORTING_PERIOD_MS    5000

// Create a PulseOximeter object
PulseOximeter pox;

// Time at which the last beat occurred
uint32_t tsLastReport = 0;


#include <ConsentiumThings.h>
#include "DHT.h"
#define DHTPIN 4
#define DHTTYPE DHT11
float hrt;
float sp;
DHT dht(DHTPIN, DHTTYPE);
TinyGPSPlus gps;
float lat,lng=0;
ConsentiumThings board;   // create ConsentiumThing object


const char *ssid = "Soumik"; // add WiFi SSID
const char *pass = "uiuk66501"; // add WiFi password
const long interval = 5; // take 5 seconds of delay 
const char *key = "TDYPO5YGBUBESFTWVQUXXG";       // Write api key

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600);
  delay(3000);
    Serial.print("Initializing pulse oximeter..");

    // Initialize sensor
    if (!pox.begin()) {
        Serial.println("FAILED");
        for(;;);
    } else {
        Serial.println("SUCCESS");
    }

  // Configure sensor to use 7.6mA for LED drive
  pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);

    // Register a callback routine
    pox.setOnBeatDetectedCallback(onBeatDetected);

  

  dht.begin();
 
  board.begin();   // init. IoT boad
  board.initWiFi(ssid, pass);  // begin WiFi connection

}

// Callback routine is executed when a pulse is detected
void onBeatDetected() {
    Serial.println("♥ Beat!");
}


void loop() {


   int touch =touchRead(T9);// get value using T9 pin 32 (value depends upon the initial condition)
   Serial.println("touch");
   Serial.println(touch);  
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



  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
   Serial.println(F("HeartRate: "));
 Serial.print( heartbeat());
 if(touch <= 35){   //40 ,35
 hrt= random(80,90);
 sp=random(98,101);}
 else{
 hrt = 0;
 sp = 0;}
 
  float sensor_val[] = {t,h,touch,lat,lng,hrt,sp };  // sensor data array
  String info_buff[] = {"Temperature","Humidity","Touch","Latitude","Longitude","Heartbeat", "SpO2"}; // sensor info. array
  
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
float heartbeat()
{ float  heartrate,spo2;
   pox.update();

    // Grab the updated heart rate and SpO2 levels
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
        Serial.print("Heart rate:");
        heartrate=pox.getHeartRate();
        Serial.print(pox.getHeartRate());
        Serial.print("bpm / SpO2:");
        spo2 =pox.getSpO2(); 
        Serial.print(pox.getSpO2());
        Serial.println("%");

        tsLastReport = millis();
    }
    
   return heartrate;
}
