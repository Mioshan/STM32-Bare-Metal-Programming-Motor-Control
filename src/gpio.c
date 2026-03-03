#include "gpio.h"

/*Register Bits*/
#define GPIOCEN         (1U<<2)
#define BTN_PIN         (1<<13)

void GPIO_Init(void)
{
    /*Enable GPIO Port C clock*/
    RCC->AHB1ENR |= GPIOCEN;
    
    /*Configure PA13 as an output pin*/
    GPIOC->MODER &= ~(1U<<26);
    GPIOC->MODER &= ~(1U<<27);
}

bool button_State(void)
{
    if (GPIOC->IDR & BTN_PIN) 
    {
        return false;
    }
    else 
    {
        return true;
    }
}