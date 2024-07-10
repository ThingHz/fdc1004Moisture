#ifndef KEYPAD_h
#define KEYPAD_h

#include "def.h"
#include "sensors.h"

char new_st=0;
char event_key=0;
char prev_key=0;

enum key_states {OK_KEY=1, UP_KEY, DOWN_KEY, BACK_KEY};
enum posOfMenuItems { item_1 = 5, item_2 = 25, item_3 = 45};
enum scanProcessEnum{ grain_selection=0, check_empty, pour, final_result }; 
char cursor_pos=1;
int  max_weight_of_grain=0;
void display_main_menu_items();
void display_grain_items();
void display_items();
void menu_page();
void home_page();
void state0();
void state1();
void state2();
void state3();
void state4();
char scan_key();

void InitKeypadPins(){
  pinMode(C1,INPUT);
  pinMode(C2,INPUT);
  pinMode(C3,INPUT);
  pinMode(C4,INPUT);
  event_key = scan_key();
}

char scan_key(){ 
  char key=0;
  key = (digitalRead(C1)) ? OK_KEY : (digitalRead(C2)) ? UP_KEY : (digitalRead(C3)) ? DOWN_KEY : (digitalRead(C4)) ? BACK_KEY : 0 ; 
  return key;
}

void (*k_state[])() = {state0,state1,state2,state3,state4}; 

void state0(){
  switch(event_key){
    case OK_KEY:
      genflag.in_main_menu=1;
      cursor_pos = 1; 
      new_st = 1;     
      menu_page();                 
    break;
    case UP_KEY:
     genflag.startWaitMsg = 1;
    break;
    case DOWN_KEY:
      
    break;
    case BACK_KEY:
      home_page();
    break;   
  }
}

void state1(){
  enum items {scan=1, info=2, setupp=3, logs=4};
  switch(event_key){
    case OK_KEY:
       switch(cursor_pos){
        case scan: 
          genflag.in_main_menu=0;  
          new_st=2;                
          menu_page();          
          genflag.scan_process = grain_selection;        
        break;
        case info:
          drawDisplay(DisplayInfo);
        break;
        case setupp:
          new_st=3;
          menu_page();
        break;
       }
    break;
    case UP_KEY:
      menu_page();    
    break;
    case DOWN_KEY:
      menu_page();
    break;
    case BACK_KEY:
      home_page();//160921                     
    break;  
  }
}

void state3(){  
  switch(event_key){
    case OK_KEY:
     display.clearDisplay();    
     draw_outline();
     display.setTextSize(0); 
     //RSTATE.isBLEActive = ~(RSTATE.isBLEActive); //this logic dostnworks with(RSTATE.isBLEActive == 1)bcoz all bits bcome high 255     
     RSTATE.isBLEActive = (RSTATE.isBLEActive) ? false : true;                 
     if( RSTATE.isBLEActive == 1 ){
      Serial.println("connected");        
         TurnBleOn();         
     }
     else{
         Serial.println("disconnected");         
         TurnBleOff();
     }
     draw_note_custom( ( ( RSTATE.isBLEActive ) ? "BLE : ON" : "BLE : OFF") ,20,5,medium); 
     
     DisplayCurssor(item_1);     
     display.display();
    break;    
      
    case BACK_KEY:
      home_page();
    break;  
  }  
}

void state2(){  
  enum grains{NONE, WHEAT, MAZE, RICE, MOONG, PADDY};
  String maxWeight = "";
  static bool warning=0;  //used in weight warning 
  switch(event_key){
    case OK_KEY:
        if(genflag.scan_process == grain_selection){  
            warning = 0;         
            spcChar("EMPTY JAR",15);                              
            genflag.scan_process = check_empty;
            break;
          }
        if(genflag.scan_process == check_empty){
            if( ! ZeroCallibrateSensors() ){
             spcChar("NOT EMPTY",15);
             genflag.scan_process=pour;  
            }
            else{
ok_pressed_after_warning:
              switch(cursor_pos){                        
                case WHEAT: maxWeight = "200g"; max_weight_of_grain=200; break;
                case MAZE:  maxWeight = "150g"; max_weight_of_grain=150; break;
                case RICE:  maxWeight = "100g"; max_weight_of_grain=100; break;
                case MOONG: maxWeight = " 80g"; max_weight_of_grain= 80; break;
                case PADDY: maxWeight = " 50g"; max_weight_of_grain= 50; break;
              }
              String msg = "POUR     " + maxWeight;
              spcChar(msg,2);                                                   
              genflag.scan_process = final_result;
              //genflag.startWaitMsg = 1;
              warning=0;              
            }
            break;
          }
          if(genflag.scan_process==pour){
             goto ok_pressed_after_warning;
             break;   
          }
          if(genflag.scan_process==final_result){           
               
            if(! checkGrainWeight(max_weight_of_grain) && (! warning) ) {warning = 1; return;}            
                                                              
            if(! DisplayMainResult(cursor_pos) ){
              
            }
            else{
              genflag.startWaitMsg = 0;               
              drawDisplay(DisplayMoisture); //100921
            }          
          }
                
    break;
    
    case UP_KEY:
      if(genflag.scan_process == grain_selection) menu_page();    
    break;
    
    case DOWN_KEY:
      if(genflag.scan_process == grain_selection) menu_page();
    break;
    
    case BACK_KEY:
      home_page();
    break;  
  }
}
void bleState(std::string data_rx){ //160921
  static char steps=0;
  static bool warning=0;  //used in weight warning
  char valMoist[20] = {0};
  static int cursorPosition = 0 
  
  switch(steps){
    case 0:
      max_weight_of_grain = 0;
           if(!strcmp(data_rx.c_str(),"ag-wheat") ){max_weight_of_grain=200;cursorPosition = 1;} 
           else if(!strcmp(data_rx.c_str(),"ag-maze" ) ){max_weight_of_grain=150;cursorPosition = 2;}  
           else if(!strcmp(data_rx.c_str(),"ag-rice" ) ){max_weight_of_grain=150;cursorPosition = 3;} 
           else if(!strcmp(data_rx.c_str(),"ag-moong") ){max_weight_of_grain=80;cursorPosition = 4;}
           else if(!strcmp(data_rx.c_str(),"ag-paddy") ){max_weight_of_grain=50;cursorPosition = 5;}                             
      if(max_weight_of_grain){ espBle.send_data("ok"); Serial.print("- ok"); steps++; }              
      else{ espBle.send_data("notok"); Serial.print("- notok"); }    
    break;
    
    case 1: 
      if(! strcmp(data_rx.c_str(),"ag-zerocallibrate") ) {
        if( ZeroCallibrateSensors() ) {espBle.send_data("ok"); Serial.print("- ok");} 
        else {espBle.send_data("notok"); Serial.print("- notok");}  
        steps++;
      }          
      else{
        espBle.send_data("notok");
        Serial.print("- notok"); 
       }
    break;
    
    case 2:
      if(! strcmp(data_rx.c_str(),"ag-startscan") ) {
         char tolerence = 5;
         String msg="";
         
         sensors.read_scale_weight();
        
         if( (RSTATE.readScale < (max_weight_of_grain - tolerence) ) && (!warning) ){ //if less weight
          int less_weight = max_weight_of_grain - RSTATE.readScale;       
           msg = "ADD " + String(less_weight) + "g"; 
           espBle.send_data(msg.c_str());
           Serial.print(" -" + msg);
           warning = 1;
           break;
         }
         else if( (RSTATE.readScale > (max_weight_of_grain + tolerence)) && (!warning) ){
          int extra_weight = RSTATE.readScale - max_weight_of_grain;
           msg = "REMOVE " + String(extra_weight) + "g"; 
           espBle.send_data(msg.c_str());
           Serial.print(" -" + msg);   
           warning = 1;
           break;
         }
         else{
           DisplayMainResult(cursorPosition);
           auto moisture = RSTATE.moistureVal;        
           snprintf(valMoist, 20, "%.2f %%", moisture);
           espBle.send_data(valMoist);
           Serial.print(valMoist);
           steps++;
         }
      }
    break;
  }
}
void state4(){ //160921
   switch(event_key){
    case OK_KEY:
      genflag.in_main_menu=1;
      cursor_pos = 1; 
      new_st = 1;     
      menu_page();                 
    break;
    case UP_KEY:
     genflag.startWaitMsg = 1;
    break;
    case DOWN_KEY:
      
    break;
    case BACK_KEY:
      home_page();
    break;   
  }
}


void home_page(){ //020921//160921
  new_st=0;
  cursor_pos=0;
  genflag.in_main_menu=0;
  genflag.scan_process = 0;
  //RSTATE.displayState = DisplayCenterTextLogo;
  //drawDisplay(RSTATE.displayState);
  event_key = OK_KEY;
  state0();
}

void menu_page(){ //020921
   display.clearDisplay();   
   draw_outline();
   display_items();  
   yield();
   delay(100);
   display.display();
}
void display_items(){
  char max_items=0;
  const char *items_main[5] =     { "1.SCAN", "2.INFO", "3.SETUP", "4.LOG(S)"}; //state1
  const char *items_grain[5] =    { "1.WHEAT","2.MAZE", "3.RICE",  "4.MOONG", "5.PADDY"};//state2
  const char *items_settings[5] = { "BLE"};//state3
  
  const char *items[5]; 

  if(new_st==3){
    std::copy_n( items_settings, 1, items);
  }
  else{
    std::copy_n( ((genflag.in_main_menu==1) ? items_main : items_grain ), 5, items);                  
  }     
     if(genflag.free_state==1) goto skipThis; // for running cursor

     max_items = (new_st == 1) ? 4 : (new_st == 2) ? 5 : (new_st == 3) ? 1 : 0;     
     if(event_key == UP_KEY)   if(cursor_pos > 1)         cursor_pos--;
     if(event_key == DOWN_KEY) if(cursor_pos < max_items) cursor_pos++;
         
skipThis:     
    if (new_st==1){             
      switch(cursor_pos){ 
        case 1:  
                  
          draw_note_custom(items[0], 20,item_1,medium); 
          draw_note_custom(items[1], 20,item_2,medium); 
          draw_note_custom(items[2], 20,item_3,medium);
          DisplayCurssor(item_1); 
        break;
        case 2:
          
          draw_note_custom(items[0], 20,item_1,medium); 
          draw_note_custom(items[1], 20,item_2,medium); 
          draw_note_custom(items[2], 20,item_3,medium);
          DisplayCurssor(item_2);   
        break;
        case 3:
           
          draw_note_custom(items[0], 20,item_1,medium); 
          draw_note_custom(items[1], 20,item_2,medium); 
          draw_note_custom(items[2], 20,item_3,medium);
          DisplayCurssor(item_3);   
        break;
        case 4:
               
          draw_note_custom(items[1], 20,item_1,medium); 
          draw_note_custom(items[2], 20,item_2,medium); 
          draw_note_custom(items[3], 20,item_3,medium);
          DisplayCurssor(item_3);  
        break;                 
     }
    }
    else if(new_st == 2){
      switch(cursor_pos){ 
        case 1:  
                  
          draw_note_custom(items[0], 20,item_1,medium); 
          draw_note_custom(items[1], 20,item_2,medium); 
          draw_note_custom(items[2], 20,item_3,medium);
          DisplayCurssor(item_1); 
        break;
        case 2:
          
          draw_note_custom(items[0], 20,item_1,medium); 
          draw_note_custom(items[1], 20,item_2,medium); 
          draw_note_custom(items[2], 20,item_3,medium);
          DisplayCurssor(item_2);   
        break;
        case 3:
           
          draw_note_custom(items[0], 20,item_1,medium); 
          draw_note_custom(items[1], 20,item_2,medium); 
          draw_note_custom(items[2], 20,item_3,medium);
          DisplayCurssor(item_3);   
        break;
        case 4:
               
          draw_note_custom(items[1], 20,item_1,medium); 
          draw_note_custom(items[2], 20,item_2,medium); 
          draw_note_custom(items[3], 20,item_3,medium);
          DisplayCurssor(item_3);  
        break;
        case 5:
                              
          draw_note_custom(items[2], 20,item_1,medium); 
          draw_note_custom(items[3], 20,item_2,medium); 
          draw_note_custom(items[4], 20,item_3,medium);
          DisplayCurssor(item_3); 
        break;           
     }
    }
    else if(new_st == 3){
         draw_note_custom(items[0], 20,item_1,medium);
         DisplayCurssor(item_1);      
    }   
}

#endif KEYPAD_h
