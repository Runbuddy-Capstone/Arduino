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


void setup()
{
   Serial.begin(9600);// initialize the serial communications:
   bt05.begin(9600);
}


void loop(){

    /*if(micros()<100000000){
      xTotal += abs(analogRead(xpin));
      yTotal += abs(analogRead(ypin));
    }
    if(micros()>100000000 && pastTestTime == false){
         xRefernce = xTotal * 0.000000001;
         yRefernce = yTotal * 0.000000001;
          pastTestTime = true;
    }*/
    if(micros() < 1000)
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
       Serial.println("HIGH!!!!!!!!!!!!!!!!!!");
    }
    if(xValue < xLCompare){
       xLow = true; 
       Serial.println("LOW!!!!!!!!!!!!!!!!!!");
    }
    if(xLow == true && xHigh == true){
      if(micros() - microsTimer >= stepPeriod){
        microsTimer = micros(); 
        xLow = false;
        xHigh = false;
        currentStep = 1;
        if(lastStep != currentStep){
          twoStep += currentStep; 
          if(twoStep == 2){
            stepCount++;
             twoStep = 0;
          }
        }
      }
      else{
        currentStep = 0;
        //xLow = false;
        //xHigh = false;
      
      }
    }

    /*if(stepCount == 2){
      stepCount++;
      twoStep = 0;
    }*/
    lastStep == currentStep;
    
    String stepcounter = "";
    stepcounter.concat(stepCount);
    Serial.println("Step counter " + stepcounter);
    Serial.println("");
    bt05.println("{\"dist\":" + stepcounter + "}");

    
}
