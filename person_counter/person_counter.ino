#include "arduino_secrets.h"
#include "thingProperties.h"

//LiquidCrystal_I2C lcd (0x3F, 20, 4); // 0 x 27

byte ir1 = 5; byte ir2 = 19;
byte personAlert=25; 

int count = 0; int count1 = 0; int personInside=0;
int temp1 = 0; int temp2 = 0;

byte fireAlert=12; byte gasAlert=13;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  pinMode(ir1, INPUT);
  pinMode(ir2, INPUT);
  pinMode(fireAlert, INPUT);
  pinMode(gasAlert, INPUT);
  pinMode(personAlert, OUTPUT);
 

  // Defined in thingProperties.h
  initProperties();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  
  /*
     The following function allows you to obtain more information
     related to the state of network and IoT Cloud connection and errors
     the higher number the more granular information you’ll get.
     The default is 0 (only errors).
     Maximum is 4
 */
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();

  digitalWrite(personAlert,LOW);
  fireAlert1=HIGH;
  gasAlert1=HIGH; 

}

void loop() {
  // put your main code here, to run repeatedly:

  ArduinoCloud.update();

  if ((digitalRead(ir1) == LOW) && (temp1 == 0) ) {

    temp1 = 1;

    if (temp2 == 0) {
      
      count++; count1++;
 
    }

  }

  if ((digitalRead(ir2) == LOW) && (temp2 == 0) ) {

    temp2 = 1;

    if (temp1 == 0) {
      count--; count1++;
      
    }

  }

  if ((temp1 == 1) && (temp2 == 1)) {

  delay(500);
    temp1 = 0; temp2 = 0;

    personInside=count;
   
  }

  if ((temp1 == 0) && (temp2 == 0)) {

    personInside=count;

  }

  //Update Cloud Person Alert


  if(personInside>0)
    {
      digitalWrite(personAlert,HIGH); 
      personAlert1=HIGH; 
      //ArduinoCloud.update();
      }
   else
   {
      digitalWrite(personAlert,LOW); 
      personAlert1=LOW;
      //ArduinoCloud.update(); 
      }

      Serial.println(personInside);
      Serial.print("Count: ");
      Serial.println(count); 
      
//Update Cloud Fire Alert

      if(digitalRead(fireAlert))
    {
      fireAlert1=LOW; 
      ArduinoCloud.update();
      Serial.println("Fire");
      }
   else
   {
      fireAlert1=HIGH; 
      //ArduinoCloud.update(); 
      }

//Update Cloud Gas Alert

      if(digitalRead(gasAlert))
    {
      gasAlert1=LOW; 
      ArduinoCloud.update();
      }
   else
   {
      gasAlert1=HIGH; 
      //ArduinoCloud.update(); 
      }
  }
  


void onResetChange()  {
  // Add your code here to act upon Reset change

  if (reset)
  {
      personInside=0;
      count=0; 
      Serial.println("Reset");  
  } 
      
}
