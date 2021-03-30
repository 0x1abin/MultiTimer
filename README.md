# MultiTimer

## 简介
MultiTimer 是一个软件定时器扩展模块，可无限扩展你所需的定时器任务，取代传统的标志位判断方式， 更优雅更便捷地管理程序的时间触发时序。

## 使用方法
1.在multi_timer.h中配置定时器tick时钟频率（即1个tick代表N毫秒钟）

```
/*
It means 1 tick for 1ms. 
Your can configurate for your tick time such as 5ms/10ms and so on.
*/
#define CFG_TIMER_1_TICK_N_MS   1
```

2.先申请一个定时器管理handle

```
struct Timer timer1;
```

3.初始化定时器对象，注册定时器回调处理函数，设置延迟启动时间（ms），循环定时触发时间

```
timer_init(struct Timer* handle, void(*timeout_cb)(void *arg), uint32_t timeout, uint32_t repeat);
```

4.启动定时器

```
timer_start(&timer1);
```

5.设置1ms的硬件定时器循环调用 *timer_ticks()* 以提供时间基准

```
void HAL_SYSTICK_Callback(void)
{
    timer_ticks();
}
```

6.在主循环调用定时器后台处理函数

```
int main() 
{
    ...
    while(1) {
        ...
        timer_loop();
    }
}
```

## 功能限制
1.定时器的时钟频率直接影响定时器的精确度，尽可能采用1ms/5ms/10ms这几个精度较高的tick;

2.定义应用定时器时，超时时间应合理设置，不应过大或过小，否则可能导致定时器超时时间不精准；

3.定时器的回调函数内不应执行耗时操作，否则可能因占用过长的时间，导致其他定时器无法正常超时；一般来说，若干个tick的时间是可以接受的；

4.由于定时器的回调函数是在timer_loop内执行的，需要注意栈空间的使用不能过大，否则可能会导致栈溢出。

## Examples

见example目录下的测试代码，main.c为普通平台测试demo，test_linux.c为linux平台的测试demo。

```
#include "multi_timer.h"

struct Timer timer1;
struct Timer timer2;

void timer1_callback(void *arg)
{
    printf("timer1 timeout!\r\n");
}

void timer2_callback(void *arg)
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
```

