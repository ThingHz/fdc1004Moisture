#ifndef SENSORS_h
#define SENSORS_h

#include "hardwareDefs.h"
#include "HX711.h"
#include <Wire.h>
#include <Protocentral_FDC1004.h>
//#include "Adafruit_SHT31.h"

int capdac = 0;
char result[100];

FDC1004 FDC;

HX711 scale;
/*
class eeprom{
  public:
    void ROMUpdateDeviceid(String device) {
      device += ";";
      write_EEPROM(device, ROM_ADDRESS_UPDATE_DEVICEID);
      EEPROM.commit();
    }
}
*/
class Sensors {
  public:

    bool readThermistor(RunTimeState &runTime) {
      double rTherm = 0 ;           // Holds thermistor resistance value
      double tKelvin = 0 ;           // Holds calculated temperature
      double tCelsius = 0; // Hold temperature in celsius

      //Now find the resistance of thermistor by applying voltage devider rule
      rTherm = BALANCE_RESISTOR * ((1023 / thermisterCalculation()) - 1);
      tKelvin = (BETA * ROOM_TEMP) / (BETA + (ROOM_TEMP * log(rTherm / RESISTOR_ROOM_TEMP)));
      tCelsius = tKelvin - 273.15;
      RSTATE.thermistorVal = tCelsius;
      return true;
    }

    void initI2C() {
      Wire.begin();
    }
   
    bool initLoadCell(long initTime) {
      unsigned long timeLoad = 0;
      bool isScaleReady = false;
      timeLoad = millis();
      scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
      while (millis() - timeLoad <= initTime) {
        if (scale.is_ready()){
            isScaleReady = true; 
          }
          break;
      }
      return isScaleReady;
    }

    void tareLoadCell(){
        scale.tare();
      }

    void callibrateScale(float callibrateValue){
        DEBUG_PRINTF("CALLIBRATED VALUE: %.1f",scale.get_scale());
        scale.set_scale(callibrateValue);
      }

    void readScaleValue(uint8_t readingTimes){
        float getReadings = 0.0f;
        getReadings = scale.get_units(readingTimes);
        RSTATE.readScale = getReadings;
      }
    void calibrate_scale_weight(){ //230821
        scale.tare();
        //Serial.println("\nPut 17 gr in the scale, press 'a' to continue");
        //while(!Serial.available());
        //scale.calibrate_scale(17, 5);        
        scale.set_scale(181.52940); //413.14154//173.11765
        //Serial.println("scale value : ");
        //Serial.println(scale.get_scale(),5);               
    }
    float read_scale_weight(){ //230821
      Serial.print("Weight:");
      RSTATE.readScale = scale.get_units(10);
      
      if(RSTATE.readScale<0) RSTATE.readScale=0;
      
      Serial.print(RSTATE.readScale);
      Serial.print(" gms");          
      Serial.println(); 
      if(RSTATE.readScale<0)RSTATE.readScale=0;//090921
      return RSTATE.readScale; 
    }
  private:
    double thermisterCalculation() {
      double adcAverage  = 0;            // Holds the average voltage measurement
      int adcSamples[SAMPLE_NUMBER_THERMISTER];
      for (int i = 0; i < SAMPLE_NUMBER_THERMISTER; i++) {
        adcSamples[i] = analogRead(THERMISTOR_PIN);  // read from pin and store
        delay(100);        // wait 10 milliseconds
      }

      for (int i = 0; i < SAMPLE_NUMBER_THERMISTER; i++) {
        adcAverage += adcSamples[i];      // add all samples up . . .
      }

      adcAverage /= SAMPLE_NUMBER_THERMISTER;
      return adcAverage;

    }

};
Sensors sensors;

bool ZeroCallibrateSensors(){ //pending fx //310821                    
    //==================== fdc callibration============                  
       FDC.configureMeasurementSingle(MEASURMENT, CHANNEL, capdac);
        FDC.triggerSingleMeasurement(MEASURMENT, FDC1004_100HZ);
        //wait for completion
        delay(20);
        uint16_t value[2];
        static int scampleCap = 0;
        int16_t msb=0;
        int32_t capacitance=0;
        float averageCap[SAMPLE_NUMBER_CAPACITANCE];
        if (! FDC.readMeasurement(MEASURMENT, value)) //succeed
        {
          msb = (int16_t) value[0];      
          capacitance = ((int32_t)457) * ((int32_t)msb); //in attofarads
          capacitance /= 1000;   //in femtofarads
          capacitance += ((int32_t)3028) * ((int32_t)capdac);              
          RSTATE.capacitanceVal = (float)capacitance / 1000;             
        }
        Serial.print("CAPACITANCE : ");
        Serial.println(RSTATE.capacitanceVal);
        if (RSTATE.capacitanceVal > 8) {
          return false;                      
        }  
    //===============init load cell=====================
        if(sensors.initLoadCell(10000)) {    
          sensors.calibrate_scale_weight();  
        } 
       return true;
  }
 bool DisplayMainResult(int cursorPosition){    
     char tolerence=5;        
     char dot_pos=1;     
     display.clearDisplay();
          
     uint16_t value[2];
     static int scampleCap = 0;
     float averageCap[SAMPLE_NUMBER_CAPACITANCE]; 
     memset(averageCap, 0 , sizeof(averageCap));
     scampleCap = 0;      
     capdac = 0; //160921
     
     //FDC.configureMeasurementSingle(MEASURMENT, CHANNEL, capdac); //160921
     //FDC.triggerSingleMeasurement(MEASURMENT, FDC1004_100HZ);
    
     //wait for completion          
     //delay(20);
          
     for(char moisture_loop = 1; moisture_loop <= SAMPLE_NUMBER_CAPACITANCE; moisture_loop++){
                            
       if(dot_pos>10)dot_pos=0; 
       please_wait_msg(dot_pos*10); //070921
       dot_pos++;
       
          FDC.configureMeasurementSingle(MEASURMENT, CHANNEL, capdac);
          FDC.triggerSingleMeasurement(MEASURMENT, FDC1004_100HZ);
          
          //wait for completion          
          delay(20);
          
          if (! FDC.readMeasurement(MEASURMENT, value))
          {
            int16_t msb = (int16_t) value[0];
            
            int32_t capacitance = ((int32_t)457) * ((int32_t)msb); //in attofarads
            capacitance /= 1000;   //in femtofarads
            capacitance += ((int32_t)3028) * ((int32_t)capdac);
            
            RSTATE.capacitanceVal = (float)capacitance / 1000;
            Serial.print("capacitance : ");
            Serial.println(RSTATE.capacitanceVal);
            
            if (msb > UPPER_BOUND)               // adjust capdac accordingly
            {
              if (capdac < FDC1004_CAPDAC_MAX)
              capdac++;
            }
            else if (msb < LOWER_BOUND)
            {
              if (capdac > 0)
              capdac--;
            } 
            if (RSTATE.capacitanceVal > 0.00) {
              averageCap[scampleCap] = RSTATE.capacitanceVal;
              scampleCap++;
            }
            
            if (scampleCap == SAMPLE_NUMBER_CAPACITANCE) {
              for (int j = 0; j < SAMPLE_NUMBER_CAPACITANCE; j++) {
                RSTATE.averageCapValue += averageCap[j];
              }
              RSTATE.averageCapValue /= SAMPLE_NUMBER_CAPACITANCE;
              RSTATE.calculateMoisture(RSTATE.averageCapValue,cursorPosition);
              if(RSTATE.moistureVal < 0) RSTATE.moistureVal = 1.02; //160921                                            
              memset(averageCap, 0 , sizeof(averageCap));
              scampleCap = 0;              
            }
          }
     }      
       return true;
 }
/*
 * init fdc is used to handle 1st time iuncorrect value problem 
 */
bool initFdc(){ //160921
     char tolerence=5;        
     char dot_pos=1;     
     display.clearDisplay();         
     uint16_t value[2];
     static int scampleCap = 0;
     float averageCap[SAMPLE_NUMBER_CAPACITANCE]; 
     memset(averageCap, 0 , sizeof(averageCap));
     scampleCap = 0;      
     capdac = 0; //160921 
         
     for(char moisture_loop = 1; moisture_loop <= SAMPLE_NUMBER_CAPACITANCE; moisture_loop++){
                                         
          FDC.configureMeasurementSingle(MEASURMENT, CHANNEL, capdac);
          FDC.triggerSingleMeasurement(MEASURMENT, FDC1004_100HZ);
                  
          delay(20);
          
          if (! FDC.readMeasurement(MEASURMENT, value))
          {
            int16_t msb = (int16_t) value[0];
            
            int32_t capacitance = ((int32_t)457) * ((int32_t)msb); //in attofarads
            capacitance /= 1000;   //in femtofarads
            capacitance += ((int32_t)3028) * ((int32_t)capdac);
            
            RSTATE.capacitanceVal = (float)capacitance / 1000;
                        
            if (msb > UPPER_BOUND)               // adjust capdac accordingly
            {
              if (capdac < FDC1004_CAPDAC_MAX)
              capdac++;
            }
            else if (msb < LOWER_BOUND)
            {
              if (capdac > 0)
              capdac--;
            } 
            if (RSTATE.capacitanceVal > 0.00) {
              averageCap[scampleCap] = RSTATE.capacitanceVal;
              scampleCap++;
            }
            
            if (scampleCap == SAMPLE_NUMBER_CAPACITANCE) {
              for (int j = 0; j < SAMPLE_NUMBER_CAPACITANCE; j++) {
                RSTATE.averageCapValue += averageCap[j];
              }
              RSTATE.averageCapValue /= SAMPLE_NUMBER_CAPACITANCE;
              RSTATE.calculateMoisture(RSTATE.averageCapValue,0);
                                                                                   
              memset(averageCap, 0 , sizeof(averageCap));
              scampleCap = 0;              
            }
          }
     }      
       return true;
 }
bool checkGrainWeight(int max_weight){
     char tolerence = 5;
     String msg="";
     sensors.read_scale_weight();
     if( RSTATE.readScale < (max_weight - tolerence) ){
      int less_weight = max_weight - RSTATE.readScale;       
       msg = "ADD " + String(less_weight) + "g"; //String(extra_weight, DEC); 
       spcChar(msg,15); 
      return false;
     }
     else if( RSTATE.readScale > (max_weight + tolerence) ){
       int extra_weight = RSTATE.readScale - max_weight;
       msg = "REMOVE " + String(extra_weight) + "g"; //String(extra_weight, DEC); 
       spcChar(msg,2); 
      return false;
     }
      return true;
}  

#endif SENSORS_h
