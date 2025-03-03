//Definitions & Initilizations: LCD Screen

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 3);

//Definitions & Initializations: Adafruit_ADS1X15

#include <Adafruit_ADS1X15.h>
Adafruit_ADS1115 ads;
const int redoxChannel = 0; //The redox input on the ADC

//Definitions & Initilizations: RTC
#include <Wire.h>
#include "RTClib.h"
RTC_PCF8523 rtc;

//Definitions & Initilizations: SD Card
#include <SPI.h>
#include <SD.h>
const int chipSelect = 10;

//Variables for storing measurements
int16_t input; //Direct reading from the ADC
float redoxmV_raw1; //Voltage (mV) calculated from the ADC reading
float redoxmV_raw2; //Voltage (mV) calculated from the ADC reading
float redoxmV_raw3; //Voltage (mV) calculated from the ADC reading

float redoxmV_adjustedPT1; //Voltage (mV) with the amplifier offsetPT1 subtracted
float redoxmV_adjustedPT2; //Voltage (mV) with the amplifier offsetPT2 subtracted
float redoxmV_adjustedPT3; //Voltage (mV) with the amplifier offsetPT3 subtracted

float offset; //Amplifier offset (mV), recorded when the inputs are shorted wrt PT1

//Relay 1 connects the Pt probe to the amplifier when activated (measurement)
//Relay 2 connects the inputs to the amplifier when activated (calibration)
const int relay1 = 30; //Relay 1 is controlled by pin 30
const int relay2 = 32; //Relay 2 is controlled by pin 32
const int relay3 = 34; //Relay 3 is controlled by pin 34
const int relay4 = 36; //Relay 4 is controlled by pin 36

//Timing for measurements
const int redoxInterval = 30000; //miliseconds -- time between measurement
unsigned long pM = millis(); //counter for previous measurement
unsigned long cM; //counter for current measurement

//number of data points since turning on
int data_count = 0;


void setup() {

  Serial.begin(9600);
  lcd.init(); //initialize the lcd
  lcd.backlight(); //open the backlight

  pinMode(relay1, OUTPUT); //set relay pins to output
  pinMode(relay2, OUTPUT);

  //*** Using the UPGRADED SSR RELAYS ***//
  digitalWrite(relay1, LOW); // [changed the polarity for SSR (changed by TH)]
  digitalWrite(relay2, LOW); // [changed the polarity for SSR (changed by TH)]

  rtc_setup();
  sdcard_setup();
  ads_setup();
  delay(3000);
  lcd.clear();
  //Run zeroing routine to calibrate readings according to the offsets
  offset = zero()*1000; //The zeroPT1() function returns the offsetPT1 in volts, converted to mV
  //Display offsetPT1 on LCD
  lcd.setCursor(0,0);
  lcd.print("Offset: ");
  lcd.print(offset, 1);
  lcd.print(" mV");
  //delay(2000);//for everything or one at the end?
}


void loop() {
  // Main loop where the measurement routine is repeated
  //Each loop, check time to see if it's been 5 seconds since last measurement
  cM = millis();
  if (cM - pM > redoxInterval)
  {
    lcd.clear();

    //Take measurement, convert to mV, and subtract offsets

    redoxmV_raw1 = take_measurementPT1()*1000;
    redoxmV_adjustedPT1 = redoxmV_raw1 - offset;//Increment # of data points taken

    redoxmV_raw2 = take_measurementPT2()*1000;
    redoxmV_adjustedPT2 = redoxmV_raw2 - offset;//Increment # of data points taken

    //redoxmV_raw3 = take_measurementPT3()*1000;
    //redoxmV_adjustedPT3 = offset-redoxmV_raw3;//Increment # of data points taken
    
    data_count++;
    //Serial.println(redoxmV_adjusted);
    //Save to SD card, see sd_task() below
    sd_task();
    //Serial print for use with laptop
    Serial.print("Reference Potential  = ");
    Serial.print(offset);
    Serial.println(";");  
    Serial.print("Redox raw = ");
    Serial.print(redoxmV_raw1,4);// raw!!
    Serial.print(","); 
    Serial.print(redoxmV_raw2,4);// raw!!
    Serial.println(";");     
    Serial.print("Redox Potential = ");
    Serial.print(redoxmV_adjustedPT1, 4);//TBT
    Serial.print(",");
    Serial.println(redoxmV_adjustedPT2, 4);//TBT
    //Serial.println(redoxmV_adjustedPT3, 4);//TBT

    // Get the current time from the RTC module
    DateTime now1 = rtc.now();

    //Display reading on LCD
    lcd.setCursor(0, 0);
    lcd.print("D");
    lcd.print(now1.timestamp(DateTime::TIMESTAMP_FULL));
    lcd.setCursor(0, 1);
    lcd.print(data_count);
    lcd.setCursor(3, 2);
    lcd.print("Redox Potential");
    lcd.setCursor(0, 3);
    // lcd.print(redoxmV_adjustedPT1, 1);//TBT
    // lcd.print(" mV");
    // lcd.print(redoxmV_adjustedPT2, 1);//TBT
    // lcd.print(" mV");
    // lcd.print(redoxmV_adjustedPT3, 1);//TBT
    // lcd.print(" mV");      
    lcd.print(redoxmV_raw1, 1);//TBT.
    lcd.print(",");
    lcd.print(redoxmV_raw2, 1);//TBT
    // lcd.print(",");
    // lcd.print(redoxmV_adjustedPT3, 1);//TBT
    lcd.print(" mV");       
    pM = cM; //update counters
  }
}
// Calibration function: Short inputs, help user adjust knob until it reads 0.5V
// ***** INCLUDES DELAYS ***** //
//TODO: should be controlled by a push button if the code resets every 30 min??
//zeroPT1();
//zeroPT2();
//zeroPT3();
//take_measurement();
//sd_task();