#ifndef DEVICESTATE_H
#define DEVICESTATE_H

#include "hardwareDefs.h"

enum DisplayMode {
  DisplayTemp,
  DisplayMoisture,
  DisplayMessage,
  DisplayCenterTextLogo,
  DisplayDeviceStatus,
  DisplayDeviceHealth,
  DisplayLoadCellInrange,
  DisplayLoadCellOutrange,
  DisplayCapcitanceRequired,  
  DisplayMenu,
  DisplayInfo,
  DisplayCapcitance
};

enum calculate_moisture {
  None,
  WheatMoisture,
  MaizeMoisture, 
  RiceMoisture,
  MoongMoisture,
  PaddyMoisture
}


class RunTimeState {
  public:
    RunTimeState(): capacitanceVal(INVALID_CAP_VALUE),
      batValue(BATT_VOL_0),
      thermistorVal(INVALID_THERM_VALUE),
      displayState(DisplayCenterTextLogo),
      isSplashTimeOver(false),
      isDeviceConnected(false),
      isBLEActive(false),
      averageCapValue(INVALID_CAP_VALUE),
      isFDCRead(true),
      roomTemp(INVALID_TEMP_READING),
      roomHumid(INVALID_HUMIDITY_READING),
      readScale(INVALID_SCALE_READING),
      isWeightOkay(false),
      moistureVal(INVALID_CAP_VALUE),
      isComoditySelected(false)
     {
      
     }

    void calculateMoisture(float capacitance, int commodity_select){
          switch(commodity_select){
           case calculate_moisture::WheatMoisture:
                moistureVal = 4.76+ (0.24 * capacitance);
                break;
           case calculate_moisture::MaizeMoisture:
                moistureVal = 7.52+ (0.21 * capacitance);
                break;
           case calculate_moisture::PaddyMoisture:
                moistureVal = 4.40+ (0.48 * capacitance);
                break;
           default:
                moistureVal = 4.76+ (0.24 * capacitance);
                break;
          }
     }
 
    float capacitanceVal;
    int batValue;
    double thermistorVal;
    DisplayMode displayState;
    bool isSplashTimeOver;
    bool isDeviceConnected;
    bool isBLEActive; 
    float averageCapValue;
    bool isFDCRead;
    float roomTemp;
    float roomHumid;
    long readScale;
    bool isWeightOkay;
    float moistureVal;
    bool isComoditySelected;
};

RunTimeState runTimeState;

#define RSTATE runTimeState

#endif
