#include "main.hpp"

//--------------------------------------------------------------------
// Init Ventilator
//--------------------------------------------------------------------
void initVentilator(){
  digitalWrite(PIN_VENT,HIGH);
  ventilatorState = 1;
  if (LOG) {ft_log(111);}
  return;
}

//--------------------------------------------------------------------
// Stop Ventilator
//--------------------------------------------------------------------
void stopVentilator(){
     digitalWrite(PIN_VENT,LOW);
     ventilatorState = 0;
     if (LOG) {ft_log(113);}
     return;
}

//--------------------------------------------------------------------
// Init Service
//-----------------
void initService()
{
  digitalWrite(PIN_RELAY,HIGH);
  initVentilator();
  serviceState = 1;
  prevState = 0;
  timeStartSvc = millis();
  if (LOG) {ft_log(110);}
  return;
}

//--------------------------------------------------------------------
// Stop Service
//--------------------------------------------------------------------
void stopService()
{
  digitalWrite(PIN_RELAY,LOW);
  stopVentilator();
  serviceState = 0;
  if (LOG) {ft_log(112);}
  return;
}