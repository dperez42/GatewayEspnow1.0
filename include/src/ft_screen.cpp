#include "main.hpp"

////// Display ////////////////
#ifdef DISPLAY_OK
U8X8_SSD1306_128X64_NONAME_HW_I2C display(/*rst*/ 16, /*scl*/ 15, /*sda*/ 4);
#endif

void showUp(){
    display.setPowerSave(false); 
      if (isConnectToWifi == true){
    display.setCursor(12,7);
    display.setInverseFont(1);
    display.print("WIFI"); 
    display.setInverseFont(0);
    } else {
    display.setCursor(12,7);
    display.print("WIFI"); 
    }
    if ( serviceState>0 ){
    display.setCursor(0,7);
    display.setInverseFont(1);
    display.print("SVC"); 
     display.setInverseFont(0);
    display.setCursor(4,7);
    display.print(timeSvc/(60*1000));
    if  (desinfectionState == 1){
    display.setCursor(6,7);
    display.setInverseFont(1);
    display.print(timeDes/(60*1000));
     display.setInverseFont(0);
    } else { 
    display.setCursor(6,7);
    display.print("   "); 
    }   
    } else {
    display.setCursor(0,7);
    display.setInverseFont(0);
    display.print("SVC"); 
    display.setCursor(4,7);
    display.print("   "); 
    display.setCursor(6,7);
    display.print("   "); 
    }
    if  (desinfectionState == 1){
    display.setCursor(6,7);
    display.setInverseFont(1);
    display.print(timeDes/(60*1000));
     display.setInverseFont(0);
    } else { 
    display.setCursor(6,7);
    display.print("   "); 
    }
    if (ventilatorState==1){
      display.setCursor(8,7);
      display.setInverseFont(1);
      display.print("VEN");
      display.setInverseFont(0);
    } else {
      display.setCursor(8,7);
      display.print("VEN");
    }
   
    display.setCursor(0,3);
    display.print("Lseg:");  
    display.setCursor(5,3);
    display.print(int(myDataGateway.l1)); 
    display.setCursor(7,3);
    display.print(",Ldes:");
    display.setCursor(13,3);
    display.print(int(myDataGateway.l2));        
    display.setCursor(0,4);
    display.print("Tmax:");
    display.setCursor(5,4);
    display.print(int(t1i));
    display.setCursor(8,4);
    display.print(",Tdes:");
    display.setCursor(14,4);
    display.print(int(t2i));
    display.setCursor(0,5);
    display.print("Tx from: ");
    display.setCursor(8,5);
    display.print((int)myDataSensor.id);
    display.setCursor(0,6);
    display.print("O3:");
    display.setCursor(3,6);
    display.print("    ");
    display.setCursor(3,6);
    display.print((int)myDataSensor.o3);
    display.setCursor(7,6);
    display.print("T:");
    display.setCursor(9,6);
    display.print("   ");
    display.setCursor(9,6);
    display.print((int)myDataSensor.temp);
    display.setCursor(12,6);
    display.print("H:");
    display.setCursor(14,6);
    display.print("  ");
    display.setCursor(14,6);
    display.print((int)myDataSensor.hum);
    
  }