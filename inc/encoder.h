#ifndef ENCODER_H_
#define ENCODER_H_

#include "stm32f4xx.h"
#include <stdint.h>

void Encoder_init(void);
float Encoder_Read_Angle(void);

#endif