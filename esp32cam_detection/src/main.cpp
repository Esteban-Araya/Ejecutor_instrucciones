#include <Arduino.h>
#include <Base64.h>
#include "Motor_esp.h"  // Disable brownour problems
#include "Cam.h"  // Disable brownour problems
#include "WiFi.h"
#include <BluetoothSerial.h>
#include <ArduinoJson.h>
#include <vector>
#include <unordered_map>

const size_t capacity = 2 * JSON_OBJECT_SIZE(10);

using namespace std;

#define MOTOR_DER_F 14
#define MOTOR_DER_A 15
#define MOTOR_IZQ_F 12
#define MOTOR_IZQ_A 13

#define CANAL_DER_F 8
#define CANAL_DER_A 0
#define CANAL_IZQ_F 1
#define CANAL_IZQ_A 2
#define CANAL_LED 3

#define RESOLUCION 8
#define FRECUENCIA 1000

#define VELOCIDAD 200

bool start = false;
bool execute = false; 
int intensity_led = 5;
int time_turn = 500;

String url = "http://";
String wifi = "http://";
String password = "http://";

BluetoothSerial BT;

enum Directions {RIGHT,UP,LEFT,DOWN};
enum Robot {PHOTO,ARROWS,RUN,STOP};

int states = STOP;
vector<int> steps;

Cam myCamera(CANAL_LED);

Motor_esp myMotors(
  CANAL_DER_F,
  CANAL_DER_A,
  CANAL_IZQ_F,
  CANAL_IZQ_A,
  MOTOR_DER_F,
  MOTOR_DER_A,
  MOTOR_IZQ_F,
  MOTOR_IZQ_A,
  FRECUENCIA,
  RESOLUCION
);

void move_up(){
  BT.println("up");
  myMotors.advance(VELOCIDAD,500);
  myMotors.stop();
  delay(800);
}

void move_down(){
  BT.println("down");
  myMotors.back(VELOCIDAD,500);
  myMotors.stop();
  delay(800);
}
void move_left(){
  BT.println("left");
  myMotors.turn_left(VELOCIDAD,time_turn);
  myMotors.stop();
  delay(800);
  myMotors.advance(VELOCIDAD,500);
  myMotors.stop();
  delay(800);
}

void move_right(){
  BT.println("right");
  myMotors.turn_right(VELOCIDAD,time_turn);
  myMotors.stop();
  delay(800);
  myMotors.advance(VELOCIDAD,500);
  myMotors.stop();
  delay(800);
}

unordered_map<int, function<void()>> execute_steps = {
  {RIGHT,move_right},
  {UP,move_up},
  {LEFT,move_left},
  {DOWN,move_down}
};


int state_bluetooth(int state){
  switch (state)
  {
  case 1:{
    bool old_url= true;
    while (old_url)
    {
      if(BT.available()){
        String new_url = BT.readString();
        new_url.trim();
        url = new_url;
        BT.println("the url is: '" + url + "'");
        old_url = false;
        }
      }
    
    
    break;
  }
  case 2:{
    bool old_wifi= true;
    while (old_wifi)
    {
      if(BT.available()){
        String new_wifi = BT.readString();
        new_wifi.trim();
        wifi = new_wifi;
        BT.println("the wifi is: '" + wifi + "'");
        old_wifi = false;
        }
      }
    
    
    break;
  }
  case 3:{
    bool old_password= true;
    while (old_password)
    {
      if(BT.available()){
        String new_password = BT.readString();
        new_password.trim();
        password = new_password;
        BT.println("the password is: '" + password + "'");
        old_password = false;
        }
      }
    
    
    break;
  }
  default:
    
    break;
  }

  return 0;
}
void initBluetooth(){
  BT.begin("Flecha");
  while (!BT.connected(1000))
  {
    
  }
  
  bool exit = false;
  int state = 0;
  do{
    if(BT.available()){
      String in = BT.readString();
      in.trim();
      BT.println(in);
      if(in == "u"){
        BT.println("change the url");
        state = 1;
      } 
      else if(in == "w"){
        BT.println("change the wifi");
        state = 2;
      } 
      else if(in == "p"){
        BT.println("change the password");
        state = 2;
      } 
      else if(in == "e"){
        exit = true;
      }
      state = state_bluetooth(state);
    }
  }while (!exit);
  
  
}
void initWifi(){
  const char* ssid = "Estudiantes";
  const char* password = "eucar_2018";
  
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  Serial.print("Conectando...");
  while (WiFi.status() != WL_CONNECTED) { //Check for the connection
    delay(500);
    Serial.print(".");
  }

  Serial.print("Conectado con éxito, mi IP es: ");
  Serial.println(WiFi.localIP());
}

String deserialize(String json, String atrribute){

    DynamicJsonDocument doc(capacity);

  // Deserializar el JSON
    DeserializationError error = deserializeJson(doc, json);

 if (error) {
    Serial.print("Error al deserializar JSON: ");
    Serial.println(error.c_str());
    return error.c_str(); 
  }
    const String data = doc[atrribute];
    return data;
}




void state_robot(int state){

  switch (state)
  {
  case STOP:
    if(start){
      if(execute){
        states = RUN;
        BT.println("run");
        execute = false;
      }
      else{
        states = ARROWS;
        BT.println("arrows");
        execute = true;
      }
      start = false;
    }
  break;
  case PHOTO:
    myCamera.request(url, intensity_led);
    break;
  case ARROWS:{

    
    String direction = myCamera.request(url, intensity_led);

    direction = deserialize(direction,"direction");
    
    if(direction == "right"){
      steps.push_back(RIGHT);
    }    
    else if(direction == "left"){
      steps.push_back(LEFT);
    }
    else if(direction == "down"){
      steps.push_back(DOWN);
    }
    else if(direction == "up"){
      steps.push_back(UP);
    
    }
    else if(direction == "circle"){
      states = STOP; 
    
    }
    BT.print("direction: ");
    BT.println(direction);
  }
    
    break;
  case RUN:
  BT.println("----------------");
  for(int val : steps){
   
    execute_steps[val]();
  }
  states = STOP;
  break;
  default:
    break;
  }
}

void change_stats(){
  if (BT.available()){
    String in = BT.readString();
    in.trim();
    BT.println(in);

    if(in == "ip"){
      intensity_led +=1;
      BT.println(intensity_led);
    }
    else if(in == "im"){
      intensity_led -=1;
      BT.println(intensity_led);
    }
    else if(in == "photo"){
      states = PHOTO;
      BT.println("state is photo");
    }
    else if(in == "arrows"){
      states = ARROWS;
      BT.println("state is arrows");
    }
    else if(in == "steps"){
      
      for (int i = 0; i < steps.size(); i++) {
          BT.print(steps[i]);
          BT.print(" ");

     }
    }
    else if(in == "start"){
      start = true;
      
    }
  }
  
}

void setup() {
  // put your setup code here, to run once:
  initBluetooth();
  initWifi();
//  
myMotors.initMotor();
myCamera.initCam();
}

void loop() {
  // put your main code here, to run repeatedly:
  change_stats();
  state_robot(states);
  delay(1000);
}