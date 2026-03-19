#include "event.h"
#include <stddef.h>

static uint32_t g_currentTime = 0;

void EventSchedulerInit(void) {
    g_currentTime = 0;
}

uint32_t EventGetCurrentTime(void) {
    return g_currentTime;
}

void EventSchedule(Event *event, uint32_t time) {
    if (event != NULL) {
        event->time = time;
    }
}

bool EventInitialized(Event *event) {
    return (event != NULL && event->callback != NULL);
}

void EventTick(void) {
    g_currentTime++;
}
