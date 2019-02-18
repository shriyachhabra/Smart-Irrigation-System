#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#include "DHT.h"

const int sensor_pin = A0;

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "your-auth-key";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "your-network-name";
char pass[] = "password";


#define DHTPIN D7          // What digital pin we're connected to

// Uncomment whatever type you're using!
#define DHTTYPE DHT11 

DHT dht;
BlynkTimer timer;
float moisture_percentage;


#define PIN_UPTIME V6
void sendSensor()
{
  
  float t = dht.getTemperature(); // or dht.readTemperature(true) for Fahrenheit

  if ( isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V4, t);
}

void myMotor(){
  float valper = ( 100.00 - ( (analogRead(A0)/1023.00) * 100.00 ) );
  Serial.println(valper);
  
  //Serial.println("hi"+val);  
  if(valper>=20.0) 
  {
     //Blynk.notify(" Alert - Soil Moisture content not appropriate!");
  digitalWrite(D4,HIGH); //if soil moisture sensor provides LOW value send LOW value to motor pump and motor pump goes off
  }
  else
  {
  digitalWrite(D4,LOW); //if soil moisture sensor provides HIGH value send HIGH value to motor pump and motor pump get on
  }
}

// This function tells Arduino what to do if there is a Widget
// which is requesting data for Virtual Pin (5)

BLYNK_READ(PIN_UPTIME)
{
  // This command writes Arduino's uptime in seconds to Virtual Pin (5)
  moisture_percentage = ( 100.00 - ( (analogRead(sensor_pin)/1023.00) * 100.00 ) );

  Blynk.virtualWrite(PIN_UPTIME, moisture_percentage);
 
}

//BLYNK_WRITE(V5) // Button Widget writes to Virtual Pin V5 
//{
//                          // if Button sends 0
//    myMotor();             // start the function
//  
//}



void setup()
{
  pinMode(D4,OUTPUT); //Set pin 13 as OUTPUT pin
  pinMode(A0,INPUT);
  
  // Debug console
  Serial.begin(9600);
  

  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

dht.setup(D7);
timer.setInterval(1000L, sendSensor);
}

void loop()
{
  Blynk.run();
  myMotor();
  timer.run();

  
  
}
