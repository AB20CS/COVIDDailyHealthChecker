
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <SD.h>
#include "DHT.h"

#define DHTPIN 7 
#define DHTTYPE DHT11   // DHT 11

DHT dht(DHTPIN, DHTTYPE);

LiquidCrystal_I2C lcd(0x3F,16,2);

const int chipSelect = 4;
File sdcard_file;

const int TouchPin=6; //touch switch attach to pin1
const int ledPin = 13; //pin13 built-in led
int TouchState=0; //store the value of touch switch
int PreviousTouchState=-1; //stores the previous touch state value
int ButtonState=0; //store the value of the button switch
boolean recordTempMode = false; //If false use button to cycle temperature. if true, take through temperature taking mode.


void setup(void)
{
  // start serial port
  Serial.begin(9600);

  //Setup for LCD
  lcd.init(); //initialize the lcd
  lcd.backlight(); //turn on the backlight

  //Setup for SD Card
  Serial.print("Initializing SD card...");
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    return;
  }
  Serial.println("Card initialized.");

  sdcard_file = SD.open("data.csv", FILE_WRITE);
  
  // if the file is available, write to it:
  
  if (sdcard_file) {
    sdcard_file.println("Temperature,Humidity");
    sdcard_file.close();
  }
  else 
  {
    Serial.println("error opening data.csv");
  }  
  
  //Setup for Touch Switch
  pinMode(TouchPin,INPUT);//set sensorPin as INPUT
  pinMode(ledPin,OUTPUT); // set ledPin as OUTPUT

  dht.begin();
  
  recordTempMode=false; //Assume that we are not going to record temperature. Someone can cycle through the previous temps recorded on SD card using button.
  PreviousTouchState=digitalRead(TouchPin);//read the value of pin6
  Serial.println("Previous TouchState");
  Serial.println(PreviousTouchState);
  displayIntro();
}

void loop(void)
{ 
  TouchState=digitalRead(TouchPin);//read the value of pin6
  if (TouchState != PreviousTouchState)
  {
    PreviousTouchState = TouchState;
    lcd.clear();
    //If recordTempMode is true then we are in the recording process and this cannot be interrupted. This why else is DO Nothing
    //If recordTempMode is false then make true to start the Record Temp process
    if (recordTempMode == false) 
      recordTempMode = true;
  }

  if(recordTempMode==true)
  {
    recordTemp();
  } 
  else
  {
    displayIntro();
  }
}

void recordTemp()
{
  //Turn on LED to show that Temperature will now be recorded
  digitalWrite(ledPin,HIGH); //turn on the led
  
  Serial.println("Requesting temperature and humidity...");
  float h = dht.readHumidity(); //read humidity before user puts hand on sensor (takes humidity of environment)

  lcd.setCursor(0,0); // set the cursor to column 15, line 0
  lcd.print("Hold for 20 sec");
  delay(3000);
  lcd.clear();

  for (int c = 19; c > 0; c--)
  {
    lcd.print(c);
    delay(1000);
    lcd.clear();
  }
  
  float t = dht.readTemperature() + 10; //add 10 degrees to calibrate
  
  if (isnan(h) || isnan(t)) 
  {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  
  Serial.print("Temperature in Celsius: ");
  Serial.print(t);
  Serial.println(" C");
  Serial.print("Humidity:");
  Serial.print(h, 2);
  Serial.print("%");

  lcd.clear();

  lcd.setCursor(0,0); // set the cursor to column 0, line 0
  lcd.print("Temp:");
  lcd.print(t, 2);
  lcd.print(char(223));
  lcd.print("C");

  lcd.setCursor(0,1); // set the cursor to column 0, line 1
  lcd.print("Humidity:");
  lcd.print(h, 2);
  lcd.print("%");

  delay(5000);
  lcd.clear();

  writeToSD(t,h);

  if (t < 36)
    {
      lcd.setCursor(0,0);
      lcd.print("Temp is low");
      lcd.setCursor(0,1);
      lcd.print("Call doctor");
      delay(3000);
      lcd.clear();
    }
  else if (36 <= t < 37)
  {
    lcd.setCursor(0,0);
    lcd.print("Temp is fine");
    delay(3000);
    lcd.clear();
  }
  else
  {
    lcd.setCursor(0,0);
    lcd.print("Temp is high");
    lcd.setCursor(0,1);
    lcd.print("Call doctor");
    delay(3000);
    lcd.clear();
  }

  lcd.setCursor(0,0);
  lcd.print("Data recorded");
  delay(3000);
  lcd.clear();

  recordTempMode = false;
  //lcd.setCursor(0,0);
  //lcd.print("Touch to quit");
  displayIntro;
}

void writeToSD(float t, float h)
{
  sdcard_file = SD.open("data.csv", FILE_WRITE);
  
  // if the file is available, write to it:
  
  if (sdcard_file) {
    sdcard_file.println(String(t) + "," + String(h));
    sdcard_file.close();
  }
  else 
  {
    Serial.println("error opening data.csv");
  }
}

void displayIntro()
{
    digitalWrite(ledPin,LOW); //turn off the led
    lcd.setCursor(0,0); // set the cursor to column 0, line 0
    lcd.print("Touch to begin"); // Print a message to the LCD
    delay(3000);
}
