#ifndef Cam_h
#define Cam_h

#if ARDUINO >100
#include <Arduino.h>
#else
#include  <WProgram.h>
#endif

#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22


class Cam
{
private:
    String get_enocde_image(int intensidad_led);
    int canal_led;
public:
    Cam(int canal_led);
    ~Cam();
    void initCam();
    String request(String url, int intensidad_led);
    
};

#endif // Cam_H
