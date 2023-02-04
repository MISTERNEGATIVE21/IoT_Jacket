#ifndef ConsentiumThings_h
#define ConsentiumThings_h

#include <Arduino.h>  

#ifdef ESP8266
    #include <ESP8266WiFi.h>
    #include <ESP8266HTTPClient.h>
    #include <WiFiClient.h> 
    #define NETWRK_LED 16       
#endif
#ifdef ESP32
    #include <WiFi.h>
    #include <HTTPClient.h>  
    #define NETWRK_LED 2
#endif

#define THRES_5 4.8828
#define THRES_3 3.2226
#define EspBaud 115200

#define LOW_PRE 2
#define MID_PRE 4
#define HIGH_PRE 6

class ConsentiumThings{
    public:
        ConsentiumThings();
        void begin();
        void loop_delay(long);
        void initWiFi(const char*, const char*);
        void sendREST(const char*, int, String[], float[], int, long); 
        float busRead(int, float);      
};

#endif