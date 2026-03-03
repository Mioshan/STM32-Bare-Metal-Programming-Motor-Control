#include "pwm.h"

// Clock enable bits
#define GPIOBEN    (1U << 1)    // RCC->AHB1ENR GPIOB enable
#define TIM3EN     (1U << 1)    // RCC->APB1ENR TIM3 enable

// Timer bits
#define UG         (1U << 0)    // EGR: Update Generation
#define OC1PE      (1U << 3)    // CCMR1: OC1 preload enable
#define ARPE       (1U << 7)    // CR1: ARR preload enable
#define CC1P       (1U << 1)    // CCER: Capture/Compare Polarity
#define CC1E       (1U << 0)    // CCER: Capture/Compare Polarity
#define CEN        (1U << 0)    // CR1: Counter enable

// PWM settings – ~160 Hz at default 16 MHz HSI bare-metal clock
#define PERIOD     1000         // ARR = 999 → 1000 steps
#define PRESCALER  100          // PSC = 99 → divider = 100

void PWM_Init(void)
{
    /*Enable clocks*/
    RCC->AHB1ENR |= GPIOBEN;     // GPIOB clock
    RCC->APB1ENR |= TIM3EN;      // TIM3 clock

    /*Configure PB4 as Alternate Function mode for TIM3_CH1*/
    GPIOB->MODER   &= ~(1U << 8);      // Clear MODER[9:8] for PB4
    GPIOB->MODER   |=  (1U << 9);      // 10 = AF mode

    GPIOB->AFR[0] &= ~(1U<<16);
    GPIOB->AFR[0] |= (1U<<17);
    GPIOB->AFR[0] &= ~(1U<<18);
    GPIOB->AFR[0] &= ~(1U<<19);

    /*Timer base configuration*/
    TIM3->PSC = PRESCALER - 1;          // Prescaler
    TIM3->ARR = PERIOD - 1;             // Auto-reload (period)

    /*PWM mode 1 on Channel 1*/
    TIM3->CCMR1 &= ~(1U<<4);			// 110 = PWM mode 1
    TIM3->CCMR1 |= (1U<<5);
    TIM3->CCMR1 |= (1U<<6);

    TIM3->CCMR1 |= OC1PE;               // Preload enable for CCR1

    TIM3->CR1   |= ARPE;                // Preload enable for ARR

    /*Output polarity and enable – ACTIVE HIGH*/
    TIM3->CCER &= ~CC1P;           // CC1P = 0 → active high
    TIM3->CCER |= CC1E;           // CC1E = 1 → output enabled on PB4

    /*Enable CC generation*/
    TIM3->EGR |= UG;

    /*Reset counter and start timer*/
    TIM3->CCR1 = 0;
    TIM3->CNT = 0;
    TIM3->CR1 |= CEN;
}

void set_duty_cycle(uint8_t duty)
{
    if (duty > 100) duty = 100;

    // Scale to PERIOD - 1 max 
    TIM3->CCR1 = ((uint32_t)(PERIOD - 1) * duty) / 100U;
}