#include <wiringPi.h>
#include <softPwm.h>
#include <stdlib.h>
#include <stdio.h>

void ledPwmControl(int gpio);
void rgb_init(int red_pin, int green_pin);