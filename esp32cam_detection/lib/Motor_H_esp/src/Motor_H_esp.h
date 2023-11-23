#ifndef Motor_H_esp_H
#define Motor_H_esp_H

class Motor_H_esp
{
private:
    int _currentSpeed;
    int frequency;
    int resolution;
    int channel_right_f;
    int channel_right_b;
    int channel_left_f;
    int channel_left_b;
    int motor_right_f;
    int motor_right_b;
    int motor_left_f;    
    int motor_left_b;        
    
    void move_front(int side, int vel);
    void move_back(int side, int vel);
public:
    Motor_H_esp(
        int channel_right_f,
        int channel_right_b,
        int channel_left_f,
        int channel_left_b,
        int motor_right_f,
        int motor_right_b,
        int motor_left_f,
        int motor_left_b,
        int frequency,
        int resolution
        );
    ~Motor_H_esp();
    void initMotor();
    void advance(int vel,int time);
    void turn_right(int vel,int time);
    void turn_left(int vel,int time);
    void back(int vel,int time);
    void PID(int speed);
    void stop();
};

#endif // Motor_H_esp_H