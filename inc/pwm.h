#ifndef PWM_H_
#define PWM_H_

#include "stm32f4xx.h"

void PWM_Init(void);
void set_duty_cycle(uint8_t duty);

#endif