#include <ESP8266WiFi.h>
#include <AltSoftSerial.h>
AltSoftSerial esp;


String apiKey="52187757-2311-B6EF-FF31-67F227333200";
char  cloudServer[] = "api.backendless.com";
String cloudURI="/0B2BCA83-8E12-DBDA-FFAE-8ACD831D5600/52187757-2311-B6EF-FF31-67F227333200/data/kienlive";
const char* ssid = "sandeep_pradeep";
const char* password = "9884034225";
WiFiClient cloudClient;

int j;
char wt[10],weight[10];
String weight_url;
#define port 3333



void serialProcess(){
  //Clearing char arrays
  memset(wt,0,sizeof(wt));
  memset(weight,0,sizeof(weight));
  int i=0;
  
  while(esp.available()>0)
      {
        Serial.print("inside weight");
        char temp=esp.read();
        if(temp!='*')       
          {
            wt[i]=temp;
            i++;
          }
        
        else  
        {//* delimeter found     
          wt[i]='\0';
          int k;

          //Serial.print("wt:");
          //Serial.println(wt);
          
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
                //Serial.println("Inside dpoint:1");
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
                //Serial.println("Inside dpoint:2");
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
                //Serial.print("dpoint:");
                //Serial.println(dpoint);
                //Serial.print("WT:");
                //Serial.println(wt);
                //Serial.println("atOne:"+wt[k+1]);
                //Serial.println(" End");
              }
//                weight=wt.substring(k+1,k+5);
//                weight+='*';
            //Serial.print("Weight:");
            //Serial.println(weight);
             weight_url = String(weight);                        
             return;
          }//if wt[k]=='='
          else{
            //Serial.print("Invalid:");     
            //Serial.println(wt);
            //Serial.print("wt[k]=");
            //Serial.println(wt[k]);
            //Serial.print("k=");
            //Serial.println(k);                   
            return;
          }
        }
      }
}
void printWiFiStatus() {

  /*Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  
Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.print("Port opened at:");
  Serial.println(port);*/
}


void setup(void) {

  Serial.begin(9600);
 
  WiFi.begin(ssid, password);

  // Configure GPIO2 as OUTPUT.
//  pinMode(ledPin, OUTPUT);
  esp.begin(9600);
  // Start TCP server.
  //server.begin();

  //Serial.println("Module initialized");
  

}

//const char server[] = "embedded-iot.net"; 

void loop(void) {

  // Check if module is still connected to WiFi.
  
  
  if (WiFi.status() != WL_CONNECTED) {

    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
    }

    // Print the new IP to Serial.
    printWiFiStatus();
  }

  if(esp.available())
  {
    //Serial.println("serial data coming");
    serialProcess();
  }
   
    
  
  
  if (cloudClient.connect(cloudServer, 80)) {
   //Serial.println("connected to server");
   //WiFi.printDiag(Serial);
   int id=4678;
   Serial.print("inside cloud");
   Serial.println(weight_url);
   
  
  String data=String("{ \"kienid\" : \"") + String(id)+String("\"\,") + String(" \"liveweight\" : \"") + weight_url +String(" \" }");
   
      String lengthd=String(data.length());
     //change URL below if using your Sub-Domain
     cloudClient.println("POST "+ cloudURI +" HTTP/1.1"); 
     cloudClient.println("Host: api.backendless.com");                 
     cloudClient.println("Accept: */*");
     //client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n"); 
     cloudClient.println("Content-Length: "+lengthd);
     cloudClient.println("Content-Type: application/json");
     cloudClient.println();
     //client.print("\n\n");
     cloudClient.println(data);
     cloudClient.stop(); 
     
     //Serial.println("\n");
     //Serial.println("My data string im POSTing looks like this: ");
     //Serial.println(data);
     //Serial.println("And it is this many bytes: ");
     //Serial.println(data.length());       
     delay(2000);
        }
       /*
        data = "kienid=" + randp + "&liveweight=" + weight_urls;// data sent must be under this form //name1=value1&name2=value2.
        //http.begin("http://jsonplaceholder.typicode.com/users");
        http.begin("https://api.backendless.com/0B2BCA83-8E12-DBDA-FFAE-8ACD831D5600/52187757-2311-B6EF-FF31-67F227333200/data/kienlive");
        http.addHeader("Content-Type", "text/plain");
        Serial.println("Data : ");
        Serial.println(data);
        int httpCode = http.POST("kienid 4374 &liveweight 25.09");
        Serial.println(httpCode);
        String payload = http.getString();
        Serial.println(payload);
        http.end();
        Serial.println("hey uploaded to the url the weight data inside client");
        //Serial.println("Led is now on.");*/
       
}


    



