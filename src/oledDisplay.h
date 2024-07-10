#ifndef OLED_DISPLAY_H_
#define OLED_DISPLAY_H_


#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_I2CDevice.h>
#include <Fonts/FreeSerif9pt7b.h>
#include <Fonts/FreeSerif12pt7b.h>
#include <Fonts/FreeSerifBold12pt7b.h>
#include <Fonts/FreeMonoBoldOblique9pt7b.h>
#include <Fonts/FreeMonoBoldOblique12pt7b.h>
//#include <Fonts/TomThumb.h>  
#include <Fonts/Picopixel.h>
//#include <Fonts/Org_01.h>
//#include <Fonts/FreeSerifItalic9pt7b.h>
//#include <Fonts/Tiny3x3a2pt7b.h>
//#include <Fonts/FreeSerifBoldItalic9pt7b.h>
//#include <Fonts/FreeSerifBold9pt7b.h>
//#include <Fonts/FreeSansOblique9pt7b.h>
//#include <Fonts/FreeSansBoldOblique9pt7b.h>
//#include <Fonts/FreeSansBold9pt7b.h>
//#include   <Fonts/arial14.h>
//#include   "arial14.h"

#include "hardwareDefs.h"
#include "espUtils.h"
#include "def.h"
enum fontSize{small=1,medium,large}; //020921

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void warning_Display(String msg); //310821

bool initDisplay()
{
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    DEBUG_PRINTLN("SSD1306 allocation failed");
    return false;
  }
  return true;
}

bool isDisplayAvailable()
{
  return true;
}

void clearDisplay()
{
  delay(100); //TODO:: don't know why this is needed.
  display.clearDisplay();
  display.setTextColor(WHITE);
}

void drawThermometer(float Value)
{
  display.fillCircle(8, 55, 7, WHITE);  // Draw filled circle (x,y,radius,color). X and Y are the coordinates for the center point

  // Draw rounded rectangle:
  display.drawRoundRect(6, 3, 5, 49, 2, WHITE);  // Draw rounded rectangle (x,y,width,height,radius,color)

  Value = Value * 0.43; //ratio for show
  display.drawLine(8, 46, 8, 46 - Value, WHITE); // Draw line (x0,y0,x1,y1,color)
  display.setFont();
  display.setCursor(110, 20);
//  display.setTextSize(1);
//  display.cp437(true);
//  display.write(167);
//  display.setTextSize(2);
//  display.print("C");
}

void empty_jar_msg(){ //010921
  // draw_note_custom("EMPTY JAR & PRESS OK",40,45);

}

void drawLoGo(String annotation) {
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setFont(&FreeMonoBoldOblique12pt7b);
  display.setCursor(5, 30);
  display.print(annotation);
}

void drawAnotation(String annotation, int16_t cursorValue)
{
  display.setFont();
  display.setTextSize(1);
  display.setCursor(cursorValue, 0);
  display.print(annotation);
}
          
void drawValue(String value)
{
  display.setFont(&FreeSerif12pt7b);
  display.setTextSize(1.9);
  display.setTextColor(WHITE);
  display.setCursor(20, 50);
  display.print(value);
}

void draw_outline(){ //020921
  display.drawRect(0, 0, display.width()-2*0, display.height()-2*0, SSD1306_WHITE);
}
 
void draw_value_custom(String msg, char x_pos, char y_pos) //230821
{
  display.setFont(&FreeSerif12pt7b);
  display.setTextSize(1.9);
  display.setTextColor(WHITE);
  display.setCursor(x_pos, y_pos);
  display.print(msg);
}

void draw_note_custom(String msg, char x_pos, char y_pos,char fontsize){ //230821
  display.setFont();
  display.setTextSize(fontsize);
  display.setCursor(x_pos,y_pos);
  display.print(msg);
}
      
void drawMoistureAndPf(String pfValue, String moistValue){
    display.setFont(&FreeSerif9pt7b);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(40, 50);
    display.print(moistValue);
    display.setFont(&FreeSerif9pt7b);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(40,30);
    display.print(pfValue);
}

void drawDisplay(DisplayMode mode) 
{
  const char MESSAGE_BODY[] = "%d grams";
  display.clearDisplay();

  char valMoist[20] = {0};
  char valMoistPf[20] = {0};
  char valWeight[20] = {0};
  draw_outline();
  switch (mode)
  {
    case DisplayCenterTextLogo:
      
      
    for(char blink_cnt=0; blink_cnt<6; blink_cnt++){
      display.clearDisplay();
      draw_outline();
      display.setTextSize(0);
      draw_note_custom("QUALITY",40,40,small);
      draw_note_custom("FOOD FOR BILLION",15,50,small);
      display.display();
      delay(10);
      display.setTextColor(WHITE);
      display.setFont(&FreeMonoBoldOblique12pt7b);
      display.setCursor(20, 30);
      display.print("ThingHz");      
      display.display();    
      delay(10);    
    } 
          
      break;
    case DisplayMoisture:
      {
        display.setTextSize(1);         
        display.drawLine(0, 21, 128, 21, SSD1306_WHITE);  //drawLine(startX, startY, endX, endY, color)
        draw_note_custom("MOISTURE :",5,5,medium); 
        auto moisture = RSTATE.moistureVal;        
        snprintf(valMoist, 20, "%.2f %%", moisture);
        drawValue(valMoist);
      }
      break;
     case DisplayLoadCellInrange:
      {
       // auto weight = RSTATE.readScale;      
       // snprintf(valWeight, 20, MESSAGE_BODY, weight);
       // draw_note_custom("GRAIN WEIGHT",30,0);        
       // draw_value_custom(valWeight,15,40);
       // draw_note_custom("Weight in range",5,55);
      }
      break;
      case DisplayLoadCellOutrange: 
      {                        
        auto weight = RSTATE.readScale;        
        snprintf(valWeight, 20, MESSAGE_BODY, weight);
        draw_note_custom("GRAIN WEIGHT",30,0,small);        
        draw_value_custom(valWeight,15,40);
        draw_note_custom("Weight not in range",5,55,small);
       }
      break;      
      case  DisplayCapcitanceRequired:
      {
        auto moisture = RSTATE.moistureVal;
        auto moisturepF = RSTATE.averageCapValue;
        //auto moisture = RSTATE.capacitanceVal;
        drawAnotation("Moisture",20);
        snprintf(valMoist, 20, "%.3f %%", moisture);
        snprintf(valMoistPf, 20, "%.3f pF", moisturepF);
        drawMoistureAndPf(valMoistPf,valMoist);
        }
       break;
       case DisplayMenu:
       { char poss=15;
         draw_note_custom("SELECT COMODITY....",0,0,small);
         draw_note_custom("  1. WHEAT",0,poss+0,small);
         draw_note_custom("  2. MAZE", 0,poss+10,small);
         draw_note_custom("  3. RICE", 0,poss+20,small);
         draw_note_custom("  4. MOONG",0,poss+30,small);
         draw_note_custom("  4. PADDY",0,poss+40,small);
       }
       break;
       case DisplayInfo:
       {    
          display.drawLine(0, 17, 128, 17, SSD1306_WHITE);  //drawLine(startX, startY, endX, endY, color)        
          display.setTextSize(0);      
          display.setTextColor(WHITE);     
          display.setFont(&Nimbus_Roman_No9_L_Regular_15); 
          display.setCursor(15, 15);
          display.print("    INFO");
          display.setCursor(6, 35);
          display.print("FW VER: 1.01");        
       }
       break;
       case  DisplayCapcitance:
      {   
        display.setTextSize(1);         
        display.drawLine(0, 21, 128, 21, SSD1306_WHITE);  //drawLine(startX, startY, endX, endY, color)
        draw_note_custom("CAP :",5,5,medium); 
        auto cap = RSTATE.averageCapValue;        
        snprintf(valMoist, 20, "%.2f %%", cap);
        drawValue(valMoist);                    
        }
       break;
      /* case ZeroCallibration:
       { char pos=15;
       display.setTextSize(1); 
       display.drawLine(0, 17, 128, 17, SSD1306_WHITE);  //drawLine(startX, startY, endX, endY, color)
         draw_note_custom("MOISTURE METER",22,5,small);    
         draw_note_custom("FIRMWARE VER:    0.1",3,pos+10,small);         
         draw_note_custom("UPDATED ON: 03/09/21",3,pos+20,small);         
       }
       break;
      */
    default:
      display.setTextSize(1);
      display.setCursor(20, 0);
      display.print("NO Mode");
      //DEBUG_PRINTLN("NO Mode selected");
      break;
  }
  yield();
  delay(100);
  display.display();
}

void Display_pressOk(String msg) //310821
{
  display.clearDisplay();
  draw_outline();  
  display.setTextSize(1);  
  //display.setFont(&FreeMonoBoldOblique12pt7b); 
  display.drawLine(0, 21, 128, 21, SSD1306_WHITE);  //drawLine(startX, startY, endX, endY, color)
  draw_note_custom(msg,5,5,medium);    
  //draw_note_custom(" PRESS OK  ",5,47,medium);                     
  yield();
  delay(100);
  display.display(); 
}

void DisplayCurssor(char y_pos){//010921
  int delayy = 10;  
    for(int test1=1; test1<10; test1++){
      //if(genflag.keypressed==1) return; 
      draw_note_custom(">",test1,y_pos,2);
      //if(genflag.keypressed==1) return;
      display.display();
      //if(genflag.keypressed==1) return;
      delay(delayy);
      //if(genflag.keypressed==1) return;
    }    
}

/* for running curssor 
void DisplayRunningCurssor(char y_pos){//070921
  int delayy = 100;
  display.clearDisplay();
  draw_outline();   
    for(int test1=1; test1<8; test1++){
      //if(genflag.keypressed==1) break; 
      draw_note_custom(">",test1,y_pos,2);
      display.display();
      delay(delayy);
    }
     display.clearDisplay();
     draw_outline(); 
     display.display();
     
    delay(500);    
}
*/
void pleaseWaitMsg(){
  String msg = "PROCESSING"; 
  char dots=0,dot_pos[50]; 
  
  for(char main_loop=60; main_loop>0; main_loop--){
   
    if(genflag.startWaitMsg == 0 )break;  
    
    dot_pos[dots++]=main_loop;     
    
    for(char loop1 = 0; loop1 < main_loop; loop1++){ 
    
    if(genflag.startWaitMsg == 0 )break; 
    
      display.clearDisplay();    
      draw_outline();
      for(char dot=0; dot<=dots; dot++){ //print last dots
         draw_note_custom(".",dot_pos[dot],25,large);     
      }
      display.setFont(&FreeMonoBoldOblique12pt7b);
      draw_note_custom(msg,5,5,medium);
      draw_note_custom(".",loop1,25,large);
      display.display();
      loop1+=7;          
    }    
    if(main_loop>7)main_loop-=7;
  }
  //===================================================
      
  display.clearDisplay();
  char going_dot=0;
  for(char main_loop=60; main_loop>0; main_loop--){
    
    if(genflag.startWaitMsg == 0 )break;  
         
    for(char loop1 = main_loop; loop1 < 120; loop1++){ 
    
    if(genflag.startWaitMsg == 0 )break; 
    
      display.clearDisplay();    
      draw_outline();
       for(char dot=going_dot; dot<dots; dot++){ //print last dots
         draw_note_custom(".",dot_pos[dot],25,large);     
      }
       display.setFont(&FreeMonoBoldOblique12pt7b);    
      draw_note_custom(msg,5,5,medium);
      draw_note_custom(".",loop1,25,large);
      display.display();
      loop1+=7;                 
    }
    going_dot++;
     if(main_loop>7)main_loop-=7;        
  }
   display.clearDisplay();
   display.display();
}

void please_wait_msg(char pos){
  if(!pos){
    display.clearDisplay();
    draw_outline();  
    draw_note_custom("PROCESSING",5,20,medium);
    display.display();
    return;
  }
  draw_outline();  
  draw_note_custom("PROCESSING",5,20,medium);
  draw_note_custom(".",pos,35,large);
  display.display();           
}

void test_fonts(){ //080921
 for(int loop1=0; loop1<7; loop1++){
      display.clearDisplay();
      draw_outline(); 
      switch(loop1){
        /*case 0: display.setFont(&TomThumb);  break;              
        case 1: display.setFont(&Picopixel); break;                                                              
        case 2: display.setFont(&FreeSerifBoldItalic9pt7b); break;
        case 3: display.setFont(&FreeSerifBold9pt7b); break;
        case 4: display.setFont(&FreeSansOblique9pt7b); break;
        case 5: display.setFont(&FreeSansBoldOblique9pt7b); break;
        case 6: display.setFont(&FreeSansBold9pt7b); break;*/
      }            
      display.setCursor(5,15);
      display.print("PLS EMPTY JAR");                                     
      display.display();
      delay(2000);
    }
}
void spcChar(String msg,char x_pos){
      display.clearDisplay();
      draw_outline();
      display.drawLine(0, 21, 128, 21, SSD1306_WHITE);  //drawLine(startX, startY, endX, endY, color) 
      display.setTextSize(0);      
      display.setTextColor(WHITE);
      //display.setFont(&FreeMonoBoldOblique9pt7b); 
      display.setFont(&Nimbus_Roman_No9_L_Regular_15); 
      display.setCursor(x_pos, 15);
      display.print(msg);
      display.setCursor(6, 35);
      display.print("PRESS OK TO"); 
      display.setCursor(15, 55);
      display.print("CONTINUE");                                                 
      display.display();            
}

#endif
