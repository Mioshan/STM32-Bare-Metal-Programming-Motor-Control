#include "encoder.h"

#define GPIOAEN     (1U << 0)
#define TIM3EN      (1U << 1)
#define CC1E        (1U << 0)
#define CC2E        (1U << 4)
#define ARPE        (1U << 7)
#define CEN         (1U << 0)

#define PRESCALER   0
#define RELOAD      0xFFFF     /* TIM3 is 16-bit */
#define COUNTS_PER_REV 3210.0f

void Encoder_init(void) 
{
    /*Enable clock for GPIO Port A and TIM3*/
    RCC->AHB1ENR |= GPIOAEN;
    RCC->APB1ENR |= TIM3EN;

    /*Configure PA6 and PA7 as Alternate Function mode (10)*/
    GPIOA->MODER &= ~(3U << 12);
    GPIOA->MODER |=  (2U << 12);

    GPIOA->MODER &= ~(3U << 14);
    GPIOA->MODER |=  (2U << 14);

    /*Configure Alternate Function AF2 (TIM3) for PA6 and PA7*/
    GPIOA->AFR[0] &= ~(0xF << 24);
    GPIOA->AFR[0] |=  (2U << 24);

    GPIOA->AFR[0] &= ~(0xF << 28);
    GPIOA->AFR[0] |=  (2U << 28);

    /*Configure Timer 3 Prescaler and Auto-Reload Registers*/
    TIM3->PSC = PRESCALER;
    TIM3->ARR = RELOAD;

    /*Configure CCMR1: CC1S = 01, CC2S = 01 (map TI1 and TI2)*/
    TIM3->CCMR1 &= ~((3U << 0) | (3U << 8));
    TIM3->CCMR1 |=  (1U << 0);
    TIM3->CCMR1 |=  (1U << 8);

    TIM3->CCMR1 |= (0x3U << 4) | (0x3U << 12);   // simple filter

    /*Set Encoder Mode 3 (SMS = 011)*/
    TIM3->SMCR &= ~(7U << 0);
    TIM3->SMCR |=  (1U << 0);
    TIM3->SMCR |=  (1U << 1);

    /*Configure Capture/Compare Register polarity for channels 1 and 2 (non-inverted)*/
    TIM3->CCER &= ~(1U << 1);
    TIM3->CCER &= ~(1U << 3);
    TIM3->CCER &= ~(1U << 5);
    TIM3->CCER &= ~(1U << 7);

    /*Enable Capture/Compare Registers*/
    TIM3->CCER |= CC1E;
    TIM3->CCER |= CC2E;

    /*Enable Timer*/
    TIM3->CR1 |= ARPE;
    TIM3->CR1 |= CEN;
}

float Encoder_Read_Angle(void)
{
    return (360.0f / COUNTS_PER_REV)* (float)TIM3->CNT;
}

