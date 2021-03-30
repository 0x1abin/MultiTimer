# MultiTimer

## 简介
MultiTimer 是一个软件定时器扩展模块，可无限扩展你所需的定时器任务，取代传统的标志位判断方式， 更优雅更便捷地管理程序的时间触发时序。

## 使用方法
1.配置定时器tick时钟频率（1个tick代表N毫秒钟）

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

## Examples

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

