#include "pwm.h"

int main(void)
{
    PWM_Init();

    while (1)
    {
        // Optional fade loop
        for (int d = 0; d <= 100; d += 1) {
            set_duty_cycle(d);
            for (volatile uint32_t i = 0; i < 20000; i++);
        }
        for (int d = 100; d >= 0; d -= 1) {
            set_duty_cycle(d);
            for (volatile uint32_t i = 0; i < 20000; i++);
        }
    }
}