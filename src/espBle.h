#include "BLEDevice.h"
#include "BLEServer.h"
#include "BLEUtils.h"
#include <BLEUtils.h>
#include <BLE2902.h>
#include "hardwareDefs.h"
#include <string>

void bleState(std::string data_rx); //160921

class ESPble
{
    BLEServer *pServer = NULL;
    BLECharacteristic *pTxCharacteristic;
  public:
    bool createServiceAndCharacteristic()
    {
      
      BLEDevice::init("Meter");
      pServer = BLEDevice::createServer();
      
      pServer->setCallbacks(new ServiceCallback());
      BLEService *pService = pServer->createService(SERVICE_UUID);

      pTxCharacteristic = pService->createCharacteristic(
                          CHARACTERISTIC_UUID_TX,
                          BLECharacteristic::PROPERTY_NOTIFY);

      pTxCharacteristic->addDescriptor(new BLE2902());

      BLECharacteristic * pRxCharacteristic = pService->createCharacteristic(
                          CHARACTERISTIC_UUID_RX,
                          BLECharacteristic::PROPERTY_WRITE);

      pRxCharacteristic->setCallbacks(new CharacteristicCallback());

      pService->start();
      pServer->getAdvertising()->start();
      Serial.println("Waiting a client connection to notify...");
      return true;
    }

    void startAdvertising(bool isDeviceConnected) {
      if (!isDeviceConnected) {
        DEBUG_PRINTLN("device not connected, Advertise");
        delay(500);
        pServer->startAdvertising(); // restart advertising
      }
    }

    void startNotify(){
      char moisturePayload[10];
        sprintf (moisturePayload, "[%.2f][%.2f]",RSTATE.averageCapValue,RSTATE.moistureVal);
        std::string txValue = moisturePayload;
        pTxCharacteristic->setValue(txValue);
        pTxCharacteristic->notify();
      }   
      
   void send_data(std::string dataToSend){ //160921
     pTxCharacteristic->setValue(dataToSend);
     pTxCharacteristic->notify();
   }
   
  private:
    class ServiceCallback : public BLEServerCallbacks
    {
        void onConnect(BLEServer *pServer)
        { 
          Serial.println("BLE CONNECTED");                    
          RSTATE.isDeviceConnected = true;
        };

        void onDisconnect(BLEServer *pServer)
        { 
          Serial.println("BLE DISCONNECTED");
          RSTATE.isDeviceConnected = false;
        }
    };

    class CharacteristicCallback : public BLECharacteristicCallbacks
    {
      /*  void onWrite(BLECharacteristic *pCharacteristic)
        {
          std::string rxValue = pCharacteristic->getValue();
          DEBUG_PRINTF("value: %s", rxValue.c_str());
          if(rxValue == "0"){
              RSTATE.isFDCRead = false;
            }
          if(rxValue == "1"){
              RSTATE.isFDCRead = true;
            }  
        }
       */
      void onWrite(BLECharacteristic *pCharacteristic)
        {
          std::string rxValue = pCharacteristic->getValue();
          DEBUG_PRINTF("\n value: %s ", rxValue.c_str());          
          bleState(rxValue);
        }  
    };

};
ESPble espBle;

void activeBle(){
#ifdef IS_BLE_REQUIRED
  if (!RSTATE.isBLEActive) {
    espBle.createServiceAndCharacteristic();
  } else {
    espBle.startAdvertising(RSTATE.isDeviceConnected);
  }
  if (RSTATE.isBLEActive && !RSTATE.isDeviceConnected) {
    espBle.startAdvertising(RSTATE.isDeviceConnected);
  }
 /* if ((millis() - startMillisBLE) >= BLE_NOTIFY_TIMER) {
    espBle.startNotify();
  }
  */
#endif 
}

void TurnBleOff(){       
   esp_bt_controller_disable();
      /*esp_bluedroid_disable();
      esp_bluedroid_deinit();
      esp_bt_controller_disable();
      esp_bt_controller_deinit();
      esp_bt_mem_release(ESP_BT_MODE_BTDM); */
}
void TurnBleOn(){
   esp_bt_controller_enable(ESP_BT_MODE_BTDM);
   activeBle();
}   
    
