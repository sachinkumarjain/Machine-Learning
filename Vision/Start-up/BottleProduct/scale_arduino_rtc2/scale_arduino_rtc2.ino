//update-9-01-16: sendESP command prevents reading weight value from second time. hence those two lines were commented out.
//update:9-01-16. Bug fix: It is found that strTrunc does not append one or two according to the decimal place.Because the second statement was = than +=. hence dpoint was not appeneded.
//update:9-01-16: Bug-fix: sendWeight function was defined with a return type string while it does not return anything. return type made void.
//update:29-01-16:This is the i2c version of scale_arduino program. Code has been modified to interface lcd via i2c.
//                Also updated to display weight as 0.00 initially and display the unit Kg at all times.
// Update 3: Added code to display ssid on lcd, before ending setup
//update:24-07-16: 1.Obtaining stable weight is consolidated inside a function for multiple calls. 2.Real-time weight update added
//update:15-01-17: Added timeout during weight stabilization 
//update:17-5-17: Removed lcd library for bottle project
//update:28-05-17: added initialization codes for rtc on setup routines
//update:24-06-17: added alarm feature

#include "HX711.h"
#include <AltSoftSerial.h>
#include <Wire.h>
#include <DS3231_Simple.h>

AltSoftSerial bt; //8,9||RX,TX
DS3231_Simple rtc;
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

HX711 scale(11,12);		// parameter "gain" is ommited; the default value 128 is used by the library
int scaleVal = 275, bottleWeight = 56, lastWeight =0, waterLevel;
float avg =0.0,currentWeight=0.0,tareWeight=0.0;
boolean upload = true;
unsigned long int up;
const int tarePin =2;
boolean led = LOW,alarmActive = false; 
int buzzer = 10;

boolean newData = false;
char msg[20];

struct myLogData{
  unsigned int waterChange,flow;
};

 String recvData()
  {
    String str = "";
    
     if(bt.available())
   {     
        char temp; boolean recvProgress = false;
       static byte i=0; 
       
        while(bt.available()>0 && newData == false)
        {
           //Serial.println("inside first wile");
           delay(1);  // very important. do not delete this line
           temp = bt.read();             

           if(recvProgress == true)
           {
                    if(temp!='#')
                   { 
                    //msg[i]=temp;
                      str+=temp;
                      i++;
                   }
                  else
                  {
                    //msg[i]='\0';
                    str+='\0';
                    i=0;
                    recvProgress = false; 
                    newData = true;
                  }
            }                                       

           else if(temp == '*')
           {
            Serial.println("Incoming data..");
            recvProgress = true;
           }

           delay(1);                                  
       }
      Serial.print("Received string: ");
      Serial.println(str);      
      newData = false;
    }

  return str;  
  }

  void printBytes(const byte data[],int len, String text = "Bytes")
  {
     Serial.print(text+":");
    for(int i=0;i<len;i++)
      Serial.print(char(data[i]));
      Serial.println();
  }

  void sendBytes(String cmd) //sending data to android
  {
     int len = cmd.length();

      byte data[len+1];
      cmd.getBytes(data,len+1);
      
     for(int i=0;i<len;i++)
      bt.write(data[i]);
  }
 void sendData(){   //sending log data to android

  DateTime logTime;
  myLogData record;
  unsigned int logData,count=0;
  boolean flag;
  
  unsigned long int curTime = millis();
  
  String waterLog [10];

  
  //rtc.writeLog(analogRead(A1));

  while(rtc.readLog(logTime, record))
  {
    Serial.print("Log No: ");
    Serial.println(count+1);
    Serial.println("-------------------");
    rtc.printTo(Serial,logTime);
    Serial.print(',');
    Serial.println(logData);

    waterLog[count]="*";
    waterLog[count] += rtc.getStringDMY(logTime);
    waterLog[count]+="W";
    int dir = record.flow==0?0:1;
    waterLog[count]+=String(record.waterChange);
    waterLog[count]+="/";
    waterLog[count]+=dir;
    waterLog[count]+='#';
    Serial.println("waterLog:"+waterLog[count]);

    count++;
  }



  if(count == 0)
  Serial.println("No records found!!");

   else{
    int k = 0; String len ="$";
    len+=count;
    len+='#';
    sendBytes(len); //sending data count to android
//    byte cnt[3];
//    len.getBytes(cnt,4);
//    printBytes(cnt,3,"records");
//    for(int j=0;j<len.length();j++)
//      bt.write(cnt[j]);
    
   for(k=0;k<count;k++)     
  { Serial.print("Size:" );
    Serial.println(sizeof(waterLog[k]));
    Serial.print("Length:");

    int len = waterLog[k].length();
    Serial.println(len);

    String part="*";
    part+= waterLog[k].substring(19,waterLog[k].length());
    Serial.println("part2: "+part);


    byte data [20], sendByte[20];
    
    String str = waterLog[k].substring(0,(waterLog[k].length())-1);
    waterLog[k].getBytes(data,20);

    for(int i=0;i<19;i++)
    sendByte[i] = data[i];
    
    sendByte[19]='+'; // 20th char

    Serial.print("bytes:");
     for(int i=0;i<20;i++)
        Serial.print(char(sendByte[i]));
    Serial.println();
    

    for(int i =0; i<20;i++)
     bt.write(sendByte[i]);

    boolean flag = false;
    
     while(1)
     {
       String recvd = recvData();
       if(recvd == "ack")
        {
          flag = true;
          break;
        }
       Serial.println("waiting for ack..."); 
     }

     if(flag)
     {
        part.getBytes(data, part.length()+1);

        printBytes(data, part.length());

        for(int i=0;i<part.length();i++)
          bt.write(data[i]);             
     }
  }  
    
  }//else if records present

 }

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

int getWaterLevel()
{
  double temp = scale.read_average(10);
  float bottle = (146600-abs(temp))/scaleVal;
  int waterLvl = bottle-bottleWeight;

  return int(waterLvl);
}

int get_weight(int lweight)
{
  int cweight;
  while(1)  //wait for a stable weight
    {
      Serial.println("Waiting for stable weight");      
      cweight = getWaterLevel();
      int diff = cweight - lweight;
      Serial.println(diff);
      if( abs(diff)> 1)      
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
 scale.tare();
}

void setBottleAlarm(int sec)
{
  DateTime MyTimestamp = rtc.read();              
  
  // We want the alarm at this second
  MyTimestamp.Second   = sec;                       
  
  // And now add the alarm to happen when the second matches
  rtc.setAlarm(MyTimestamp, DS3231_Simple::ALARM_MATCH_SECOND); 
}

void setup() {
  Serial.begin(9600);  
  Serial.println("module initialized");

   pinMode(13,OUTPUT);
   digitalWrite(13,LOW);
   
//  pinMode(tarePin,INPUT_PULLUP);
//  attachInterrupt(digitalPinToInterrupt(tarePin),tare,RISING);
         
  bt.begin(9600);
  
  Serial.println("initializing rtc module..");
  rtc.begin();
  rtc.formatEEPROM();

  rtc.printTo(Serial);
  Serial.println();

  Serial.println("Initializing load sensor.. ");
  // ideal 19500
  scale.set_scale(scaleVal);  // this value is obtained by calibrating the scale with known weights; see the README for details (19500 for kien) //305 old value
  scale.tare();				        // reset the scale to 0

  Serial.println("After setting up the scale:");

  Serial.print("read: \t\t");
  Serial.println(scale.read());                 // print a raw reading from the ADC

   double rawVal = scale.read_average(20);
  Serial.print("read average: \t\t");
  Serial.println(rawVal);       // print the average of 20 readings from the ADC

  if(abs(rawVal)<140000)
  {
    Serial.println("Bottle present!!");
    if(abs(rawVal)<130000)
    {
      Serial.print("bottle has:");
      waterLevel = getWaterLevel();
      Serial.print(waterLevel);      
      Serial.println(" ml");
    }
    else Serial.println("Bottle doesnt have water!");
    
 }


  Serial.print("get value: \t\t");
  Serial.println(scale.get_value(5));		// print the average of 5 readings from the ADC minus the tare weight, set with tare()

  Serial.print("get units: \t\t");
  Serial.println(scale.get_units(5), 1);        // print the average of 5 readings from the ADC minus tare weight, divided 
						// by the SCALE parameter set with set_scale

  Serial.println("Readings:");  
  
}

void loop() {
 // avg = scale.get_units(10);
   uint8_t AlarmsFired = rtc.checkAlarms();
   if(AlarmsFired & 1)
   {
    Serial.println("Alarm triggered!");
    digitalWrite(13,HIGH);
    alarmActive = true;
   }
   
   if(abs(getWaterLevel() - waterLevel) > 1)
  {
     if(alarmActive)
     {
       Serial.println("alarm reset!");
       alarmActive = false;
       digitalWrite(13,LOW);
     }
    
    Serial.print("Water change:");
    Serial.println(getWaterLevel());
    
    if(getWaterLevel()<0)
    {
      Serial.println("Bottle has been removed!");
      lastWeight = waterLevel;
     
      while(1)
     { 
      if(getWaterLevel()>0)
       {
        Serial.println("bottle back!");
        waterLevel = get_weight(waterLevel); 
        Serial.print(lastWeight - waterLevel);
        Serial.println(" ml was removed!");
        myLogData temp;
        temp.waterChange=(lastWeight - waterLevel);
        temp.flow=1;
        rtc.writeLog(temp);
        Serial.println("Water change logged!");
        Serial.print("Water Level:");
        Serial.print(waterLevel);
        Serial.println(" ml");
        break;
       }
     }
    } 
   else
   {
     Serial.println("Bottle in use!");
    //currentWeight = scale.get_units(10);
    lastWeight = waterLevel;

    waterLevel=get_weight(lastWeight);
    Serial.println("Stable weight obtained!");
    
    Serial.print("Read raw:");
       Serial.println(scale.read_average(10));

   int waterChange = waterLevel - lastWeight;
   
   Serial.print(waterChange);
        Serial.println(" ml was added!");

        myLogData temp;
        temp.waterChange=waterChange;
        temp.flow=0;
        rtc.writeLog(temp);


   Serial.println("Weight change logged!");

//   String msg=strTrunc(currentWeight);
//    sendWeight(msg);

    
    Serial.print("Bottle has:");
    Serial.print(waterLevel);
    Serial.println(" ml!");
    
    
//    unsigned long int timeOut=millis();
//    
//    while(1) //wait till the current load is removed.
//    {
//      avg=scale.get_units(10);
//      
//      if(avg-currentWeight>=1.0 || avg-currentWeight<=-1.0)
//      {
//        avg=scale.get_units(10);
//          currentWeight=get_weight(avg);          
//         Serial.print("Weight:");
//         Serial.println(currentWeight);
//                  
//      if(currentWeight<1.0)
//      {
//       Serial.println("Some one stepped out!");
//       break;  
//      }
//      else
//      {
//        rtc.writeLog(int(currentWeight));
//         Serial.println("Weight Logged!");
//      
//      }
//   }  
//      
//    if(millis()-timeOut>=10000)
//      {
//        Serial.println("Timeout!");
//        break;
//      }
//    }
//      
//    upload = true;
//    scale.tare();
//    avg=scale.get_units(10);
//    scale.tare();
//    avg=scale.get_units(5);
//    
//    Serial.print("Weight:");
//  Serial.println(avg);
// }  
  }
 }   
   if ( bt.available() )   
    {  
      String str  = recvData();
      if(str == "sync")
        {
          String cmd ="%ack#";
          sendBytes(cmd);
          sendData();
        } 
      if(str == "alarm")
      {
         Serial.println("Alarm request obtained!"); 
         String cmd ="%ack#";
         sendBytes(cmd);
         String recvd;
         while(1)
         {
            Serial.println("Waiting for alarm value!");
            recvd = recvData();
            if(recvd.toInt()>=1)
            break;
            else
            {
              Serial.println("Invalid number!");
            }
            
         }
         int alarmVal = recvd.toInt();
           if(alarmVal == 0)
           rtc.disableAlarms();
           else
           {
            Serial.print("Alarm set for: ");
            Serial.print(alarmVal);
            Serial.print(" sec(s)");
            setBottleAlarm(alarmVal);
           } 
      }
    }    
}
