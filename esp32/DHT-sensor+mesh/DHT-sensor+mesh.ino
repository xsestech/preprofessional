#include "painlessMesh.h"
#include <Arduino.h>
#include "DHT.h"

#define DHTPIN 4 
#define DHTTYPE DHT11 
int SOILPIN =  34;

#define   MESH_PREFIX     "qwertyasdf"
#define   MESH_PASSWORD   "12asdfghjk3"
#define   MESH_PORT       5555


DHT dht(DHTPIN, DHTTYPE);
Scheduler userScheduler; 
painlessMesh  mesh; 

String name = "k1";

void sendMessage(); 
 
Task taskSendMessage( TASK_SECOND * 1, TASK_FOREVER, &sendMessage );
 
void sendMessage() {
//Отпровляемый json - файл обязательно включает в себя name и type

//json = ["from",  "type",  "temp",  "hum",  "soil"]
//type - это тип отправляемой информации (info | awake и может ещё что-то придумаю)
  int _soil = map((analogRead(SOILPIN)),3400,0,100,0);
 // Serial.print("FROM TASK: "); Serial.println(analogRead(SOILPIN));
  float _temp = dht.readTemperature();
  float _hum = dht.readHumidity();
  if (isnan(_temp) || isnan(_hum)) {
    _temp = 0;
    _hum = 0;
  }
  if (isnan(_soil)) {
    _soil = 0;
  }
  DynamicJsonDocument doc(2048);
  doc["from"] = name;
  doc["type"] = "info";
  doc["temp"] = _temp;
  doc["hum"] = _hum;
  doc["soil"] = _soil;
  String msg;
  serializeJson(doc, msg);
  mesh.sendBroadcast( msg );
}
 
void receivedCallback( uint32_t from, String &msg ) {
  String json;
  DynamicJsonDocument doc(2048);
  json = msg.c_str();
  DeserializationError error = deserializeJson(doc, json);
  if (error)
  {
    Serial.print("Json Error");
  }
  if(doc["type"] == "info"){

    float _temp = doc["temp"];
    float _hum = doc["hum"];
    int _soil = doc["soil"];
    Serial.print("temp: ");    Serial.print(_temp);
    Serial.print("  hum: ");   Serial.println(_hum);
    Serial.print ("soil: ");  Serial.println(_soil);
  }
  if(doc["type"] == "awake"){
    String _from = doc["from"];
    Serial.print(_from);   Serial.println(" awake");
  }
}

void newConnectionCallback(uint32_t nodeId) {
  DynamicJsonDocument doc(1024);
  doc["from"] = name;
  doc["type"] = "awake";
  String msg;
  serializeJson(doc, msg);
  mesh.sendBroadcast( msg );
}

void changedConnectionCallback() {
  Serial.printf("Changed connections\n");
}
void nodeTimeAdjustedCallback(int32_t offset) {
    Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(),offset);
}
void setup() {
  Serial.begin(115200);
  dht.begin();
  //mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // выбираем типы
  mesh.setDebugMsgTypes( ERROR | STARTUP);  
 
  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
 
  userScheduler.addTask( taskSendMessage );
  taskSendMessage.enable();
  

  pinMode(2, OUTPUT);
  pinMode(34, INPUT);
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