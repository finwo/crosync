#include <string.h>

#include "event.h"

// Pointer to the first registered event
Event * event_named_first = 0;

Event * event_named_last()
{
  if(!event_named_first) return 0;
  Event * event = event_named_first;
  while(event->next) {
    event = event->next;
  }
  return event;
}

// Register an event
Event * event_on(char *name, void *function, void *udata)
{
  // Create the new event
  Event *event = calloc(1, sizeof(Event));
  event->name  = name;
  event->func  = function;
  event->udata = udata;
  
  // Register it in the list
  if (event_named_first) {
    Event *lastEvent = event_named_last();
    lastEvent->next = event;
    event->previous = lastEvent;
  } else {
    event_named_first = event;
  }
  
  return event;
}

void event_trigger(char *name, void *edata)
{
  if(!event_named_first) return;
  Event * event = 0;
  do {
    if (event) {
      event->next;
    } else {
      event = event_named_first;
    }
    if(strcmp(event->name, name)==0) {
      (*event->func)(event->udata, edata);
    }
  } while(event->next);
}