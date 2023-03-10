#include "painlessMesh.h"
#include <Arduino.h>
#include <DFRobot_DHT11.h>

#define DHT11_PIN 4

#define   MESH_PREFIX     "qwertyasdf"
#define   MESH_PASSWORD   "12asdfghjk3"
#define   MESH_PORT       5555

DFRobot_DHT11 DHT;
Scheduler userScheduler; 
painlessMesh  mesh; 

void sendMessage() ; 
 
Task taskSendMessage( TASK_SECOND * 2 , TASK_FOREVER, &sendMessage );
 
void sendMessage() {
  DHT.read(DHT11_PIN);
  int temp = DHT.temperature;
  int hum = DHT.humidity;
  DynamicJsonDocument doc(1024);
  doc["temp1"] = temp;
  doc["hum1"] = hum;
  String msg;
  serializeJson(doc, msg);
  mesh.sendBroadcast( msg );
}
 

void receivedCallback( uint32_t from, String &msg ) {
  String json;
  DynamicJsonDocument doc(1024);
  json = msg.c_str();
  DeserializationError error = deserializeJson(doc, json);
  if (error)
  {
    Serial.print("Json Error");
  }
  int temp = doc["temp1"];
  int hum = doc["hum1"];
  Serial.print("temp: ");
  Serial.print(temp);
  Serial.print("  hum ");
  Serial.println(hum);
}
void newConnectionCallback(uint32_t nodeId) {
    Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}
void changedConnectionCallback() {
  Serial.printf("Changed connections\n");
}
void nodeTimeAdjustedCallback(int32_t offset) {
    Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(),offset);
}
void setup() {
  Serial.begin(115200);
//mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // выбираем типы
  mesh.setDebugMsgTypes( ERROR | STARTUP | CONNECTION | MESH_STATUS);  
 
  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
 
  userScheduler.addTask( taskSendMessage );
  taskSendMessage.enable();

  pinMode(2, OUTPUT);
}
 
void loop() {
  mesh.update();
  checkConection();
}

void checkConection()
  {
    if (mesh.getNodeList().size() > 0)
    {
      digitalWrite(2, HIGH);
    }
    else
    {
      digitalWrite(2, LOW);
    }
  }