#include <stdio.h>
#include <signal.h>
#include <sys/time.h>
#include "../multi_timer.h"

void signalHandler(int signo)
{
    switch(signo)
    {
        case SIGALRM:
            timer_ticks();
            //printf("Caught the SIGALRM signal!\n");
            break;
    }
}

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

int main(void)
{
    signal(SIGALRM, signalHandler);

    struct itimerval new_value, old_value;
    new_value.it_value.tv_sec     = 1;
    new_value.it_value.tv_usec    = 0;
    new_value.it_interval.tv_sec  = 0;
    new_value.it_interval.tv_usec = 1000;
    setitimer(ITIMER_REAL, &new_value, &old_value);

    timer_init(&timer1, timer1_callback, 4000, 1000);  // start timer after 4s
    timer_start(&timer1);

    timer_init(&timer2, timer2_callback, 0, 2000);
    timer_start(&timer2);

    while(1)
    {
        timer_loop();
    }
    return 0;
}
