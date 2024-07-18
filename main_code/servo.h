#include <wiringPi.h>
#include <softPwm.h>
#include <iostream>

void servo_init(int servo_pin);
void rotateServo(int servo_pin, int angle);