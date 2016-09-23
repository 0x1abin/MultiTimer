#include "multi_timer.h"

struct Timer timer1;
struct Timer timer2;

void timer1_callback()
{
    printf("timer1 timeout!\r\n");
}

void timer2_callback()
{
    printf("timer2 timeout!\r\n");
}

int main()
{
	timer_init(&timer1, timer1_callback, 1000, 1000); //1s loop
	timer_start(&timer1);
	
	timer_init(&timer2, timer2_callback, 50, 0); //50ms delay
	timer_start(&timer2);
	
	while(1) {
	    
	    timer_loop();
	}
}

void HAL_SYSTICK_Callback(void)
{
    timer_ticks(); //1ms ticks
}