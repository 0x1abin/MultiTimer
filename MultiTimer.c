#include "MultiTimer.h"
#include <stdio.h>

// Timer handle list head.
static MultiTimer* timerList = NULL;

// Timer tick
static PlatformTicksFunction_t platformTicksFunction = NULL;

/**
 * @brief 
 * 
 * @param ticksFunc 
 * @return int 
 */
int MultiTimerInstall(PlatformTicksFunction_t ticksFunc)
{
    platformTicksFunction = ticksFunc;
    return 0;
}

/**
  * @brief  Initializes the timer struct handle.
  * @param  handle: the timer handle strcut.
  * @param  timeout_cb: deadline callback.
  * @param  repeat: repeat interval time.
  * @retval None
  */
int MultiTimerInit(MultiTimer* timer, uint32_t period, MultiTimerCallback_t cb, void* userData)
{
    if (!timer || !cb) {
        return -1;
    }
    timer->callback = cb;
    timer->userData = userData;
    timer->period = period;
    return 0;
}

/**
  * @brief  Start the timer work, add the handle into work list.
  * @param  handle: target handle strcut.
  * @param  deadline: Set the start time.
  * @retval 0: succeed. -1: already exist.
  */
int MultiTimerStart(MultiTimer* timer, uint32_t startTime)
{
    MultiTimer** nextTimer = &timerList;

    /* Remove the existing target timer. */
    for (; *nextTimer; nextTimer = &(*nextTimer)->next) {
        if (timer == *nextTimer) {
            *nextTimer = timer->next; /* remove from list */
            break;
        }
    }

    /* New deadline time. */
    timer->deadline = platformTicksFunction() + startTime;

    /* Insert timer. */
    for (nextTimer = &timerList;; nextTimer = &(*nextTimer)->next) {
        if (!*nextTimer) {
            timer->next = NULL;
            *nextTimer = timer;
            break;
        }
        if (timer->deadline < (*nextTimer)->deadline) {
            timer->next = *nextTimer;
            *nextTimer = timer;
            break;
        }
    }
    return 0;
}

/**
  * @brief  Stop the timer work, remove the handle off work list.
  * @param  handle: target handle strcut.
  * @retval None
  */
int MultiTimerStop(MultiTimer* timer)
{
    MultiTimer** nextTimer = &timerList;

    /* Find and remove timer. */
    for (; *nextTimer; nextTimer = &(*nextTimer)->next) {
        MultiTimer* entry = *nextTimer;
        if (entry == timer) {
            *nextTimer = timer->next;
            break;
        }
    }
    return 0;
}

/**
  * @brief  main loop.
  * @param  None.
  * @retval None
  */
void MultiTimerYield(void)
{
    MultiTimer* target = timerList;
    for (; target; target = target->next) {
        if (target->deadline > platformTicksFunction()) {
            return;
        }
        MultiTimerStop(target);
        if (target->period) {
            MultiTimerStart(target, target->period);
        }
        if (target->callback) {
            target->callback(target, target->userData);
        }
    }
}
