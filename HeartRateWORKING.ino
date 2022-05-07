//#include <LiquidCrystal.h>
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#include <SoftwareSerial.h>
 SoftwareSerial bt05(11,10);
 int xpin = A2;                
 int ypin = A3;                
 //int zpin = A1; 
 int xValue;
 int yValue;

int microsTimer = 0;
int stepPeriod = 20000;

bool xHigh;
bool xLow;

bool yHigh;
bool yLow;

bool pastTestTime = false;
int lastStep = 0;
int currentStep = 0;
int stepCount = 0;

int xHCompare, xLCompare;

int swingRange = 20;

float xRefernce;
float yRefernce;

int xTotal, yTotal;

int twoStep =0;
void stepCounter();
int numSteps;
//LiquidCrystal lcd(13, 12, 11, 10, 9, 8);
 
#define REPORTING_PERIOD_MS     1000

PulseOximeter pox;
uint32_t tsLastReport = 0;
 
void onBeatDetected()
{
    Serial.println("Beat!");
}
 
void setup()
{
    Serial.begin(9600);
    bt05.begin(9600);
    Serial.print("Initializing pulse oximeter..");

    if (!pox.begin()) {
        Serial.println("FAILED");
        for(;;);
    } else {
        Serial.println("SUCCESS");
    }
     pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
 
    // Register a callback for the beat detection
    pox.setOnBeatDetectedCallback(onBeatDetected);
}
 
void loop()
{
    // Make sure to call update as fast as possible
    pox.update();
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
        Serial.print("Heart rate:");
        Serial.print(pox.getHeartRate());
        Serial.print("bpm / SpO2:");
        Serial.print(pox.getSpO2());
        Serial.println("%");
        stepCounter();
        String HRstring = "";
        HRstring.concat(pox.getHeartRate());
        
        int totalSteps;
        totalSteps += numSteps;
        Serial.println("");
        Serial.println("Number of Steps" +numSteps);
        
        String totalStepsString = "";
        totalStepsString.concat(totalStepsString);
        bt05.println("{\"dist\":" + totalStepsString + "}");
        //bt05.println("{\"HR\":" + HRstring + "}");
        //bt05.print("{\"HR\":");
        //bt05.print(HRstring);
        //bt05.print(",\"dist\":");
        //bt05.print(numSteps);
        //bt05.println("}");
        //bt05.print(pox.getHeartRate());
        //bt05.print("'");
 
 
 
        tsLastReport = millis();
    }
}

void stepCounter(){

    if(micros() < 100)
    xRefernce = analogRead(xpin);
    
    xHCompare = xRefernce + swingRange;
    xLCompare = xRefernce - swingRange;
    
    xValue = analogRead(xpin);
    yValue = analogRead(ypin);

    Serial.print("X = ");
    Serial.println(xValue);
    Serial.print("xRefernce ");
    Serial.println(xRefernce);
    Serial.print("Y = ");
    Serial.println(yValue);
    
    

    if(xValue > xHCompare){
       xHigh = true; 
       //Serial.println("HIGH!!!!!!!!!!!!!!!!!!");
    }
    if(xValue < xLCompare){
       xLow = true; 
       //Serial.println("LOW!!!!!!!!!!!!!!!!!!");
    }
    if(xLow == true && xHigh == true){
      if(micros() - microsTimer >= stepPeriod){
        microsTimer = micros(); 
        xLow = false;
        xHigh = false;
        currentStep = 1;
        //if(lastStep != currentStep){
          twoStep += currentStep; 
          if(twoStep == 2){
            stepCount = 1;
             twoStep = 0;
             numSteps = 1;             //numSteps.concat(stepCount);
             stepCount = 1;
          //}
        }
      }
    }
    
      else{
        currentStep = 0;
        numSteps = 0;
        //xLow = false;
        //xHigh = false;
      
      }
    lastStep == currentStep;
    
    //String stepcount = "";
    //stepcounter.concat(stepCount);
    //Serial.println("Step counter " + stepcounter);
    //Serial.println("");

 }
