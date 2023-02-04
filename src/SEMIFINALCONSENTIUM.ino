#include <TinyGPSPlus.h>
#include "EEPROM.h"
//--------------------------------------------------
//set length of char string
#define LENGTH(x) (strlen(x) + 1)
#define EEPROM_SIZE 200
//--------------------------------------------------
//WiFi credential reset pin (Boot button on ESP32)
#define WiFi_rst 0
unsigned long rst_millis;
//rx2 tx2 pin 16 , 17 WROOM ESP32
#include <Adafruit_Sensor.h>
#include <ConsentiumThings.h>
#include <DHT.h>
DHT dht(4, DHT11);
TinyGPSPlus gps;
float lat,lng=0;
ConsentiumThings board;   // create ConsentiumThing object

String sid;
String pss;
const long interval = 5; // take 5 seconds of delay 
const char *key = "TDYPO5YGBUBESFTWVQUXXG";       // Write api key
void setup() {
  Serial.begin(115200);
  //------------------------------------------------
  pinMode(WiFi_rst, INPUT);
  if (!EEPROM.begin(EEPROM_SIZE)) { //Init EEPROM
    Serial.println("failed to init EEPROM");
    delay(1000);
  }
  //------------------------------------------------
  else
  {
    //Read SSID stored at address 0
    sid = readStringFromFlash(0);
    Serial.print("SSID = ");
    Serial.println(sid);
    // Read Password stored at address 40
    pss = readStringFromFlash(40);
    Serial.print("psss = ");
    Serial.println(pss);
  }
  //------------------------------------------------
  WiFi.begin(sid.c_str(), pss.c_str());
  delay(3000);
  //------------------------------------------------
  //if WiFi is not connected
  if (WiFi.status() != WL_CONNECTED)
  {
    //Init WiFi as Station, start SmartConfig
    WiFi.mode(WIFI_AP_STA);
    WiFi.beginSmartConfig();
    //----------------------------------------------
    //Wait for SmartConfig packet from mobile
    Serial.println("Waiting for SmartConfig.");
    while (!WiFi.smartConfigDone()) {
      delay(500);
      Serial.print(".");
    }
    //----------------------------------------------
    Serial.println("");
    Serial.println("SmartConfig received.");
    //----------------------------------------------
    //Wait for WiFi to connect to AP
    Serial.println("Waiting for WiFi");
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    //----------------------------------------------
    Serial.println("WiFi Connected.");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    //----------------------------------------------
    //read the connected WiFi SSID and password
    sid = WiFi.SSID();
    pss = WiFi.psk();
    //----------------------------------------------
    Serial.print("SSID:");
    Serial.println(sid);
    Serial.print("PASS:");
    Serial.println(pss);
    Serial.println("Storing SSID & PASSWORD in EEPROM");
    //----------------------------------------------
    //store the ssid at address 0
    writeStringToFlash(sid.c_str(), 0);
    //store the password at address 40
    writeStringToFlash(pss.c_str(), 40);
    //----------------------------------------------
    Serial.println("OK");
  }
  //------------------------------------------------
  else
  {Serial.println("WiFi Connected");}
  //

  Serial.begin(9600);

  Serial2.begin(9600);

  delay(10000);
  WiFi.mode(WIFI_AP_STA);
  WiFi.beginSmartConfig();

  //Wait for SmartConfig packet from mobile
  Serial.println("Waiting for SmartConfig.");
  while (!WiFi.smartConfigDone()) {
    delay(500);
    sid = WiFi.SSID();
    pss = WiFi.psk();
    
    Serial.print(".");
  }
    const char *ssid=(char*)sid.c_str();

    const char *pass=(char*)pss.c_str();


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
  

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
 
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

 rst_millis = millis();
  while (digitalRead(WiFi_rst) == LOW) {
    // Wait till boot button is pressed 
  }
  //----------------------------------------------
  // check the button press time if it is greater 
  //than 3sec clear wifi cred and restart ESP 
  if (millis() - rst_millis >= 3000) {
    Serial.println("Reseting the WiFi credentials");
    writeStringToFlash("", 0); // Reset the SSID
    writeStringToFlash("", 40); // Reset the Password
    Serial.println("Wifi credentials erased");
    Serial.println("Restarting the ESP");
    delay(500);
    ESP.restart();
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

void writeStringToFlash(const char* toStore, int startAddr) {
  int i = 0;
  for (; i < LENGTH(toStore); i++) {
    EEPROM.write(startAddr + i, toStore[i]);
  }
  EEPROM.write(startAddr + i, '\0');
  EEPROM.commit();
}


String readStringFromFlash(int startAddr) {
  char in[128]; // char array of size 128 for reading the stored data 
  int i = 0;
  for (; i < 128; i++) {
    in[i] = EEPROM.read(startAddr + i);
  }
  return String(in);
}