#ifndef GPIO_H_
#define GPIO_H_

#include "stm32f4xx.h"
#include <stdbool.h>

void GPIO_Init(void);
bool button_State(void);

#endif 