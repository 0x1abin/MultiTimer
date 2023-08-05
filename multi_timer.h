/*
 * Copyright (c) 2016 Zibin Zheng <znbin@qq.com>
 * All rights reserved
 */

#ifndef _MULTI_TIMER_H_
#define _MULTI_TIMER_H_

#include <stdint.h>
#include <stddef.h>

/*
It means 1 tick for 1ms.
Your can configurate for your tick time such as 5ms/10ms and so on.
*/
#define CFG_TIMER_1_TICK_N_MS   1

typedef struct Timer {
    uint32_t        cur_ticks;          /* Record current timer start tick */
    uint32_t        cur_expired_time;   /* Record current timer expired time */
    uint32_t        timeout;    /* Delay (xx ms) time to start timer */
    uint32_t        repeat;     /* Timer interval expired time (xx ms) */
    void *          arg;        /* Input argument for timeout_cb function */
    void            (*timeout_cb)(void *arg); /* Timer expired callback function */
    struct Timer*   next;       /* Pointer to next timer */
} Timer;

#ifdef __cplusplus
extern "C" {
#endif

/**
  * @brief  Initializes the timer struct handle.
  * @param  handle: the timer handle struct.
  * @param  timeout_cb: timeout callback.
  * @param  timeout: delay to start the timer.
  * @param  repeat: repeat interval time.
  * @param  arg: the input argument for timeout_cb fucntion.
  * @retval None
  */
void timer_init(struct Timer* handle, void(*timeout_cb)(void *arg), \
            uint32_t timeout, uint32_t repeat, void *arg);

/**
  * @brief  Start the timer work, add the handle into work list.
  * @param  handle: target handle struct.
  * @retval 0: succeed. -1: already exist.
  */
int timer_start(struct Timer* handle);

/**
  * @brief  Stop the timer work, remove the handle off work list.
  * @param  handle: target handle struct.
  * @retval 0: succeed. -1: timer not exist.
  */
int timer_stop(struct Timer* handle);

/**
  * @brief  background ticks, timer repeat invoking interval nms.
  * @param  None.
  * @retval None
  */
void timer_ticks(void);

/**
  * @brief  main loop.
  * @param  None.
  * @retval None
  */
void timer_loop(void);

#ifdef __cplusplus
}
#endif

#endif
