
#include <ArduinoJson.h>
#include <ezTime.h>
#include <WiFiClientSecure.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#include <Ultrasonic.h>


#ifndef STASSID
#define STASSID "INDOOR"
#define STAPSK  ""
#endif

Ultrasonic ultasonic_in(D1,D0);
Ultrasonic ultrasonic_out(D5,D6);

const int sensorID=5;

// defines variables
long duration;
int dist_in;
int height_in;

int dist_out;
int height_out;
int temp;
int temp1;
int people=0;
byte pos;
String date;
void setup()
{

    WiFi.begin(STASSID);
  Serial.begin(9600); // Starts the serial communication @9600
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());
  for(int i=0;i<100;i++){
      dist_in=ultasonic_in.read(CM);
      delay(300);
      dist_out=ultrasonic_out.read(CM);
      delay(300);
  }
    
  
  Serial.println("Distance Colibration In: ");
  Serial.print(dist_in);
  Serial.println("Distance Colibration Out: ");
  Serial.print(dist_out);
}
void loop() {

  
    // Clears the trigPin
    
    temp=ultasonic_in.read(CM);
    
    temp1=ultrasonic_out.read(CM);


//
    if(dist_in-temp>50){
        date=getTime();
        getJsonString( 1,  dist_in-temp, date);
        delay(300);
      }
  

    
    else if(dist_out-temp1>50){
        date=getTime();
        getJsonString( 0, dist_out-temp1, date);
        delay(300);
        }
   
                   
}

  void getJsonString(byte pos ,int height, String date){
  
       DynamicJsonDocument doc(2048);
       doc["people"]=pos;
       doc["SensorID"]=sensorID;
       doc["Height"]=height;
       doc["Date"]=date;
       String json;
      serializeJson(doc, json);
      // wait for WiFi connection
      if ((WiFi.status() == WL_CONNECTED)) {
    
        WiFiClient client;
        HTTPClient http;
    
        Serial.print("[HTTPS] begin...\n");
        // configure traged server and url
        
        http.begin(client, "http://www.hitprojectscenter.com/locationCounterAPI/new/event"); //HTTPs
        http.addHeader("Content-Type", "application/json");
        Serial.println(json);
        Serial.print("[HTTP] POST...\n");
        // start connection and send HTTP header and body
        int httpCode = http.POST(json);
        Serial.println(httpCode);
        delay(500);

   
  }
  }



  String getTime(){

      waitForSync();
      Timezone timenow;
      timenow.setLocation("Asia/Jerusalem");
      delay(10);
      return timenow.dateTime(" d-M-y H:i:s ");

  }
