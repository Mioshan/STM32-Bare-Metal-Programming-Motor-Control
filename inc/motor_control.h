#ifndef MOTOR_CONTROL_H_
#define MOTOR_CONTROL_H_

#include "stm32f4xx.h"
#include <stdint.h>

void Motor_init(void);
void Encoder_start(void);
void motor_PWM(uint8_t duty);
float Encoder_Read(void);
#endif