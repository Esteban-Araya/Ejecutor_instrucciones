#include "Motor_esp.h"
#include <Arduino.h>

enum Motor {RIGHT,LEFT};

Motor_esp::Motor_esp(
    int channel_right_f,
    int channel_right_b,
    int channel_left_f,
    int channel_left_b,
    int motor_right_f,
    int motor_right_b,
    int motor_left_f,
    int motor_left_b,
    int freq,
    int resolution){

        this->channel_right_b = channel_right_b;
        this->channel_left_f = channel_left_f;
        this->channel_left_b = channel_left_b;
        this->motor_right_f = motor_right_f;
        this->motor_right_b = motor_right_b;
        this->motor_left_f = motor_left_f;
        this->motor_left_b = motor_left_b;
        this->freq = freq;
        this->resolution = resolution;

        
}

Motor_esp::~Motor_esp()
{
}

void Motor_esp::initMotor(){

  ledcSetup(channel_right_f,freq,resolution);
  ledcSetup(channel_right_b,freq,resolution);
  ledcSetup(channel_left_f,freq,resolution);
  ledcSetup(channel_left_b,freq,resolution);

  ledcAttachPin(motor_right_f,channel_right_f);  
  ledcAttachPin(motor_right_b,channel_right_b);
  ledcAttachPin(motor_left_f,channel_left_f);  
  ledcAttachPin(motor_left_b,channel_left_b);
}

void Motor_esp::move_front(int side, int vel){


  if(side == RIGHT){
    ledcWrite(motor_right_f,vel);
    ledcWrite(motor_right_b,0);
  }
  else{
   ledcWrite(motor_left_f,vel);
   ledcWrite(motor_left_b,0);
  }
}
void Motor_esp::move_back(int side, int vel){


 if(side == RIGHT){
    ledcWrite(motor_right_f,0);
    ledcWrite(motor_right_b,vel);
  }
  else{
   ledcWrite(motor_left_f,0);
   ledcWrite(motor_left_b,vel);
  }
}

void Motor_esp::advance(int vel,int time = 0){
  move_front(RIGHT,vel);
  move_front(LEFT,vel);
  delay(time);

}


void Motor_esp::turn_right(int vel, int time = 0){
  move_front(LEFT,vel);
  move_back(RIGHT,vel);
  delay(time);
}

void Motor_esp::turn_left(int vel, int time = 0){
  move_front(RIGHT,vel);
  move_back(LEFT,vel);
  delay(time);
}

void Motor_esp::back(int vel,int time = 0){
  move_back(LEFT,vel);
  move_back(RIGHT,vel);
  delay(time);
}

void Motor_esp::stop(){
  move_back(LEFT,0);
  move_back(RIGHT,0);
}

