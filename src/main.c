#include "encoder.h"

int main(void)
{
    Encoder_init();
    float s;

    while(1)
    {
        s = Encoder_Read_Angle();
        int16_t angle = (int16_t)s;
    }
}
