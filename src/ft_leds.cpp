#include "main.hpp"

//--------------------------------------------------------------------
// Led State O3
//--------------------------------------------------------------------
void ledsState(){
  int duration = 500;
  if ((millis() - lastTLedState) > duration) {
    if (ledStateOn == true) {
      if (myDataSensor.o3 >= l1i) {
        ledcWrite(2 , 0);
        ledcWrite(1 , 0);} 
        else {
          ledcWrite(2 , 0);
          ledcWrite(1 , 0);
          }
      ledStateOn = false;
    } 
    else {
      if (myDataSensor.o3 < l1i) {
        ledcWrite(2 , 1500);} 
        else {
          ledcWrite(1 , 1500);
          }      
        ledStateOn = true;
    }
    lastTLedState = millis();
  }
  return;
} 

//--------------------------------------------------------------------
// Led State Service
//--------------------------------------------------------------------
void ledsService(){
    if (serviceState == 0) {
        ledcWrite(0 , 0);
    } 
    else {
        ledcWrite(0 , 1500);
    }
  return;
}