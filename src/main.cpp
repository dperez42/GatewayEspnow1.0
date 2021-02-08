#include "main.hpp"

char l1[25]; //concentracion de ozono peligrosa en ppb (50)
char l2[25]; //concentracion de ozono en desinfección en ppb (400)
char t1[25]; //tiempo maximo de servicio en min (seguridad)
char t2[25]; //tiempo maximo de desinfección en min
int l1i;
int l2i;
int t1i;
int t2i;
boolean isConnectToWifi;
char ssid[50];      
char password[50];
int id_gate;
unsigned long timerRecieve;  // Set time to get order from server to 2 seconds 
unsigned long lastTimeRecieve;
unsigned long timerSender;  // Set time to sent data to server to 15 seconds 
unsigned long lastTimeSender;

unsigned long last;
unsigned long limitLastRcv;

////// BluetoothSerial /////////
BluetoothSerial SerialBT;
String msgBT;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org",3600, 160000); //+3600 Madrid, 60s to check

////// Display ////////////////
//#ifdef DISPLAY_OK
//U8X8_SSD1306_128X64_NONAME_HW_I2C display(/*rst*/ 16, /*scl*/ 15, /*sda*/ 4);
//#endif


////// Espnow ////////////////
boolean newData;
// Variable to store if sending data was successful
String success;
unsigned long timeLastRcv;          //tiempo desde el ultimo dato recibido
esp_now_peer_info_t peerInfo;
struct_message myDataSensor;
struct_messagegw myDataGateway;
uint8_t espkey[16] = {0x33, 0x44, 0x33, 0x44, 0x33, 0x44, 0x33, 0x44, 0x33, 0x44, 0x33, 0x44, 0x33, 0x44, 0x33, 0x44};
uint8_t espnodo[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

////// HTTP variables ////////////////
String httpsenddata;
String httpgetorder;

////// Service Control ////////////////
int serviceState;    //0: standby 1:Service, 2:Desinfección, 3:Ventilación >0 en servicio
int prevState;
int ventilatorState; //0: off 1:on
int desinfectionState;
unsigned long timeStartSvc;         //tiempo en millis de incio de servicio
unsigned long timeSvc;              //tiempo acumulado de servicio
unsigned long timeStartDes;         //tiempo en millis de incio desinfección
unsigned long timeDes;              //tiempo acumulado de desinfeccion

////// Led State ////////////////
unsigned long lastTLedState;
boolean ledStateOn;

//----------------------------------------------------------------
//get NTP Time
//----------------------------------------------------------------

String getTimeByNTP(){
  /*
  Serial.println("Entroe");
  struct tm timeinfo;
  // char *buf[40];
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return ;
  }
  Serial.println("Salgo");
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  //2020-10-02T10-8-38Z
  //sprintf(timeString, "%04d-%02d-%02dT%02d-%02d-%02dZ",time_info->tm_year,time_info->tm_mon,time_info->tm_mday,  time_info->tm_hour, time_info->tm_min, time_info->tm_sec);
  //Serial.println(timeString);
  return ;
  */

  // Need to be update.
  if (!timeClient.update()) {
   Serial.print(".");
   timeClient.forceUpdate();
  } else {
    //Serial.print("Unable to Update Time");     
  }
  // The formattedDate comes with the following format:
  // 2018-05-28T16:00:13Z
  String formattedDate = timeClient.getFormattedDate();
  //Serial.print("Actual Date/Time:");
  //Serial.println(formattedDate);
  
  //long unsigned int timestampNTP = timeClient.getEpochTime()- 2*60*60; //correción para funcion posterior
  //Serial.print("timestamp:");
  //Serial.println(timestampNTP);
  //String msg1 = "#T{\"actualTimestamp\": \"" + String(timestampNTP) + "\"}";
  //Serial.println(msg1);
  //Para ajuste de hora invierno o verano ver fecha y cambiar el timestampNTP sumando o restando una hora (3600)
  return formattedDate;
}


void setup() {
  Serial.begin(115200);
  isConnectToWifi = true;
  timerRecieve = TIME_RCV_SERVER;  // Set time to get order from server to 2 seconds 
  timerSender = TIME_SEND_SERVER;  // Set time to sent data to server to 15 seconds 
  limitLastRcv = TIME_LIMIT_RECIEVED_FROM_NODO;
  httpsenddata = HTTP_SEND_DATA;  
  httpgetorder = HTTP_GET_ORDER;
  newData = false;
  serviceState = 0;    //0: standby 1:Service, 2:Desinfección, 3:Ventilación >0 en servicio
  prevState = 0;
  ventilatorState = 0; //0: off 1:on
  desinfectionState = 0;  
  ledStateOn = false;
  //espkey[] = {0x33, 0x44, 0x33, 0x44, 0x33, 0x44, 0x33, 0x44, 0x33, 0x44, 0x33, 0x44, 0x33, 0x44, 0x33, 0x44};
  //espnodo[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  /// SETUP PINS
  digitalWrite(PIN_RELAY, LOW);
  pinMode(PIN_RELAY, OUTPUT);
  pinMode(PIN_BUTTON, INPUT);
  ledcSetup(0,5000,12);
  ledcAttachPin(PIN_SVC,0);
  ledcSetup(1,5000,12);
  ledcAttachPin(PIN_RED,1);
  ledcSetup(2,500,12);
  ledcAttachPin(PIN_GREEN,2);
  
  id_gate = getIdEsp32();
  
  //Retrieve from eeprom
  initEeprom();

  // Set device as a Wi-Fi Station
  //init_ESPNOW();
  connectSsdi();

  timeClient.begin();
  Serial.println("NTPClient iniciado.");

  // BLUETOOTH setup
  SerialBT.begin("OzoneViewGW " + String(id_gate)); //Bluetooth gateway name
  Serial.print("Inicializado Bluetooth:");
  Serial.println("OzoneViewGW " + String(id_gate));
  
  //inicio los timers
  lastTimeRecieve = millis();
  lastTimeSender = millis();
}

void loop() {

       //check wifi connect
   //getTimeByNTP();
   if (WiFi.status() != WL_CONNECTED){
    Serial.println("try to connect to external wifi");
    isConnectToWifi = false;
    connectSsdi();
  }
  //Leds y reset button
  ledsState();
  ledsService();
  if (digitalRead(PIN_BUTTON)){reset();}

  //leo puerto serie y Bluetooth 
  msgBT = "";
  while (Serial.available()){
    if (Serial.available()>0){
      char c = Serial.read();
      msgBT += c;
      }
      String mSR = "SR:" + String(msgBT.c_str());
           Serial.println(mSR);
     //display.setCursor(0,2);
     //display.print(mSR);
    }
  while (SerialBT.available()){
    if (SerialBT.available()>0){
      char c = SerialBT.read();
      msgBT += c;
      }
      String mBT = "BT:" + String(msgBT.c_str());
      //display.setCursor(0,2);
      //display.print(mBT);
  }

  SerialBT.flush();

  if (msgBT != "") {setorder(msgBT);}
  
  // Send message via ESP-NOW
  if (millis() - last > TIME_SEND_TO_NODO) {     //&& (isConnect == true )
        myDataGateway.DevType = DEVICE_TYPE;
        myDataGateway.l1 = l1i;
        myDataGateway.l2 = l2i;
        // Send message via ESP-NOW
        esp_err_t result = esp_now_send(espnodo, (uint8_t *) &myDataGateway, sizeof(myDataGateway));         
        if (result == ESP_OK) {
          if (DEBUG){
              Serial.println("###########################################");
              Serial.println("sent");
              Serial.print("Limite inferior(l1): ");
              Serial.print(myDataGateway.l1);
              Serial.print(", Limite desinfección(l2): ");
              Serial.println(myDataGateway.l2);
              Serial.printf("sent: %3u on channel: %u\n", myDataGateway, WiFi.channel());
              Serial.println("Sent with success");
              Serial.println("###########################################");
          }
     } else {
        if (DEBUG){
           Serial.println("###########################################");
           Serial.println("Error sending the data");
           Serial.println("###########################################");
        }
     }
    last = millis();
    }

    
//Check service state
    if (serviceState > 0){
      timeSvc = millis() - timeStartSvc;
      if (myDataSensor.o3 > l2i)
      {
          if (serviceState != 2){
            timeStartDes = millis();
            serviceState = 2;
            prevState = 1;
          } else {
            timeDes = millis() - timeStartDes;
            if (timeDes/(1000*60) >= t2i){          //se ha cumplido el tiempo de desinfección paso de 2 a 3
                serviceState = 3;
                prevState = 2;
                timeDes = 0;
                //display.setCursor(6,7);
                //display.print("  ");
                stopService();                      // Paro el reactor
            }
          }
      } else 
      { 
        if (serviceState == 3){       // en fase de desinfección ha caido por debajo de la concentración reinicio el servicio
          prevState = 3;
          serviceState = 2;
          timeDes = 0;
        }
      }
      if (myDataSensor.o3 < l1i && prevState == 3)                 //se ha terminado la desinfección paso a ventilación
      {
        serviceState = 0;
        stopVentilator();
      }

      //Si se ha superado el tiempo de servicio max: parada por seguridad o no hay sensor
      if (timeSvc/(60*1000) >= t1i){        
        if(LOG) {ft_log(131);}
        stopService();
      }
      if  ((millis()-timeLastRcv) > limitLastRcv){  
        if(LOG) {ft_log(130);}
        stopService();
      }
    }

    //Comunication with server
    if ( ( millis() - lastTimeRecieve > timerRecieve) && ( isConnectToWifi == true ))
    {
      if (DEBUG) {Serial.println("Asking for Orders");}
      listenHTTP(getTimeByNTP());
      lastTimeRecieve=millis();
    }
    
    //Send data
    if ( ( millis() - lastTimeSender > timerSender) && ( isConnectToWifi == true ) && newData == true)
      {
      if (DEBUG) {Serial.println("Sending Data");}
      sendHTTP(getTimeByNTP());
      newData = false;
      lastTimeSender=millis();
      }

    //showUp();
  //delay(500);  
}