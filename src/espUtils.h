#ifndef ESPUTILS_H
#define ESPUTILS_H
#include <WiFi.h>
#include <WiFiUdp.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>


class espUtils {
  public:
    //read battery value in percentage
    int getBatteryPercentage(const float battVoltage) {
      int maxVal = BATT_VOL_100 * 100; // scale by 100
      int minVal = BATT_VOL_0 * 100; // scale by 100
      int percentage = 0;
      if (battVoltage > maxVal) {
        maxVal = battVoltage;
      }

      //DEBUG_PRINTF("Max Value %d \n",maxVal);
      percentage = map(int(battVoltage * 100), minVal, maxVal, 0, 100);
      //DEBUG_PRINTF("Battery Percentage %d \n",percen);
      if (percentage < 0) {
        percentage = 0;
      }  if (percentage > 100) {
        percentage = 100;
      }
      return percentage;
    }

    
};
#endif ESPUTILS_H
