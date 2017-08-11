// char array version
//update: 5th may: Changed code for recieving msg from client. Changed ssid and password. WORKING AS OF NOW
#include <ESP8266WiFi.h>

const char* ssid = "SSN";
const char* password = "ssn12";
//String weight,wt;
int j;
char wt[10],weight[10];
const int ledPin = 2;
#define port 3333
WiFiServer server(port);

String message="";

void serialProcess(){
  //Clearing char arrays
  memset(wt,0,sizeof(wt));
  memset(weight,0,sizeof(weight));
  int i=0;
  
  while(Serial.available()>0)
      {
        
        char temp=Serial.read();
        if(temp!='*')       
          {
            wt[i]=temp;
            i++;
          }
        
        else  
        {//* delimeter found     
          wt[i]='\0';
          int k;

          Serial.print("wt:");
          Serial.println(wt);
          
          for(j=0;j<i;j++) //Noting the pos. of equal to sign.
          if(wt[j]=='=')
           {
            k=j;
            break;
           }
          
         if(wt[k]=='=') //confirming that kth character is =
            {              
              int dpoint = int(wt[k+1]); //obtaining decimal point info.
              if(dpoint==1)
              {
                Serial.println("Inside dpoint:1");
                k=2;
                for(j=0;j<=5;j++)
                {
                  weight[j]=wt[k];
                  k++;
                }
                weight[j]='*';
                weight[j+1]='\0';              
              }
              else if(dpoint==2)
              {
                Serial.println("Inside dpoint:2");
                k=2;
                for(j=0;j<=4;j++)
                {
                  weight[j]=wt[k];
                  k++;
                }
                weight[j]='*';
                weight[j+1]='\0';
              }
              else{
                Serial.print("dpoint:");
                Serial.println(dpoint);
                Serial.print("WT:");
                Serial.println(wt);
                Serial.println("atOne:"+wt[k+1]);
                Serial.println(" End");
              }
//                weight=wt.substring(k+1,k+5);
//                weight+='*';
            Serial.print("Weight:");
            Serial.println(weight);                        
             return;
          }//if wt[k]=='='
          else{
            Serial.print("Invalid:");     
            Serial.println(wt);
            Serial.print("wt[k]=");
            Serial.println(wt[k]);
            Serial.print("k=");
            Serial.println(k);                   
            return;
          }
        }
      }
}
void printWiFiStatus() {

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  
Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.print("Port opened at:");
  Serial.println(port);
}


void setup(void) {

  Serial.begin(9600);
 
  WiFi.begin(ssid, password);

  // Configure GPIO2 as OUTPUT.
  pinMode(ledPin, OUTPUT);

  // Start TCP server.
  server.begin();

  Serial.println("Module initialized");
  
}

void loop(void) {

  // Check if module is still connected to WiFi.
  if (WiFi.status() != WL_CONNECTED) {

    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
    }

    // Print the new IP to Serial.
    printWiFiStatus();
  }

  WiFiClient client = server.available();

  if(Serial.available())
   serialProcess();
  

  if (client) {
   
    Serial.println("Client connected.");
    client.flush();
    client.println("Hello Client! You have connected to ESP8266!*");

    while (client.connected()) {

      if(Serial.available())
      {
        serialProcess();
        client.flush();
        client.println(weight);
        //client.flush();
        //client.println(" Kg*");
      }
      
      if (client.available()> 0) {
         char command; 
        Serial.println("Reciving message!");
        
         while(client.available()>0)
       { command = client.read();
          message+=command;

        if (command == 'H') {

          digitalWrite(ledPin, HIGH);
          Serial.println("Led is now on.");
        }
        else if (command == 'L') {

          digitalWrite(ledPin, LOW);
          Serial.println("Led is now off.");
        }
      }
       if(message!="")
  {
    Serial.println("Message:"+message);
    message ="";
  }

    }//if client available
    }//while client connected
    Serial.println("Client disconnected.");
    client.stop();
  
  }  
}


    


