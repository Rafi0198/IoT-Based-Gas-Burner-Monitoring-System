#include<Wire.h>
#include<DHT.h>
#include<ESP32Servo.h>
#define dhtPin 4
#define dhtType DHT11
#include<SoftwareSerial.h>
#include<LiquidCrystal_I2C.h>
#include<WiFi.h>
#include<ESP_Mail_Client.h>
#include "arduino_secrets.h"
#include "thingProperties.h"

LiquidCrystal_I2C lcd (0x27, 20, 4);

SoftwareSerial gsm (16, 17); //(3,2) Rx, Tx

DHT dht (dhtPin, dhtType);

Servo servo;


byte gas = 34;    byte gas_bulb = 14;  byte gas_buzz = 27; // pin gasbuzz
byte fire = 25;   byte fire_bulb = 33; byte fire_buzz = 32; // pin fire buzz
byte servoPin = 13; byte person_alert = 18;
float roomTemp;   byte humidity;

byte fireAlert = 23; byte gasAlert = 26;
byte EXTIPin = 5;   byte led = 19;

byte count1 = 0;   byte count2 = 0;
byte burningRate;  byte up = 15;   byte down = 2;
byte servoWrite = 0;
byte count3 = 0; byte count4 = 0; bool cond3; long ct;

String msgFire = "Warning! Gas burner running!" ;
String msgGas = "Warning! Gas leak detected from the burner!" ;

// WiFi Credinetials

const char* ssid = "EEE" ;
const char* password = "qnh1013hg" ;

// Email Credinetials

#define SMTP_server "smtp.gmail.com"

#define SMTP_Port 465

#define sender_email "rehanaakterrumi71@gmail.com"

#define sender_password "sjuhbwjneoglsaab"

#define Recipient_email "hoquerafi727@gmail.com"

#define Recipient_name "THR"

SMTPSession smtp;

//void IRAM_ATTR ledOn() {
//
//    digitalWrite(led,HIGH);
//    //Serial.println("EXTI generated");
//
//  }

void setup() {
  // put your setup code here, to run once:

  pinMode(gas, INPUT);
  pinMode(gas_bulb, OUTPUT);
  pinMode(gas_buzz, OUTPUT);
  pinMode(gasAlert, OUTPUT);
  pinMode(fire, INPUT);
  pinMode(fire_bulb, OUTPUT);
  pinMode(fire_buzz, OUTPUT);
  pinMode(fireAlert, OUTPUT);
  pinMode(person_alert, INPUT);
  pinMode(up, INPUT);
  pinMode(down, INPUT);
  //pinMode(led,OUTPUT);
  //pinMode(EXTIPin, INPUT_PULLDOWN);
  //attachInterrupt(digitalPinToInterrupt(EXTIPin),ledOn, RISING);


  servo.setPeriodHertz(50);
  servo.attach(servoPin, 500, 2400);
  servo.write(0);


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

  dht.begin();
  Serial.begin(9600);
  gsm.begin(9600);
  WiFi.begin(ssid , password);

  Serial.println("Initializing Netwrok...");

  gsm.println("AT");
  checkSerial();

  gsm.println("AT+CSQ");
  checkSerial();

  gsm.println("AT+CCID");
  checkSerial();

  gsm.println("AT+CREG?");
  checkSerial();

  gsm.println("AT+CBC");
  checkSerial();

  gsm.println("AT+COPS?");
  checkSerial();

  gsm.println("AT+CMGF=1"); // Initializes the text mode
  checkSerial();

  gsm.println("AT+CNMI=2,2,0,0,0"); // Decides how newly arrived messages will be handled
  checkSerial();

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("WELCOME TO RAFI'S");
  lcd.setCursor(0, 1);
  lcd.print("GAS BURNER MONITOR");
  lcd.setCursor(0, 2);
  lcd.print("UNDERGRAD STUDENT");
  lcd.setCursor(0, 3);
  lcd.print("DEPT. OF EEE, BUET");
  delay(3000);

  // Connect to Wi-Fi
  //  while (WiFi.status() != WL_CONNECTED) {
  //    delay(1000);
  //    Serial.println("Connecting to WiFi...");
  //  }

  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  lcd.clear();

  digitalWrite(fire_bulb, LOW);
  digitalWrite(gas_bulb, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:

  ArduinoCloud.update();

  roomTemp = dht.readTemperature();
  humidity = dht.readHumidity();
  //digitalWrite(led,LOW);

  roomTemp1 = roomTemp;
  humidity1 = humidity;

  //Burning Rate

  burningRate = map(servo.read(), 0, 180, 0, 100);
  burningRate1 = burningRate;

  lcd.setCursor(0, 0);
  lcd.print("BURNING RATE:");
  lcd.print(burningRate);
  lcd.print("%    ");

  lcd.setCursor(0, 3);
  lcd.print("T&H: ");
  lcd.print(roomTemp);
  lcd.write(223);
  lcd.print("C & ");
  lcd.print(humidity);
  lcd.print("%");

  /*Serial.print("Temperature is " );
    Serial.print(roomTemp);
    Serial.write(227);
    Serial.println("C");

    Serial.print("Humidity is " );
    Serial.print(humidity);
    Serial.println("%");
  */


  // Controlling the Burner Manually

  if (digitalRead(up) && servoWrite < 180)
  {
    servoWrite = servoWrite + 20;
    servo.write(servoWrite);

  }

  else if (digitalRead(down) && servoWrite > 0)
  {
    servoWrite = servoWrite - 20;
    servo.write(servoWrite);

  }

  // Condition3 by Sir during 1st Update

  cond3 = !(digitalRead(person_alert)) && (digitalRead(fire)) && burningRate;

  if (cond3)

  {

    Serial.println("cond3");

    if (count3 == 0)
    {
      ct = millis() / 1000;
      count3 = 1;

    }

    if (((millis() / 1000) - ct) > 10)

    {

      digitalWrite(fire_bulb, HIGH);
      digitalWrite(fire_buzz, HIGH);
      digitalWrite(fireAlert, HIGH);

      count4 = 1;

      Serial.println(((millis() / 1000) - ct));

    }

    //     else
    //
    //      {
    //
    //        if (count4==1)
    //
    //          {
    //            digitalWrite(fire_bulb,LOW);
    //            digitalWrite(fire_buzz,LOW);
    //            digitalWrite(fireAlert,LOW);
    //
    //            count4=0;
    //
    //            }
    //
    //        }

  }

  else

  {
    count3 = 0;

    // if (count4==1)

    //{
    digitalWrite(fire_bulb, LOW);
    digitalWrite(fire_buzz, LOW);
    digitalWrite(fireAlert, LOW);

    count4 = 0;

    //}



  }


  //Fire Alert

  if (digitalRead(fire) == LOW) {

    count1 = 1;

    //Fire Alert in Absence of Human
    if (digitalRead(person_alert) == LOW)
    {

      //personAlert=HIGH;

      digitalWrite(fire_bulb, HIGH);
      digitalWrite(fire_buzz, HIGH);

      digitalWrite(fireAlert, HIGH);

      servo.write(0);

      lcd.setCursor(0, 0);
      lcd.print("      CAUTION!      ");
      lcd.setCursor(0, 1);
      lcd.print("   BURNER RUNNING   ");

      byte i;

      for (i = 0; i < 5; i++)
      {
        digitalWrite(fire_bulb, HIGH);
        digitalWrite(fire_buzz, HIGH);
        delay(800);
        digitalWrite(fire_bulb, LOW);
        digitalWrite(fire_buzz, LOW);
        delay(800);

      }

      lcd.setCursor(0, 1);
      lcd.print("SENDING EMAIL...    ");
      lcd.setCursor(0, 2);
      lcd.print("PLEASE WAIT...      ");
      sendmail(msgFire);

      lcd.setCursor(0, 1);
      lcd.print("SENDING SMS...      ");
      sms(msgFire);

      lcd.clear();
      count1 = 1;

    }

    else

      //Fire Alert in Presence of Human

    {
      digitalWrite(fire_bulb, HIGH);
      //digitalWrite(fire_buzz, HIGH);
      //personAlert=LOW;

      digitalWrite(fireAlert, HIGH);

      lcd.setCursor(0, 0);
      lcd.print("      CAUTION!      ");
      lcd.setCursor(0, 1);
      lcd.print("   BURNER RUNNING   ");

      count1 = 1;
      delay(500);
      lcd.clear();

    }
  }

  else

  {
    if (count1 == 1)
    {
      digitalWrite(fire_bulb, LOW);
      digitalWrite(fire_buzz, LOW);
      digitalWrite(fireAlert, LOW);
      count1 = 0;
      //Serial.println("Fire Bulb Low");
    }
  }

  // Gas Leak Alert

  if (digitalRead(gas) == LOW) {

    //count1=1;

    //Gas Leak Alert in Absence of Human
    if (digitalRead(person_alert) == LOW)
    {
      //personAlert=HIGH;
      digitalWrite(gas_bulb, HIGH);
      digitalWrite(gas_buzz, HIGH);

      digitalWrite(gasAlert, HIGH);

      lcd.setCursor(0, 0);
      lcd.print("      CAUTION!      ");
      lcd.setCursor(0, 1);
      lcd.print("  Gas Leak Detected!   ");

      byte i;

      for (i = 0; i < 5; i++)
      {
        digitalWrite(gas_bulb, HIGH);
        digitalWrite(gas_buzz, HIGH);
        delay(800);
        digitalWrite(gas_bulb, LOW);
        digitalWrite(gas_buzz, LOW);
        delay(800);

      }

      lcd.setCursor(0, 1);
      lcd.print("SENDING EMAIL...    ");
      lcd.setCursor(0, 2);
      lcd.print("PLEASE WAIT...      ");
      sendmail(msgGas);

      lcd.setCursor(0, 1);
      lcd.print("SENDING SMS...      ");
      sms(msgGas);

      lcd.clear();
      count1 = 1;
      servo.write(0);
    }

    else

    {

      //Gas Alert in Presence of Human

      digitalWrite(gas_bulb, HIGH);
      digitalWrite(gasAlert, HIGH);

      lcd.setCursor(0, 0);
      lcd.print("      CAUTION!      ");
      lcd.setCursor(0, 1);
      lcd.print("  Gas Leak Detected!");
      delay(500);
      lcd.clear();

      //count1=1;

    }
  }

  else

  {

    //if(count1==1)
    //{
    digitalWrite(gas_bulb, LOW);
    digitalWrite(gas_buzz, LOW);
    digitalWrite(gasAlert, LOW);
    //count1=0;
    //}
  }


}


void onBurnerStateChange()  {
  // Add your code here to act upon BurnerState change

  if (burningRate)
  {
    servo.write(map(burnerState, 0, 100, 0, 180));
  } 
}

void checkSerial() {

  delay(500); // Used to ensure enough lag time between the at commands

  //  while (Serial.available())
  //    gsm.write(Serial.read());

  while (gsm.available())
    Serial.write(gsm.read());

}

void sms(String MSG)  {

  gsm.println("ATD+8801988448287;"); // Dials the Destination Number ***Make Call Prior to Sending SMS
  checkSerial();

  delay(15000);

  gsm.println("ATH"); // Hangs up the call after 20 Seconds
  checkSerial();

  gsm.println("AT+CMGF=1");
  checkSerial();

  gsm.println("AT+CMGS=\"+8801988448287\""); // Set Destination Phone Number
  checkSerial();

  gsm.println(MSG); // Set Message Content
  checkSerial();

  gsm.write(26);

}

// Send email

void sendmail(String msg) {

  smtp.debug(1);

  ESP_Mail_Session session;

  session.server.host_name = SMTP_server ;

  session.server.port = SMTP_Port;

  session.login.email = sender_email;

  session.login.password = sender_password;

  session.login.user_domain = "";

  /* Declare the message class */

  SMTP_Message message;

  message.sender.name = "BURNER ALERT";

  message.sender.email = sender_email;

  message.subject = "GAS BURNER ALERT";

  message.addRecipient(Recipient_name, Recipient_email);


  //Send simple text message

  String textMsg = msg; // "Door has been opened. Thank You."

  message.text.content = textMsg.c_str();

  message.text.charSet = "us-ascii";

  message.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit;

  if (!smtp.connect(&session))

    return;

  if (!MailClient.sendMail(&smtp, &message))

  {
    Serial.println("Error sending Email, " + smtp.errorReason());
    lcd.clear();
    delay(100);
    lcd.setCursor(0, 1);
    lcd.print("ERROR SENDING EMAIL");

  }


}
