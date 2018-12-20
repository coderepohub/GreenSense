
/*
   ########## GREEN SENSE ############
   This project is basically an arduino based water gardener,which should take care of the plant.
   
   $$$Connection Setup$$$
   Analog Pin 0 - Soil Moisture Sensor-1
   Analog Pin 1 - Soil Moisture Sensor-2
   Analog Pin 2 - Soil Moisture Sensor-3
   Analog Pin 3 - LDR 
   Analog Pin 4 - Nail to monitor water Placed inside Gallon
   Digital Pin 2 - DHT Sensor
   Digital Pin 7 - Relay Module(Water Pump)
   Digital Pin 9 - Servo
   Digital Pin 3 - LED Pin (Red)
   Digital Pin 4 - LED Pin (Green)
   
   
   Date : 22-04-2015 
   Author : ^Devasish Raj ^Suraj Kumar ^Sudarshan Vyas ^Aman Kumar
  
  
*/

//Including library for DHT sensor
#include <DHT.h>
//Including Servo library
#include <Servo.h> 
//Including Ethernet and Twitter Library
#include <SPI.h> 
#include <Ethernet.h>
#include <Twitter.h>


//defining the variables for pins

//used for Water Pump
#define WaterPumpPin 7

//used for DHT Sensor (digital pin)
#define DHTPin 2

//Defining DHT Type
//Here we are using DHT11 
#define DHTType DHT11

//Variables to store storing the sensors value 

//Stores Soil Moisture Values
int moistureValueA0 = 0;
int moistureValueA1 = 0;
int moistureValueA2 = 0;

//Stores LDR light Sensor VAlues
int LDRValue = 0;

//Initialising DHT Sensor for Arduino
DHT dht(DHTPin,DHTType);

// create servo object to control a servo 
Servo myServo;

// Ethernet Shield Settings
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

// Specifying the IP address.
byte ip[] = { 169, 254, 165, 201 }; 

//  Token to Tweet 
Twitter twitter("3181896870-FBJONFE310XD5Qc8fAzn2LL6eUfG1mbx0e8V44i");


void setup()
{
  //Setting the baud rate
   Serial.begin(9600);
   //Start DHT 
     dht.begin();
   
   //Attaches the servo on pin 9 to the servo object 
     myServo.attach(9);  
     myServo.write(0);
   
   //Setting the WaterPump PinMode
      pinMode(WaterPumpPin,OUTPUT);
   //Setting the OUTPUT PIN for LED's
     pinMode(4,OUTPUT);
     pinMode(3,OUTPUT);
   
}

void loop()
{
  //Initially Set LED green turn ON and set Red to OFF
 // digitalWrite(3,LOW);
 // digitalWrite(4,HIGH);

   //getting the sensors values 
    //1-Reading Soil Moisture Sensor by calling it's Method
    Serial.println("<<<<------------>>>>");
    Serial.println("Reading soil Moisture....");
     ReadSoilMoisture();
    //2-Reading Light Sensor by calling it's Method
    Serial.println("////////////////////////");
    Serial.println("Reading Light Sensor....");
     ReadLDRLightSensor();
     //3-Reading Temperature and Humidity
    Serial.println("////////////////////////");
    Serial.println("Reading DHT Sensor....");
     ReadDHTSensor();
     //Monitor the water level in Gallon 
     MonitorGallon();
 
}

//Reading Soil MOisture from Sensor pins
void ReadSoilMoisture()
{
  //Plant 0
  int sumMoistureValue0=0;
  int avgMoistureValue0=0;
  //Plant 1
  int sumMoistureValue1=0;
  int avgMoistureValue1=0;
  //Plant 2
  int sumMoistureValue2=0;
  int avgMoistureValue2=0;
  
  
  //for Plant 0
   for(int i=0;i<3;i++)
   {
     //calculating 3 times in arow and summing it up
      moistureValueA0 = analogRead(0);
      sumMoistureValue0=sumMoistureValue0 + moistureValueA0;
   }
   avgMoistureValue0 = sumMoistureValue0/3;
   moistureValueA0 = avgMoistureValue0;
   
   //for Plant 1
   for(int i=0;i<3;i++)
   {
     //calculating 3 times in a row and summing it up
      moistureValueA1 = analogRead(1);
      sumMoistureValue1 = sumMoistureValue1 + moistureValueA1;
   }
   avgMoistureValue1 = sumMoistureValue1/3;
   moistureValueA1 = avgMoistureValue1;
  //for Plant 2
      for(int i=0;i<3;i++)
   {
     //calculating 3 times in a row and summing it up
      moistureValueA2 = analogRead(2);
      sumMoistureValue2 = sumMoistureValue2 + moistureValueA2;
   }
   avgMoistureValue2 = sumMoistureValue2/3;
   moistureValueA2 = avgMoistureValue2;
 
   //Checking the dryness of Soil and perform operation accordingly
  CalculateMoisture(moistureValueA0,moistureValueA1,moistureValueA2);
 
}


//Calculating SoilMoisture and perform operation accordingly
void CalculateMoisture(int moistureValueA0,int moistureValueA1,int moistureValueA2)
{
   //Checking the conditions to water the plant
   
   //for Plant 1
   if(moistureValueA0 < 100 )
   {
     //Turning green LED OFF and Red ON
     digitalWrite(4,LOW);
     digitalWrite(3,HIGH);
     //Set the Servo (Servo Method)
        AdjustServo(1);
     //after some delay turn on the water pump
         delay(1000);
        Serial.println("Turning Water PUMP ON");
        digitalWrite(WaterPumpPin,HIGH);
        //after some delay turn off the PUMP 4 second
        delay(3000);
        Serial.println("Turning Water PUMP OFF");
        digitalWrite(WaterPumpPin,LOW);
        
       //Again Set LED RED to OFF and green to ON
       //digitalWrite(3,LOW);
       //digitalWrite(4,HIGH);
       delay(500);
   }
   
   
   //for Plant 2
   if(moistureValueA1 < 100)
   {
     //Turning green LED OFF and Red ON
     digitalWrite(4,LOW);
     digitalWrite(3,HIGH);
     //Set the Servo to plant 2 (Servo Method)
     AdjustServo(2);
     
     //after some delay turn on the water pump
      delay(1000);
      Serial.println("Turning Water Pump ON");
      digitalWrite(WaterPumpPin,HIGH);
     // after some delay turn it off again
      delay(3000);
      Serial.println("Turning Water Pump OFF");
      digitalWrite(WaterPumpPin,LOW);
      
      //Again Set LED RED to OFF and green to ON
       //digitalWrite(3,LOW);
       //digitalWrite(4,HIGH);
       delay(500);
     
   }
   
   //for plant 3
    if(moistureValueA2 > 600)
   {
     Serial.println("Plant 3");
     //Turning green LED OFF and Red ON
     digitalWrite(4,LOW);
     digitalWrite(3,HIGH);
     
     //Set the Servo position to plant 3 (Servo Method)
     AdjustServo(3);
     
    //after some delay turn on the water pump
      delay(1000);
      Serial.println("Turning Water Pump ON");
      digitalWrite(WaterPumpPin,HIGH);
     // after some delay turn it off again
      delay(2000);
      Serial.println("Turning Water Pump OFF");
      digitalWrite(WaterPumpPin,LOW);
      
       //Again Set LED RED to OFF and green to ON
       //digitalWrite(3,LOW);
       //digitalWrite(4,HIGH);
     
   }
   else
   {
      digitalWrite(3,LOW);
      digitalWrite(4,HIGH);
   }
  
}


//Reading LDR Light Sensor

void ReadLDRLightSensor()
{
  LDRValue = analogRead(3);
   //Checking the value for dim light
     if(LDRValue < 200)
      {
        //Light is too dim
        //Turn LED red ON and green OFF
        digitalWrite(4,LOW);
        digitalWrite(3,HIGH);
        //calling function to send tweet
        //Alerting Buzzer
        BuzzerAlert();
        TwitterCall("Low Light",1,1);
        
      }
  //Printing the LDR Value
  Serial.print("Light : ");
  Serial.println(LDRValue);
  
}

void ReadDHTSensor()
{
    //Wait for few seconds
      delay(2000);
    //Reading humidity
    float humidity = dht.readHumidity();
    //Reading Temperature in Celcius
    float tempC = dht.readTemperature();
    //Reading Temperautre in Fahrenheit
    float tempF = dht.readTemperature(true);
    
    //Checking for read failure
    if(isnan(humidity) || isnan(tempC) || isnan(tempF))
    {
      Serial.println("Fail to read");
      return;
    }
    //Compute Heat Index
    //It takes 2 args 1- temperature in F 2-Humidity
    float heatIndex = dht.computeHeatIndex(tempF,humidity);
   
   // Printing Value on Serial Monitor
    Serial.println("---------------");
    Serial.println("Temperature and Humidity");
    Serial.print("Humidity: "); Serial.print(humidity); 
    Serial.print("\t");
    
    Serial.print("Temperature: "); Serial.print(tempC); Serial.print("*c ");
    Serial.print(tempF); Serial.print("*F\t");
    
    Serial.print("Heat Index: "); Serial.print(heatIndex);
    Serial.print(" *F");
    
    //Alerting for Critical temperature and humidity
    if(tempC > 35 || humidity > 75)
    {
      //Turn LED red ON and green OFF
        digitalWrite(4,LOW);
        digitalWrite(3,HIGH);
        //Call Buzzer Alert
        BuzzerAlert();
      TwitterCall("High Temp and Humidity",tempC,humidity);
    }
    
    //Turn Green LED to show plant is happy and red OFF
    digitalWrite(3,LOW);
    digitalWrite(4,HIGH);
    
    
}

//Servo Method

void AdjustServo(int x)
{
  int position=0;
  switch(x)
  {
    case 1 :
    //Instructing Servo to go to position at first plant
     Serial.println("Positioning servo to plant 1....");
   Serial.println("____________________________");
     myServo.write(30);              
    delay(500);           
    break;
    //For second plant
   case 2 :
   //Instructing Servo to go to position at second plant
    Serial.println("Positioning servo to plant 2....");
    Serial.println("____________________________");
   myServo.write(100);
   delay(500);
   break;
   //For third plant
   case 3 :
   //Instructing Servo to go to position at third plant
   Serial.println("Positioning servo to plant 3....");
   Serial.println("____________________________");
   myServo.write(150);
   delay(700);
   break;
   
   default :
   Serial.println("set to default");
  // break; 
  }
      
}

//Monitor Gallon 
void MonitorGallon()
{
  char msg[] = "Please Fill the Tank...";
  
  int waterLevel=0; 
  int sum=0;
  for(int i=0;i<3;i++)
  {
    waterLevel = analogRead(4);
    sum = sum + waterLevel;
  }
  if((sum/3)<500)
  {
    //Buzzer
    BuzzerAlert();
    TwitterCall(msg,0,0);
    exit(0);
  }
}

//Working with Ethernet
void TwitterCall(char msg[],int tempC,int humidity)
{
  delay(1000);
  //Begining Ethernet Shield
  Ethernet.begin(mac, ip);
 
  Serial.begin(9600);
  //Checking the incoming msg
  if(tempC==0 && humidity==0)
  {
    msg = "@amancse10 Please Fill the tank";
  }
  else
  {
    msg = "@amancse10 High Temp and Humidity : " ;
  }
  
  Serial.println("connecting ...");
  if (twitter.post(msg)) {
    // Specify &Serial to output received response to Serial.
    int status = twitter.wait(&Serial);
    if (status == 200) {
      Serial.println("OK.");
    } else {
      Serial.print("failed : code ");
      Serial.println(status);
    }
  } else {
    Serial.println("connection failed.");
  }
  
  delay(5000);
}

void BuzzerAlert()
{
  analogWrite(6,20);
  delay(2000);
  analogWrite(6,20);
  delay(2000);
  analogWrite(6,0);
}

