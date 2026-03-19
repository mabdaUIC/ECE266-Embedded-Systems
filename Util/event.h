#ifndef EVENT_H_
#define EVENT_H_

#include <stdint.h>
#include <stdbool.h>

// Define the Event type that launchpad.h is missing
typedef uint32_t uint32;
typedef void (*EventCallback)(void *);

typedef struct _Event {
    EventCallback callback;
    void *arg;
    uint32_t time;
    struct _Event *next;
} Event;

// Function prototypes used by pushbutton.c and launchpad.c
void EventSchedulerInit(void);
void EventSchedule(Event *event, uint32_t time);
uint32_t EventGetCurrentTime(void);
bool EventInitialized(Event *event);
void EventTick(void);

#endif /* EVENT_H_ */
