#include "main.hpp"
/////////////////////////////////////// LOGS /////////////////////////////////

void ft_log(int nb_log)
{
  String msg;
  if (nb_log == 101) { msg = " STATE: Standby";}
  if (nb_log == 102) { msg = " STATE: Inicio de Servicio";}
  if (nb_log == 103) { msg = " STATE: Inicio de Desinfección";}
  if (nb_log == 104) { msg = " STATE: Fin de Desinfección";}
  if (nb_log == 105) { msg = " STATE: Inicio de Ventilación";}
  if (nb_log == 106) { msg = " STATE: Fin de Ventilación";}
  if (nb_log == 107) { msg = " STATE: Fin de Servicio";}
  if (nb_log == 108) { msg = " STATE: Parada de Servicio";}


  if (nb_log == 110) { msg = " STATE: Arranca Reactor";}
  if (nb_log == 111) { msg = " STATE: Arranca Ventilador";}
  if (nb_log == 112) { msg = " STATE: Parando Reactor";}
  if (nb_log == 113) { msg = " STATE: Parando Ventilador";}

  if (nb_log == 120) { msg = " CONFIG: Recibida nueva configuración WIFI";}
  if (nb_log == 121) { msg = " CONFIG: Recibida nueva configuración LIMITES";}

  if (nb_log == 130) { msg = " ERROR: Pérdida de conexión con sensor";}
  if (nb_log == 131) { msg = " ERROR: Tiempo límite de servicio sobrepasado, apagando....";}
  if (nb_log == 132) { msg = " ERROR: Parsing error en nueva configuración WIFI";}
  if (nb_log == 133) { msg = " ERROR: Parsing error en nueva configuración LIMITES";}
  
  Serial.print("LOG: ");
  Serial.print(nb_log);
  Serial.print(":");
  Serial.println(msg);
}

