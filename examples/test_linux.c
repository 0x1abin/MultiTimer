#include <stdio.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>
#include <stdarg.h>
#include <string.h>

#include "../multi_timer.h"

#define LOG_TIMESTAMP_FORMAT    "[%Y-%m-%d %H:%M:%S]"
#define LOG_TIMESTAMP_LEN       21
#define MS_TIMESTAMP_LEN        5

int32_t printf_timestamp(const char *msg, ...)
{
    char content[1024] = {0};
    time_t  time_write;
    struct tm tm_Log;
    struct timeval t; 
    uint32_t len = 0;
    va_list vl_list;

    time_write = time(NULL);
    localtime_r(&time_write, &tm_Log);
    strftime((char *)&content[len], sizeof(content) - len - 1, LOG_TIMESTAMP_FORMAT, &tm_Log);
    len = strlen(&content[len]);

    /* .msec] */
    gettimeofday(&t, NULL);
    len--; // min a ']' char           
    snprintf((char*)&content[len], sizeof(content) - len, ".%03d]", (int)(t.tv_usec / 1000));        
    len += MS_TIMESTAMP_LEN; 

    va_start(vl_list, msg);
    vsnprintf((char *)&content[len], sizeof(content) - len - 1, (const char *)msg, vl_list);
    va_end(vl_list);

    printf("%s", content);

    return 0;
}

void signalHandler(int signo)
{
    switch(signo)
    {
        case SIGALRM:
            timer_ticks();
            //printf_timestamp("Caught the SIGALRM signal every 1ms !\n");
            break;
    }
}

struct Timer timer1;
struct Timer timer2;

void timer1_callback(void *arg)
{
    printf_timestamp("timer1 timeout! arg: %p\r\n", arg);
}

void timer2_callback(void *arg)
{
    printf_timestamp("timer2 timeout! arg: %p\r\n", arg);
}

int main(void)
{
    printf_timestamp("%s start ...\r\n", __func__);

    signal(SIGALRM, signalHandler);

    struct itimerval new_value, old_value;
    new_value.it_value.tv_sec     = 0; // should be 0x00 !!!
    new_value.it_value.tv_usec    = 1; // non-zero is OK !!!
    new_value.it_interval.tv_sec  = 0;
    new_value.it_interval.tv_usec = 1000 * CFG_TIMER_1_TICK_N_MS; // 1ms
    setitimer(ITIMER_REAL, &new_value, &old_value);

    timer_init(&timer1, timer1_callback, 4000, 1000, &timer1);  // start timer after 4s
    timer_start(&timer1);

    timer_init(&timer2, timer2_callback, 0, 5000, &timer2); // start timer right now
    timer_start(&timer2);

    while(1) {
        /* Maybe some sleep time is needed to avoid running CPU all the time. */
        timer_loop();
    }

    return 0;
}
