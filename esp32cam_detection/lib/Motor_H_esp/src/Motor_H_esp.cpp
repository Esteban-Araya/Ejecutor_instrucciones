#include "Motor_H_esp.h"
#include <Arduino.h>

enum Motor {RIGHT,LEFT};

Motor_H_esp::Motor_H_esp(
    int channel_right_f,
    int channel_right_b,
    int channel_left_f,
    int channel_left_b,
    int motor_right_f,
    int motor_right_b,
    int motor_left_f,
    int motor_left_b,
    int frequency,
    int resolution){

        this->channel_right_f = channel_right_f;
        this->channel_right_b = channel_right_b;
        this->channel_left_f = channel_left_f;
        this->channel_left_b = channel_left_b;
        this->motor_right_f = motor_right_f;
        this->motor_right_b = motor_right_b;
        this->motor_left_f = motor_left_f;
        this->motor_left_b = motor_left_b;
        this->frequency = frequency;
        this->resolution = resolution;

        
}

Motor_H_esp::~Motor_H_esp()
{
}

void Motor_H_esp::initMotor(){

  ledcSetup(channel_right_f,frequency,resolution);
  ledcSetup(channel_right_b,frequency,resolution);
  ledcSetup(channel_left_f,frequency,resolution);
  ledcSetup(channel_left_b,frequency,resolution);

  ledcAttachPin(motor_right_f,channel_right_f);  
  ledcAttachPin(motor_right_b,channel_right_b);
  ledcAttachPin(motor_left_f,channel_left_f);  
  ledcAttachPin(motor_left_b,channel_left_b);
}

void Motor_H_esp::move_front(int side, int vel){

  
  if(side == RIGHT){
    ledcWrite(channel_right_f,vel);
    ledcWrite(channel_right_b,0);
  }
  else{
   ledcWrite(channel_left_f,vel);
   ledcWrite(channel_left_b,0);
  }
}
void Motor_H_esp::move_back(int side, int vel){


 if(side == RIGHT){
    ledcWrite(channel_right_f,0);
    ledcWrite(channel_right_b,vel);
  }
  else{
   ledcWrite(channel_left_f,0);
   ledcWrite(channel_left_b,vel);
  }
}

void Motor_H_esp::advance(int vel, int time=0){
  move_front(RIGHT,vel);
  move_front(LEFT,vel);
  delay(time);
}


void Motor_H_esp::turn_right(int vel, int time = 0){
  move_front(LEFT,vel);
  move_back(RIGHT,vel);
  delay(time);
}

void Motor_H_esp::turn_left(int vel, int time = 0){
  move_front(RIGHT,vel);
  move_back(LEFT,vel);
  delay(time);
}

void Motor_H_esp::back(int vel, int time=0){
  move_back(LEFT,vel);
  move_back(RIGHT,vel);
  delay(time);
}

void Motor_H_esp::stop(){
  move_front(LEFT,0);
  move_back(RIGHT,0);

}
void Motor_H_esp::PID(int speed)
{
    
    if (speed < 0)
    {
        
        speed = min(speed, -255);
        
        back(speed);
    }
    else
    {
       
        speed = min(speed, 255);
        advance(speed);
    }
    
   
    
}
