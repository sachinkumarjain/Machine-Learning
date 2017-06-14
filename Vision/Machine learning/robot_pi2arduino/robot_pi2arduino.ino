//#include <LiquidCrystal.h>

//LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int lm1= 7 ;        //setting up left motor pins
int lm2= 6;
int rm1= 8;        //setting up right motor pins
int rm2= 9;

// ultrasonic 

const int trigPin = 10;
const int echoPin = 11;

long duration;
int distance;




void Forward()     //Forward
{
  //lcd.print(" Moving Forward");
  digitalWrite(lm1,1);
  digitalWrite(lm2,0);
  digitalWrite(rm1,1);
  digitalWrite(rm2,0);
  
}

void Stop()        //Stop
 {
  //lcd.print("Stopped ");
  digitalWrite(lm1,0);
  digitalWrite(lm2,0);
  digitalWrite(rm1,0);
  digitalWrite(rm2,0);
}
void Reverse()     //Reverse
{
  //lcd.print("Reversing");
  digitalWrite(lm1,0);
  digitalWrite(lm2,1);
  digitalWrite(rm1,0);
  digitalWrite(rm2,1);
}
void Right()        //Right 
{
  //lcd.print("Moving Right");
  digitalWrite(lm1,1);
  digitalWrite(lm2,0);
  digitalWrite(rm1,0);
  digitalWrite(rm2,1);
}
void Left()         //Left
{
  //lcd.print("Moving Left");
  digitalWrite(lm1,0);
  digitalWrite(lm2,1);
  digitalWrite(rm1,1);
  digitalWrite(rm2,0);
}


 
 
void setup()
{
   /*  lcd.begin(16,2);
   lcd.clear();
   lcd.print("IRON MAN");
   delay(2000);*/
   pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
   pinMode(echoPin, INPUT); // Sets the echoPin as an Input
   Serial.begin(9600);
   pinMode(lm1,OUTPUT);
   pinMode(lm2,OUTPUT);
   pinMode(rm1,OUTPUT);
   pinMode(rm2,OUTPUT);
  
   
}
int distance_sensor()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

// Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

// Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);

// Calculating the distance
 distance= duration*0.034/2;

// Prints the distance on the Serial Monitor
return distance
}
void loop()
{
   if(Serial.available())
   {
      //lcd.clear();
      
      String c =Serial.read();
      //lcd.print(c);
      //delay(1000);
      switch (c)
      {
         case 'forward':
               Forward();
               break;
               
         case 'left':
               Left();
               break;
         case 'right':
               Right();
               break;
         
          case 'reverse':
               Reverse();
               break;
        
         
      }
      
      int objectDistance= distance_sensor();

      if(objectDistance < 5) 
      {
        Stop();
      }
   }
   
  else
    Stop();

    
}               
