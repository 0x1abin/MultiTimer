#include "multi_timer.h"

struct Timer timer1;
struct Timer timer2;

void timer1_callback(void *arg)
{
    printf("timer1 timeout! arg: %p\r\n", arg);
}

void timer2_callback(void *arg)
{
    printf("timer2 timeout! arg: %p\r\n", arg);
}

int main(void)
{
	timer_init(&timer1, timer1_callback, 1000, 1000, NULL); //1s loop
	timer_start(&timer1);
	
	timer_init(&timer2, timer2_callback, 50, 0, NULL); //50ms delay
	timer_start(&timer2);
	
	while(1) {	    
	    timer_loop();
	}

    return 0;
}

void HAL_SYSTICK_Callback(void)
{
    timer_ticks(); //1ms ticks
}