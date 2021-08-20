# MultiTimer

## 简介
MultiTimer 是一个软件定时器扩展模块，可无限扩展你所需的定时器任务，取代传统的标志位判断方式， 更优雅更便捷地管理程序的时间触发时序。

## 使用方法
1. 配置系统时间基准接口，安装定时器驱动；

```c
uint32_t PlatformTicksGetFunc(void)
{
    /* Platform implementation */
}

MultiTimerInstall(PlatformTicksGetFunc);
```

2. 实例化一个定时器对象；

```c
MultiTimer timer1;
```

3. 设置定时时间，超时回调处理函数， 用户上下指针，启动定时器；

```c
int MultiTimerStart(&timer1, uint32_t timing, MultiTimerCallback_t callback, void* userData);
```

4. 在主循环调用定时器后台处理函数

```c
int main(int argc, char *argv[])
{
    ...
    while (1) {
        ...
        MultiTimerYield();
    }
}
```

## 功能限制
1.定时器的时钟频率直接影响定时器的精确度，尽可能采用1ms/5ms/10ms这几个精度较高的tick;

2.定义应用定时器时，最大定时不可以超过 UINT32_MAX / 4；

3.定时器的回调函数内不应执行耗时操作，否则可能因占用过长的时间，导致其他定时器无法正常超时；

4.由于定时器的回调函数是在 MultiTimerYield 内执行的，需要注意栈空间的使用不能过大，否则可能会导致栈溢出。

## Examples

见example目录下的测试代码，main.c为普通平台测试demo，test_linux.c为linux平台的测试demo。

```c
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include "MultiTimer.h"

MultiTimer timer1;
MultiTimer timer2;
MultiTimer timer3;

uint32_t PlatformTicksGetFunc(void)
{
    struct timespec current_time;
    clock_gettime(CLOCK_MONOTONIC, &current_time);
    return (uint32_t)((current_time.tv_sec * 1000) + (current_time.tv_nsec / 1000000));
}

void exampleTimer1Callback(MultiTimer* timer, void *userData)
{
    printf("exampleTimer1Callback-> %s.\r\n", (char*)userData);
    MultiTimerStart(timer, 1000, exampleTimer1Callback, userData);
}

void exampleTimer2Callback(MultiTimer* timer, void *userData)
{
    printf("exampleTimer2Callback-> %s.\r\n", (char*)userData);
}

void exampleTimer3Callback(MultiTimer* timer, void *userData)
{
    printf("exampleTimer3Callback-> %s.\r\n", (char*)userData);
    MultiTimerStart(timer, 4567, exampleTimer3Callback, userData);
}

int main(int argc, char *argv[])
{
    MultiTimerInstall(PlatformTicksGetFunc);

    MultiTimerStart(&timer1, 1000, exampleTimer1Callback, "1000ms CYCLE timer");
    MultiTimerStart(&timer2, 5000, exampleTimer2Callback, "5000ms ONCE timer");
    MultiTimerStart(&timer3, 3456, exampleTimer3Callback, "3456ms delay start, 4567ms CYCLE timer");

    while (1) {
        MultiTimerYield();
    }
}
```

