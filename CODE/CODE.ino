#include <Wire.h>
#include <TinyMPU6050.h>
#include <ESP8266WiFi.h>

String apiKey = "ThingspeakAPIKEY";     //  Enter your Write API key from ThingSpeak
 
const char *ssid =  "Wifiname";  // name of your wifi
const char *pass =  "Wifipassword";  // password of your wifi
const char* server = "api.thingspeak.com";
MPU6050 mpu (Wire);
WiFiClient client;


void setup() {

  // Initialization
  mpu.Initialize();
  // Calibration (wait for about 20s to calibrate)
  mpu.Calibrate();

  //start up
  Serial.begin(9600);
  Serial.println("Done Clabration");

   Serial.println("Connecting to ");
       Serial.println(ssid);
 
       WiFi.begin(ssid, pass);
 
      while (WiFi.status() != WL_CONNECTED) 
     {
            delay(500);
            Serial.print(".");
     }
      Serial.println("");
      Serial.println("WiFi connected");
   


}

void loop() {
  mpu.Execute();
  while (mpu.GetRawAccX() <= -8000) {
    Serial.println("front");
    mpu.Execute();

  }
   while (mpu.GetRawAccX() >= 8000) {
      
    Serial.println("back");
    mpu.Execute();


  }
   while (mpu.GetRawAccY() <= -8000) {
    Serial.println("right");
    mpu.Execute();
 

  }
   while (mpu.GetRawAccY() >= 8000) {
    Serial.println("left");
    mpu.Execute();
  

  }
    while (mpu.GetRawAccX() < 8000 and mpu.GetRawAccX() > -8000 and mpu.GetRawAccY() < 8000 and mpu.GetRawAccY() > -8000) {

    Serial.println("none");
    mpu.Execute();
    
    }


  if (client.connect(server,80))   //   "184.106.153.149" or api.thingspeak.com
    {  
     String postStr = apiKey;

       postStr +="&field1=";
       postStr += String(mpu.GetRawAccX());
       postStr +="&field2=";
       postStr += String(mpu.GetRawAccX());
       postStr +="&field3=";
       postStr += String(mpu.GetRawAccY());
       postStr +="&field4=";
       postStr += String(mpu.GetRawAccY());
       postStr += "\r\n\r\n";
  
  

       client.print("POST /update HTTP/1.1\n");
       client.print("Host: api.thingspeak.com\n");
       client.print("Connection: close\n");
       client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
       client.print("Content-Type: application/x-www-form-urlencoded\n");
       client.print("Content-Length: ");
       client.print(postStr.length());
       client.print("\n\n");
       client.print(postStr);
    }
   client.stop();

  

}
