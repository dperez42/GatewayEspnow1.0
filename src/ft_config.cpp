//Mandar nueva ssid y password
//#S{"Ssid":"MIWIFI_2G_RKkD","Password":"C6PrnauD"}
//#S{"Ssid":"iPhone de Paula","Password":"alicia88"}
//#S{"Ssid":"iPhone de DANIEL","Password":"paswordoo"}
//#S{"Ssid":"Anto´s Iphone","Password":"rodri03richi05"}
//#S{"Ssid":"HUAWEI","Password":"password"}
//Mandar nueva configuración:
// t1: Máximo tiempo de servicio, t2: Tiempo de desinfección
// l1: Limite mínimo de O3, l2: Concentración de O3 para desinfección
//#C{"l1":"50","l2":"400", "t1":"120", "t2":"10"}

#include "main.hpp"

//----------------------------------------------------------------
//new wifi from app
//----------------------------------------------------------------
void newSsid(String msg){
  char JSONssid[100];
  const char *ssidConf;
  const char *passConf;
  int i = 2;
  
  while (i < msg.length()){
    JSONssid[i-2] = msg[i];
    i++;
  }
  JSONssid[i-2] = '\0';
  Serial.println("One wifi recieved.");
  StaticJsonDocument<200> JSONBuffer;
  DeserializationError error = deserializeJson(JSONBuffer, JSONssid);
  if (error) {   //Check for errors in parsing
    if (LOG) {ft_log(132);}
    Serial.println("Parsing failed");
    //delay(100);
   } else {
      if (LOG) {ft_log(120);}
      ssidConf = JSONBuffer["Ssid"];
      passConf = JSONBuffer["Password"]; 
      Serial.println(ssidConf);
      Serial.println(passConf);
      saveToEeprom(0,ssidConf);
      saveToEeprom(50,passConf);
      loadFromEeprom(0).toCharArray(ssid, 50);
      loadFromEeprom(50).toCharArray(password, 50);
      Serial.println ("New SSID save");
      Serial.print("ssdi:");
      Serial.println (ssid);
      Serial.print("password:");
      Serial.println (password);
      ESP.restart();
      return;  
      }
}

//----------------------------------------------------------------
//New config from app
//----------------------------------------------------------------
void newConfig(String msg){
  char JSONconfig[100];
  const char *l1Conf;
  const char *l2Conf;
  const char *t1Conf;
  const char *t2Conf;
  int i = 2;
  while (i < msg.length()){
    JSONconfig[i-2] = msg[i];
    i++;
  }
  JSONconfig[i-2] = '\0';
  Serial.println("New config recieved.");
  StaticJsonDocument<200> JSONBuffer;
  DeserializationError error = deserializeJson(JSONBuffer, JSONconfig);
  if (error) {   //Check for errors in parsing
    if (LOG) {ft_log(133);}
    Serial.println("Parsing failed");
    //delay(100);
   } else {
      if (LOG) {ft_log(121);}
      l1Conf = JSONBuffer["l1"]; //limite O3 ozono = 50 ppb
      l2Conf = JSONBuffer["l2"]; //limite O3 ozono desinfeccion = 400 ppb
      t1Conf = JSONBuffer["t1"]; //tiempo de tratamiento en minutos = 10 mins
      t2Conf = JSONBuffer["t2"]; //tiempo de tratamiento en minutos = 10 mins
      Serial.println ("New Config recieve:");
      Serial.print("Concentración O3 en ppb no saludable:");
      Serial.println (l1Conf);
      Serial.print("Concentratción O3 en ppb para desinfección:");
      Serial.println (l2Conf);
      Serial.print("Tiempo limite en minutos para servicio:");
      Serial.println (t1Conf);
      Serial.print("Tiempo en minutos para desinfección:");
      Serial.println (t2Conf);
      //Salvo en memoria )0:ssid, 50:password, 100:l1, 150;l2, 200:t2
      saveToEeprom(100,l1Conf);
      saveToEeprom(150,l2Conf);
      saveToEeprom(200,t1Conf);
      saveToEeprom(250,t2Conf);
      //Grabo de memoria
      loadFromEeprom(100).toCharArray(l1, 50);
      loadFromEeprom(150).toCharArray(l2, 50);
      loadFromEeprom(200).toCharArray(t1, 50);
      loadFromEeprom(250).toCharArray(t2, 50);
      Serial.println ("New Config save");
      Serial.print("Concentración O3 en ppb no saludable:");
      Serial.println (l1);
      Serial.print("Concentración O3 en ppb para desinfección:");
      Serial.println (l2);
      Serial.print("Tiempo máximo en servicio:");
      Serial.println (t1);
      Serial.print("Tiempo en minutod para desinfección:");
      Serial.println (t2);
      l1i = atoi(l1);
      l2i = atoi(l2);
      t1i = atoi(t1);
      t2i = atoi(t2);  
      return;  
      }
}
