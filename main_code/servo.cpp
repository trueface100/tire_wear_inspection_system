#include "servo.h"

void servo_init(int servo_pin){
    pinMode(servo_pin, PWM_OUTPUT);
    pwmSetMode(PWM_MODE_MS);
    pwmSetRange(2000);
    pwmSetClock(192);

}

void rotateServo(int servo_pin, int angle){
    int pulseWidth = (angle * (250-50)/180)+50;
    pwmWrite(servo_pin, pulseWidth);
}