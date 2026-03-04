#include "pwm.h"
#include "gpio.h"

bool btn_state;

int main(void)
{
    PWM_Init();
    GPIO_Init();

    set_duty_cycle(0);

    while (1)
    {
        btn_state = button_State();

        if (btn_state)
        {
            set_duty_cycle(80);
        }
        else 
        {
            set_duty_cycle(0);
        }
    }
}
