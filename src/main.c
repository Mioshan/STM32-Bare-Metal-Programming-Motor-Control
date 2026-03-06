#include "motor_control.h"

float s;
    
int main(void)
{
    Motor_init();
    Encoder_start();

    TIM3->CNT = 0;

    while(1)
    {
        while (s < 180)
        {
            s = Encoder_Read();
            motor_PWM(10);
        }
        TIM3->CNT = 0;
        s = 0;
        motor_PWM(0);
        for (volatile uint32_t i = 0; i < 1000000; i++);
        
    }
}
