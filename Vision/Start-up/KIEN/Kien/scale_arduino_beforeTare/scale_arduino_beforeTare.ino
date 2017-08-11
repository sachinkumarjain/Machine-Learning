//update-9-01-16: sendESP command prevents reading weight value from second time. hence those two lines were commented out.
//update:9-01-16. Bug fix: It is found that strTrunc does not append one or two according to the decimal place.Because the second statement was = than +=. hence dpoint was not appeneded.
//update:9-01-16: Bug-fix: sendWeight function was defined with a return type string while it does not return anything. return type made void.
//update:29-01-16:This is the i2c version of scale_arduino program. Code has been modified to interface lcd via i2c.
//                Also updated to display weight as 0.00 initially and display the unit Kg at all times.
// Update 3: Added code to display ssid on lcd, before ending setup
//update:24-07-16: 1.Obtaining stable weight is consolidated inside a function for multiple calls. 2.Real-time weight update added
//update:15-01-17: Added timeout during weight stabilization 

#include "HX711.h"
#include <AltSoftSerial.h>
#include <Wire.h>
//#include <LCD.h>
#include <LiquidCrystal_I2C.h>
//#include <DS3231_Simple.h>


LiquidCrystal_I2C  lcd(0x27,2,1,0,4,5,6,7); // 0x27 is the I2C bus address for an unmodified module
AltSoftSerial esp; //8,9||RX,TX

/*
 * This uses the Liquid Crystal library from https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads GNU General Public License, version 3 (GPL-3.0)
 * Pin Connections: 
 *      SCL = A5
 *      SDA = A4
 *      VCC = 5V
 *      GND = GND
 */
  
// HX711.DOUT	- pin #A1 or 6
// HX711.PD_SCK	- pin #A0  or 7

HX711 scale(5,6);		// parameter "gain" is ommited; the default value 128 is used by the library
float avg =0.0,currentWeight=0.0,lastWeight=0.0,tareWeight=0.0;
boolean upload = true;
unsigned long int up;
const int tarePin =2;
boolean led = LOW;
//int calib = 23100;

String strTrunc(float wt)
{
  Serial.println("Truncating weight value");
  int tmp;
    String number;
    if(wt>10.00)
    {
       tmp = wt*100;
       String val(tmp);
       number+='1';       
       number+=val.substring(0,2);
      number+='.';
      number+=val.charAt(2);      
    }
    else if(wt<10.00)
    {
       tmp = wt*100;
       String val(tmp);
       number+='2';
       number+= val.charAt(0);
      number+='.';
      number+=val.charAt(1);      
    }
    Serial.println(number);
  return number;
}
void sendWeight(String msg)
{
  Serial.println("inside send weight");
  String weight;
  weight+='=';
  weight+=msg;
  weight+='*';
  Serial.print("weight:");
  Serial.println(weight);
  esp.println(weight);
  Serial.println("Weight sent to ESP");
  }

  void lcdWeight(float weight)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Weight:");
    lcd.setCursor(0,1);
    lcd.print(weight);
    lcd.setCursor(7,1);
    lcd.print("Kg");
  }

float get_weight(float lweight)
{
  float cweight;
  while(1)  //wait for a stable weight
    {
      Serial.println("Waiting for stable weight");      
      cweight = scale.get_units(10);
      float diff = cweight - lweight;
      Serial.println(diff);
      if( diff>1.0)      
         { lweight = cweight;
           Serial.print("Diff: ");
           Serial.println(diff);
          delay(500);
         }
         else
         break;
    }
  return cweight;
}

void tare()
{
  //Serial.println("Tare Pressed!");
 scale.tare();
//  float temp = scale.get_units(10);
 // lcdWeight(0.00);
//    digitalWrite(13,!led);
//    led=!led;
}
void setup() {
  Serial.begin(9600);

   pinMode(13,OUTPUT);
   
 // pinMode(tarePin,INPUT_PULLUP);
 // attachInterrupt(digitalPinToInterrupt(tarePin),tare,RISING);

    
//    lcd.setBacklightPin(3,POSITIVE);
  //  lcd.setBacklight(HIGH); // NOTE: You can turn the backlight off by setting it to LOW instead of HIGH
    lcd.begin(16, 2);
    lcd.clear();  
    
    esp.begin(9600);

  lcd.print("Wifi Scale!!");
  delay(2000);
  lcd.clear();
  lcd.print("Initializing...");
  
// ideal 19500
  scale.set_scale(305);  // this value is obtained by calibrating the scale with known weights; see the README for details (19500 for kien)
  scale.tare();				        // reset the scale to 0

  Serial.println("After setting up the scale:");

  Serial.print("read: \t\t");
  Serial.println(scale.read());                 // print a raw reading from the ADC

  Serial.print("read average: \t\t");
  Serial.println(scale.read_average(20));       // print the average of 20 readings from the ADC

  Serial.print("get value: \t\t");
  Serial.println(scale.get_value(5));		// print the average of 5 readings from the ADC minus the tare weight, set with tare()

  Serial.print("get units: \t\t");
  Serial.println(scale.get_units(5), 1);        // print the average of 5 readings from the ADC minus tare weight, divided 
						// by the SCALE parameter set with set_scale

  Serial.println("Readings:");
  lcd.clear();
  lcd.print("Wifi Connected!");
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("IP:192.168.1.10");
  lcd.setCursor(0,1);
  lcd.print("SSID: Shankar");
  delay(2000);
  scale.tare();
  lcdWeight(0.00);
  
}

void loop() {

//  scale.set_scale(calib);
  
//  Serial.print("one reading:\t");
//  Serial.print(scale.get_units(), 1);
//  Serial.print("\t| average:\t");  
  avg = scale.get_units(10);
  if(avg>=1.0)
  {
    Serial.println("Some one stepped in!");
    currentWeight=0.0,lastWeight=0.0;
    currentWeight = scale.get_units(10);
    lastWeight = currentWeight;
   
//    while(1)  //wait for a stable weight
//    {
//      Serial.println("Waiting for stable weight");      
//      currentWeight = scale.get_units(10);
//      float diff = currentWeight - lastWeight;
//      Serial.println(diff);
//      if( diff>1.0)      
//         { lastWeight = currentWeight;
//           Serial.print("Diff: ");
//           Serial.println(diff);
//          delay(500);
//         }
//         else
//         break;
//    }

    currentWeight=get_weight(lastWeight);
    Serial.println("Stable weight obtained!");

   String msg=strTrunc(currentWeight);
    sendWeight(msg);

    Serial.print("Weight:");
    Serial.println(currentWeight);
    lcdWeight(currentWeight);

    unsigned long int timeOut=millis();
    
    while(1) //wait till the current load is removed.
    {
      avg=scale.get_units(10);
      if(avg-currentWeight>=1.0 || avg-currentWeight<=-1.0)
      {
        currentWeight=get_weight(avg);
        lcdWeight(currentWeight);
      }
      if(avg<0.2)
      {
       Serial.println("Some one stepped out!");
       break;  
      }
      if(millis()-timeOut>=10000)
      {
        Serial.println("Timeout!");
        break;
      }
    }
      
    upload = true;
    scale.tare();
    avg=scale.get_units(10);
    scale.tare();
    avg=scale.get_units(5);
    
    Serial.print("Weight:");
  Serial.println(avg);
  lcdWeight(avg);
   
  }
  

  
    if ( esp.available() )   
    {  
      while(esp.available()>0)
      Serial.write(esp.read());
    }
    
  

}
