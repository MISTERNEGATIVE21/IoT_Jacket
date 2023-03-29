#include <TinyGPSPlus.h>
#include <WiFiManager.h>
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
DHT dht(DHTPIN, DHTTYPE);
TinyGPSPlus gps;
float lat,lng=0;
ConsentiumThings board;   // create ConsentiumThing object
String sid; //from user
String pss;
const long interval = 10; // take 5 seconds of delay 
const char *key = "Default";       // Write api key

int spo2; //SPO2 value
int heartRate; //heart rate value

void setup() {
  Serial.begin(115200);

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

   WiFiManager wfm;

  
  wfm.setDebugOutput(false);

  wfm.resetSettings();

    WiFiManagerParameter custom_text_box("my_text", "Enter the API KEY ", "xyxyxyx", 50);
  // Add custom parameter
  wfm.addParameter(&custom_text_box);

  if (!wfm.autoConnect("IOT-JACKET", "iotjacket")) {
    // Did not connect, print error message
    Serial.println("failed to connect and hit timeout");

    // Reset and try again
    ESP.restart();
    delay(1000);
  }

  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  Serial.print("Custom text box entry: ");
  Serial.println(custom_text_box.getValue());

  Serial2.begin(9600);

  Serial.println(F("DHTxx test!"));

  dht.begin();
 
  board.begin();   // init. IoT boad
     //read the connected WiFi SSID and password
    sid = WiFi.SSID();
    pss = WiFi.psk();
     const char *ssid=(char*)sid.c_str();
     const char *pass=(char*)pss.c_str();
     const char *key = (char*)custom_text_box.getValue();
  board.initWiFi(ssid, pass);  // begin WiFi connection

}

// Callback routine is executed when a pulse is detected
void onBeatDetected() {
    Serial.println("♥ Beat!");
}


void loop() {


   int touch =touchRead(T9);// get value using T9 pin 32 (value depends upon the initial condition)

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
  Serial.print(F("°C "));
  Serial.print(f);
   pox.update();

    // Grab the updated heart rate and SpO2 levels
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
        Serial.print("Heart rate:");
        heartRate=pox.getHeartRate();
        Serial.print(pox.getHeartRate());
        Serial.print("bpm / SpO2:");
        spo2 =pox.getSpO2(); 
        Serial.print(pox.getSpO2());
        Serial.println("%");

        tsLastReport = millis();
    }
   Serial.println(F("HeartRate: "));
  Serial.println(heartRate);
  Serial.println(F("SpO2: "));
  Serial.println(spo2);
  float sensor_val[] = {t,h,touch,lat,lng,heartRate,spo2};  // sensor data array
  String info_buff[] = {"Temperature","Humidity","Touch","Latitude","Longitude","Heartbeat","SpO2"}; // sensor info. array
  
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
