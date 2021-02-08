//Inicio de servicio:
//#J{}

#include "main.hpp"

void setorder(String msgBT){

    // Ordenes from Serial o BT
  if (msgBT[0]=='#'){       //todos los mensajes recibidos por BT empiezan #   
      if (msgBT[1]=='S'){     //Nueva SSDI y PASSWORD
        isConnectToWifi = false;
        newSsid(msgBT);
      }
      if (msgBT[1]=='C'){     //Nuevos CONFIG límites 1 y 2
        newConfig(msgBT);     // Save new config
      }
      
      if (msgBT[1]=='J'){     //Inicio Servicio
       if (serviceState == 0) {
          ft_log(102);
          if(LOG) {initService();}
          } else { 
          if(LOG) {ft_log(108);}
          stopService();
          }
      }
    }
    return;
}