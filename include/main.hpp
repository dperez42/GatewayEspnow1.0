//#pragma once

#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include <esp_now.h>
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_event.h"
#include "esp_pm.h"
#include <WiFi.h>
#include "BluetoothSerial.h" //Biblioteca para Bluetooth
#include <U8x8lib.h>        //Biblioteca para Pantalla
#include <NTPClient.h>  //Biblioteca para Tiempo NTP
#include "ArduinoJson.h"     //Biblioteca para Json
#include <EEPROM.h>          //Biblioteca para grabar en EEPROM
#include <HTTPClient.h> //Biblioteca para HTTP


#define DEBUG false
#define LOG   true

#define DEVICE_TYPE     1
#define TIME_RCV_SERVER               2000    //Tiempo limite para recibir del servidor
#define TIME_SEND_SERVER              15000  //Sample Tiempo para mandar al servidor
#define TIME_LIMIT_RECIEVED_FROM_NODO 30000  //Tiempo limite para recibir del sensor
#define TIME_SEND_TO_NODO             1000
#define KEY {0x33, 0x44, 0x33, 0x44, 0x33, 0x44, 0x33, 0x44, 0x33, 0x44, 0x33, 0x44, 0x33, 0x44, 0x33, 0x44};
#define NODO {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
//#define NODO {0x7C,0x9E,0xBD,0xF3,0x53,0x28}; //SENSOR MAC DANIEL
//#define NODO {0x7C,0x9E,0xBD,0xF3,0xDF,0x3C}; //SENSOR MAC ANTOLIN
#define HTTP_SEND_DATA "http://www.glidingbytes.com/Ozono/guarda_dato";
#define HTTP_GET_ORDER "http://www.glidingbytes.com/Ozono/setEstado?json={\"id\":\"1\",\"id_state\":\"5\",\"t_svc\":\"120\",\"t_des\":\"15\"}"; 



#define PIN_SVC 13                     
#define PIN_RED 12                       
#define PIN_GREEN 14                      
#define PIN_WIFI 27                       
#define PIN_BUTTON 26
#define PIN_BUZZER 25
#define PIN_RELAY 33
#define PIN_VENT 32
#define DISPLAY_OK true

extern int id_gate;


extern char l1[25]; //concentracion de ozono peligrosa en ppb (50)
extern char l2[25]; //concentracion de ozono en desinfección en ppb (400)
extern char t1[25]; //tiempo maximo de servicio en min (seguridad)
extern char t2[25]; //tiempo maximo de desinfección en min
extern int l1i;
extern int l2i;
extern int t1i;
extern int t2i;

//#S{"Ssid":"MIWIFI_2G_RKkD","Password":"C6PrnauD"}
//#S{"Ssid":"iPhone de Paula","Password":"alicia88"}
//#S{"Ssid":"iPhone de DANIEL","Password":"paswordoo"}
//#S{"Ssid":"Anto´s Iphone","Password":"rodri03richi05"}
//#S{"Ssid":"HUAWEI","Password":"password"}
extern boolean isConnectToWifi;
//char* ssid = "MIWIFI_2G_RKkD";
//char* password = "C6PrnauD";
extern char ssid[50];      
extern char password[50];

extern boolean newData;
extern uint8_t espnodo[];
extern uint8_t espkey[];
// Variable to store if sending data was successful
extern String success;
extern unsigned long timeLastRcv;          //tiempo desde el ultimo dato recibido
extern esp_now_peer_info_t peerInfo;
// Structure to send data
// Must match the sender structure
typedef struct struct_message {
  int               id;
  unsigned char     DevType;
  unsigned char     VSoft;
  unsigned char     VHard;
  unsigned int      o3;                       //O3 Low
  unsigned int      o3H;                      //O3 High
  unsigned int      temp;                     //Temperature
  unsigned int      hum;                      //Humidity
  unsigned char     bat;                      //Battery    
  int               log;
} struct_message;

extern struct_message myDataSensor;

// Structure to recieve data
// Must match the sender structure
typedef struct struct_messagegw {
  unsigned char     DevType;
  unsigned int      l1;
  unsigned int      l2;
} struct_messagedw;

extern struct_messagegw myDataGateway;


extern int serviceState;    //0: standby 1:Service, 2:Desinfección, 3:Ventilación >0 en servicio
extern int prevState;
extern int ventilatorState; //0: off 1:on
extern int desinfectionState;
extern unsigned long timeStartSvc;         //tiempo en millis de incio de servicio
extern unsigned long timeSvc;              //tiempo acumulado de servicio
extern unsigned long timeStartDes;         //tiempo en millis de incio desinfección
extern unsigned long timeDes;              //tiempo acumulado de desinfeccion

extern unsigned long lastTLedState;
extern boolean ledStateOn;

extern String httpsenddata;
extern String httpgetorder;

extern void ft_log(int nb_log);

extern void saveToEeprom(int addr, String a);
extern String loadFromEeprom(int addr);
extern void initEeprom();

extern uint32_t getIdEsp32();
extern void reset();

extern int init_ESPNOW();
extern void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
extern boolean checkMac(const uint8_t *sender, const uint8_t *receiver);
extern void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len);

void connectSsdi();

void initVentilator();
void stopVentilator();
void initService();
void stopService();

void ledsState();
void ledsService();

void showUp();

void setorder(String msgBT);
void newSsid(String msg);
void newConfig(String msg);

void listenHTTP(String Actual_Time);
void sendHTTP(String Actual_Time);

#endif