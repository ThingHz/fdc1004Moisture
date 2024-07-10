#ifndef HARDWAREDEFS_H
#define HARDWAREDEFS_H

//Values corresponding to specific application
#define IS_LOADCELL_REQUIRED        1
#define IS_BLE_REQUIRED             1
//#define IS_CALLIBRATION_REQUIRED    1
//#define IS_CAP_REQUIRED             1
   
#define INVALID_CAP_VALUE       99
#define INVALID_THERM_VALUE     99
#define INVALID_TEMP_READING    99
#define INVALID_SCALE_READING   99
#define INVALID_HUMIDITY_READING 99
#define BATT_VOL_100            100
#define BATT_VOL_0              0


//values corresponding to OLED
#define SCREEN_WIDTH                            128 // OLED display width, in pixels
#define SCREEN_HEIGHT                           64 // OLED display height, in pixels


//Values corresponding to FDC
#define UPPER_BOUND  0X4000                 // max readout capacitance
#define LOWER_BOUND  (-1 * UPPER_BOUND)
#define CHANNEL 0                          // channel to be read
#define MEASURMENT 0                       // measurment channel

//values corresponding to thermister
#define SAMPLE_NUMBER_CAPACITANCE   50 //010921
#define SAMPLE_NUMBER_THERMISTER    20
#define BALANCE_RESISTOR            10000
#define ROOM_TEMP                   298.15
#define RESISTOR_ROOM_TEMP          10000.0
#define THERMISTOR_PIN              21
#define READ_THERMISTER_READING     20    // interval for reading sensor data in gateway 100
#define BETA                        3799.42
#define FDC_CONTROL_PIN             39

#define C1 16 //310821
#define C2 17
#define C3 18
#define C4 19 

//values Corresponding to load Cell
#define LOADCELL_DOUT_PIN           32 //2  //190821
#define LOADCELL_SCK_PIN            27 //3
#define LOAD_CELL_CALL_VALUE        2280.f

#define MILLI_SECS_MULTIPLIER                   1000

//values corresponding to BLE
#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" 
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"


#define DEBUG_PRINTF(...)           Serial.printf(__VA_ARGS__)
#define DEBUG_PRINTLN(...)          Serial.println(__VA_ARGS__)

#define CONDUCTION_SENSOR_PIN       A0

#define BLE_NOTIFY_TIMER          5000

#define WEIGHT_CHECK_TIMER          1000

unsigned long startMillisLoadCell = 0;
unsigned long startMillisBLE = 0;

#endif
