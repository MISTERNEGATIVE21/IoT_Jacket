#include <ConsentiumThings.h>

unsigned long previousMillis = 0;  

int ledState = LOW; 

int select_lines[4] = {14,12,13,15}; //s0, s1, s2, s3

int MUXtable[16][4]=
{
  {0,0,0,0}, {1,0,0,0}, {0,1,0,0}, {1,1,0,0},
  {0,0,1,0}, {1,0,1,0}, {0,1,1,0}, {1,1,1,0},
  {0,0,0,1}, {1,0,0,1}, {0,1,0,1}, {1,1,0,1},
  {0,0,1,1}, {1,0,1,1}, {0,1,1,1}, {1,1,1,1}
};


ConsentiumThings::ConsentiumThings(){}

void ConsentiumThings::begin(){
    Serial.begin(EspBaud);
    pinMode(NETWRK_LED, OUTPUT);
    for(int i=0; i<4; i++) {
      pinMode(select_lines[i],OUTPUT); 
    }   
}

void ConsentiumThings::initWiFi(const char* ssid, const char* password){
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Got IP : ");
  Serial.println(WiFi.localIP());
}

float ConsentiumThings::busRead(int j, float threshold){
  digitalWrite(select_lines[0], MUXtable[j][0]);
  digitalWrite(select_lines[1], MUXtable[j][1]);
  digitalWrite(select_lines[2], MUXtable[j][2]);
  digitalWrite(select_lines[3], MUXtable[j][3]);
  float adc_0_data = analogRead(A0) * threshold;
  return adc_0_data;
}

void ConsentiumThings::sendREST(const char* key, int sensor_num, String info[],  float data[], int pre, long delay_interval_s){ 
  String serverName = "http://consentiuminc.online:80";
  
  unsigned long currentMillis = millis();
  
  long delay_interval_ms = delay_interval_s * 1000;

  if ((currentMillis - previousMillis) >= delay_interval_ms) {
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;

    if (sensor_num == 1){
      float sensor_0 = data[0];
      String sensor_0_type = info[0];
      serverName = serverName + "/update?send_key=" + (String)key + "&sensor1=" + String(sensor_0, pre) + "&info1=" + sensor_0_type;
    }
    else if (sensor_num == 2){
      float sensor_0 = data[0];
      String sensor_0_type = info[0];
      float sensor_1 = data[1];
      String sensor_1_type = info[1];
      serverName = serverName + "/update?send_key=" + (String)key + "&sensor1=" + String(sensor_0, pre) + "&info1=" + sensor_0_type + "&sensor2=" + String(sensor_1, pre) + "&info2=" + sensor_1_type;
    }
    else if (sensor_num == 3){
      float sensor_0 = data[0];
      String sensor_0_type = info[0];
      float sensor_1 = data[1];
      String sensor_1_type = info[1];
      float sensor_2 = data[2];
      String sensor_2_type = info[2];
      serverName = serverName + "/update?send_key=" + (String)key + "&sensor1=" + String(sensor_0, pre) + "&info1=" + sensor_0_type + "&sensor2=" + String(sensor_1, pre) + "&info2=" + sensor_1_type + "&sensor3=" + String(sensor_2, pre) + "&info3=" + sensor_2_type;
    }
    else if (sensor_num == 4){
      float sensor_0 = data[0];
      String sensor_0_type = info[0];
      float sensor_1 = data[1];
      String sensor_1_type = info[1];
      float sensor_2 = data[2];
      String sensor_2_type = info[2];
      float sensor_3 = data[3];
      String sensor_3_type = info[3];
      serverName = serverName + "/update?send_key=" + (String)key + "&sensor1=" + String(sensor_0, pre) + "&info1=" + sensor_0_type + "&sensor2=" + String(sensor_1, pre) + "&info2=" + sensor_1_type + "&sensor3=" + String(sensor_2, pre) + "&info3=" + sensor_2_type + "&sensor4=" + String(sensor_3, pre) + "&info4=" + sensor_3_type;
    }
    else if (sensor_num == 5){
      float sensor_0 = data[0];
      String sensor_0_type = info[0];
      float sensor_1 = data[1];
      String sensor_1_type = info[1];
      float sensor_2 = data[2];
      String sensor_2_type = info[2];
      float sensor_3 = data[3];
      String sensor_3_type = info[3];  
      float sensor_4 = data[4];
      String sensor_4_type = info[4]; 
      serverName = serverName + "/update?send_key=" + (String)key + "&sensor1=" + String(sensor_0, pre) + "&info1=" + sensor_0_type + "&sensor2=" + String(sensor_1, pre) + "&info2=" + sensor_1_type + "&sensor3=" + String(sensor_2, pre) + "&info3=" + sensor_2_type + "&sensor4=" + String(sensor_3, pre) + "&info4=" + sensor_3_type + "&sensor5=" + String(sensor_4, pre) + "&info5=" + sensor_4_type;
    }
    else if (sensor_num == 6){
      float sensor_0 = data[0];
      String sensor_0_type = info[0];
      float sensor_1 = data[1];
      String sensor_1_type = info[1];
      float sensor_2 = data[2];
      String sensor_2_type = info[2];
      float sensor_3 = data[3];
      String sensor_3_type = info[3];  
      float sensor_4 = data[4];
      String sensor_4_type = info[4]; 
      float sensor_5 = data[5];
      String sensor_5_type = info[5]; 
      serverName = serverName + "/update?send_key=" + (String)key + "&sensor1=" + String(sensor_0, pre) + "&info1=" + sensor_0_type + "&sensor2=" + String(sensor_1, pre) + "&info2=" + sensor_1_type + "&sensor3=" + String(sensor_2, pre) + "&info3=" + sensor_2_type + "&sensor4=" + String(sensor_3, pre) + "&info4=" + sensor_3_type + "&sensor5=" + String(sensor_4, pre) + "&info5=" + sensor_4_type + "&sensor6=" + String(sensor_5, pre) + "&info6=" + sensor_5_type;
    }
    else if (sensor_num == 7){
      float sensor_0 = data[0];
      String sensor_0_type = info[0];
      float sensor_1 = data[1];
      String sensor_1_type = info[1];
      float sensor_2 = data[2];
      String sensor_2_type = info[2];
      float sensor_3 = data[3];
      String sensor_3_type = info[3];  
      float sensor_4 = data[4];
      String sensor_4_type = info[4]; 
      float sensor_5 = data[5];
      String sensor_5_type = info[5]; 
      float sensor_6 = data[6];
      String sensor_6_type = info[6]; 
      serverName = serverName + "/update?send_key=" + (String)key + "&sensor1=" + String(sensor_0, 6) + "&info1=" + sensor_0_type + "&sensor2=" + String(sensor_1, 6) + "&info2=" + sensor_1_type + "&sensor3=" + String(sensor_2, 6) + "&info3=" + sensor_2_type + "&sensor4=" + String(sensor_3, 6) + "&info4=" + sensor_3_type + "&sensor5=" + String(sensor_4, 6) + "&info5=" + sensor_4_type + "&sensor6=" + String(sensor_5, 6) + "&info6=" + sensor_5_type + "&sensor7=" + String(sensor_6, 6) + "&info7=" + sensor_6_type;
    }
    http.begin(client, serverName.c_str());
      
    // Send HTTP GET request
    int httpResponseCode = http.GET();
      
    if (httpResponseCode>0) {
        previousMillis = currentMillis;

      if (ledState == LOW) {
        ledState = HIGH;
      } else {
        ledState = LOW;
      }

      digitalWrite(NETWRK_LED, ledState);
      String payload = http.getString();
      Serial.println(payload);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
      // Free resources
    http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
  }
}

