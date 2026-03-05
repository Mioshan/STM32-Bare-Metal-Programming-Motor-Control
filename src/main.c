#include "motor_control.h"

int main(void)
{
    Encoder_start();
    float s;
    int32_t sush;
    while(1)
    {
        s = Encoder_Read();

        sush = Encoder_Angle_Degrees(s);
        sush++;
    }
}
