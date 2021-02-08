#include "main.hpp"

//----------------------------------------------------------------
//Red ESPNOW
//----------------------------------------------------------------
// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  if (DEBUG){
    Serial.print("\r\nLast Packet Send Status:\t");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  }
  if (status ==0){
    success = "Delivery Success :)";
  }
  else{
    success = "Delivery Fail :(";
  }
}
// Check MACs
boolean checkMac(const uint8_t *sender, const uint8_t *receiver){
  int i;
  i=0;
  while (i < 6){
    if (sender[i] != receiver[i]){
      return false;
    }
    i++;
  }
  return true;
}
// Callback when data is received
void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) {
//if (checkMac(mac_addr, espnodo)){    
    memcpy(&myDataSensor, incomingData, sizeof(myDataSensor));
    char macStr[18];
    if (DEBUG){
      Serial.println("###########################################");
      Serial.print("Packet received from: ");
      snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
               mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
      Serial.println(macStr);
      Serial.println("received: ");
      Serial.print("id: ");
      Serial.print(myDataSensor.id);
      Serial.print(", Tipo Device: ");
      Serial.print(myDataSensor.DevType);
      Serial.print(", Hardware: ");
      Serial.print((float)myDataSensor.VHard/10.0f);
      Serial.print(", Software: ");
      Serial.print((float)myDataSensor.VSoft/10.0f);
      Serial.print(", O3 Low: ");
      Serial.print((float)myDataSensor.o3/10.0f);
       Serial.print(", O3 High: ");
      Serial.print((float)myDataSensor.o3H/10.0f);
      Serial.print(", temp: ");
      Serial.print((float)myDataSensor.temp/10.0f);
      Serial.print(", hum: ");
      Serial.print((float)myDataSensor.hum/10.0f);
      Serial.print(", bat: ");
      Serial.print((float)myDataSensor.bat/100.0f);
      Serial.print(", readingId: ");
      Serial.println(myDataSensor.log);
      Serial.println("");
      Serial.print("Total Bytes received: ");
      Serial.println(len);
      Serial.println("###########################################");
      }
    timeLastRcv = millis();
    newData = true;
//}
}

int init_ESPNOW(){
  // Init ESP-NOW
  delay(5000);
  if (esp_now_init() != ESP_OK) {
   if (DEBUG) {Serial.println("Error initializing ESP-NOW");}
   if (LOG) {ft_log(10);}
    return 1;
  }
  delay(5000);
  if (DEBUG) {Serial.println("Succces initializing ESP-NOW");}
  if (LOG) {ft_log(5);}
  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  //esp_now_register_send_cb(OnDataSent);
  if (esp_now_set_pmk(espkey) != ESP_OK) { ;   //Set the primary master key.
     if (DEBUG) {Serial.println("Error in Key encrypt ESP-NOW");}
     if (LOG) {ft_log(4);}
    return 1;
  }
  // Register peer
  // esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, espnodo, 6);  //ESPNOW peer MAC address that is also the MAC address of station or softap
  memcpy(peerInfo.lmk, espkey, 16);                    // ESPNOW peer local master key that is used to encrypt data
  peerInfo.channel = 0;                            //Wi-Fi channel that peer uses to send/receive ESPNOW data. If the value is 0, use the current channel which station or softap is on. Otherwise, it must be set as the channel that station or softap is on.
  //peerInfo.ifidxn =                                  //Wi-Fi interface that peer uses to send/receive ESPNOW data ESP_IF_WIFI_STA o ESP_IF_WIFI_AP
  peerInfo.encrypt = false;                          // ESPNOW data that this peer sends/receives is encrypted or not
  
  // Add peer        
  if(esp_now_add_peer(&peerInfo) != ESP_OK){
    if (DEBUG) {Serial.println("Failed to add peer");}
    if (LOG) {ft_log(3);}
    return 1;
  }
  if (DEBUG) {Serial.println("Add peer Ok");}
  //if (DEBUGA) {Serial.println("Success to add peer");}
  if (LOG) {ft_log(3);}
  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);
  return 0;
}