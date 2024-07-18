#include "rgb.h"

void rgb_init(int red_pin, int green_pin){
    pinMode(red_pin, OUTPUT);
    pinMode(green_pin,OUTPUT);
    digitalWrite(red_pin,0);
    digitalWrite(green_pin,0);
}
void ledPwmControl(int gpio)
{
    
    softPwmCreate(gpio, 0, 255);
    
    for (int i=0; i<255; i++){
        softPwmWrite(gpio, i&255);
        delay(1);
    }
    softPwmWrite(gpio,0);
}