#include "motor_control.h"

/*Motor Register bits*/
#define GPIOBEN     (1U << 1)
#define TIM2EN      (1U << 0)
#define AF1         (1U << 12)
#define OC2M        (6U << 12)
#define OC2PE       (1U << 11)
#define CC2P        (1U << 5)
#define CC2E        (1U << 4)
#define ARPE        (1U << 7)
#define UG          (1U << 0)
#define CEN         (1U << 0)

/*Encoder Register bits*/
#define GPIOAEN     (1U << 0)
#define TIM3EN      (1U << 1)
#define AFR6        (2U << 24)
#define AFR7        (2U << 28)
#define SMS         (3U << 0)
#define CC1P        (1U << 1)
#define CC1NP       (1U << 3)
#define CC2P        (1U << 5)
#define CC2NP       (1U << 7)
#define CC1E        (1U << 0)
#define CC2E        (1U << 4)

/*Definitions*/
#define PRESCALER   100
#define RELOAD      1000
#define ENC_ARR     0xFFFF
#define ENC_PSC     0 
#define CPR         3200.0f

void Motor_init(void)
{
    /*Enable clock for GPIO Port B*/
    RCC->AHB1ENR |= GPIOBEN;
    RCC->APB1ENR |= TIM2EN;

    /*Configure PB3 to Timer 2, Channel 2*/
    GPIOB->MODER &= ~(1U << 6);
    GPIOB->MODER |= (1U << 7);
    
    GPIOB->AFR[0] &= ~(0xF << 12);
    GPIOB->AFR[0] |= AF1;

    /*Configure PSC and ARR register of Timer 2, Channel 2*/
    TIM2->PSC = PRESCALER;
    TIM2->ARR = RELOAD;

    /*Set to PWM Mode 1*/
    TIM2->CCMR1 &= ~(7U << 12);
    TIM2->CCMR1 |= OC2M;

    /*Enable output compare 2 and auto-reload preload enable*/
    TIM2->CCMR1 |= OC2PE;
    TIM2->CR1 |= ARPE;

    /*Enable Capture/Compare output and configure polarity*/
    TIM2->CCER &= ~CC2P;
    TIM2->CCER |= CC2E;

    /*Enable event generation*/
    TIM2->EGR |= UG;

    /*Reset counter and start clock*/
    TIM2->CNT = 0;
    TIM2->CCR2 = 0;
    TIM2->CR1 |= CEN;

}

void Encoder_start(void)
{
    /*Enable clock for GPIOA*/
    RCC->AHB1ENR |= GPIOAEN;
    RCC->APB1ENR |= TIM3EN;

    /*Configure PA6 and PA7 for Alternative Function (10)*/
    GPIOA->MODER &= ~(3U << 12);
    GPIOA->MODER |=  (2U << 12);

    GPIOA->MODER &= ~(3U << 14);
    GPIOA->MODER |=  (2U << 14);

    /*Configure Alternate Function AF2 (TIM3) for PA6 and PA7*/
    GPIOA->AFR[0] &= ~(0xF << 24);
    GPIOA->AFR[0] |=  AFR6;

    GPIOA->AFR[0] &= ~(0xF << 28);
    GPIOA->AFR[0] |=  AFR7;

    /*Configure ARR and PSC for Encoder reading*/
    TIM3->PSC = ENC_PSC;
    TIM3->ARR = ENC_ARR;

    /*Configure CCMR1: CC1S = 01, CC2S = 01 (map TI1 and TI2)*/
    TIM3->CCMR1 &= ~((3U << 0) | (3U << 8));
    TIM3->CCMR1 |=  (1U << 0);
    TIM3->CCMR1 |=  (1U << 8);

    TIM3->CCMR1 |= (0x3U << 4) | (0x3U << 12);   // simple filter

    /*Configure Timer 3 to Encoder Interface Mode*/
    TIM3->SMCR &= ~(7U << 0);
    TIM3->SMCR |= SMS;

    /*Configure bit polarity*/
    TIM3->CCER &= ~CC1P;
    TIM3->CCER &= ~CC1NP;

    TIM3->CCER &= ~CC2P;
    TIM3->CCER &= ~CC2NP;

    /*Enable Capture Registers for Channel 1 and 2*/
    TIM3->CCER |= CC1E;
    TIM3->CCER |= CC2E;
    
    /*Enable Timer 3*/
    TIM3->EGR |= UG;
    TIM3->CR1 |= CEN;

}

void motor_PWM(uint8_t duty)
{
    if (duty > 100) duty = 100;

    // Scale to PERIOD - 1 max 
    TIM2->CCR2 = ((uint32_t)(RELOAD - 1) * duty) / 100U;
}

float Encoder_Read(void)
{
    int16_t counts = (int16_t)TIM3->CNT;
    return counts * (360.0f / CPR);
}

