
#include "deviceState.h"
#include "oledDisplay.h"
#include "hardwareDefs.h"
#include "sensors.h"
#include "espUtils.h"
#include <Ticker.h>
#include "espBle.h"
#include "def.h"
#include "keypad.h" //310821
Ticker sensorCheckTimer;

//ESPble espBle;

TaskHandle_t xHandleFDC;
TaskHandle_t xloadCellThrad; //250821
TaskHandle_t xHandleKEYPAD;
//TaskHandle_t xHandleDisplay;

void setup() {
  // put your setup code here, to run once
  Serial.begin(115200);
  /*
  Serial.print("Xtal freq : ");
  Serial.println(getXtalFrequencyMhz());
  Serial.print("Cpu  freq : ");
  Serial.println(getCpuFrequencyMhz());
  delay(2000);
  setCpuFrequencyMhz(160);
  Serial.print("Xtal freq : ");
  Serial.println(getXtalFrequencyMhz());
  Serial.print("Cpu  freq : ");
  Serial.println(getCpuFrequencyMhz());
  delay(2000); 
  */
  sensors.initI2C();
  initDisplay();
  clearDisplay();
  InitKeypadPins(); //310821 
  RSTATE.displayState = DisplayCenterTextLogo;
  drawDisplay(RSTATE.displayState);
  initFdc();//160921
  
#ifdef IS_CAP_REQUIRED
  //RSTATE.displayState = DisplayCapcitanceRequired;
#endif 
activeBle(); 
   
  event_key = OK_KEY;
  state0();  
   
  xTaskCreatePinnedToCore(//250821
    main_thread
    ,  "main_thread"   
    ,  2048  
    ,  NULL
    ,  3  
    ,  &xloadCellThrad
    ,  1);
     
}

void loop() { //230821
  
}

void main_thread(void *pvParameters){ //250821
  (void) pvParameters;

  for (;;) // A Task shall never return or exit.
  {
    event_key = scan_key();
    if(event_key != prev_key){
      genflag.keypressed=1;
      prev_key=event_key;
      genflag.free_state=0;           
      (k_state[new_st])();      
    }
      genflag.keypressed=0;    
    vTaskDelay(1);
  } 
} 
