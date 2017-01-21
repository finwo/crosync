#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "events.h"

// These have no purpose outside this file
typedef struct {
  char *name;
  void *data;
} Event;

typedef struct {
  char *eventName;
  void *function;
} Listener;

typedef struct {
  void *resource;
  void *function;
  void *extra_arg;
} IO_Listener;

// Tell the compiler how to call events
void (*functionPointer)(void *data);

// Lists and their size
Listener *namedListenerList = 0;
int namedListenerCount = 0;
IO_Listener *ioListenerList = 0;
int ioListenerCount = 0;

// We'll use this more than once
int eventPointerSize = sizeof(Listener);
int ioPointerSize    = sizeof(IO_Listener);

// Will sleep for [sleeptime] microseconds if no event was fired
void ev_poll( unsigned int sleeptime )
{
  int eventFired = 0;
  
  if(!eventFired) {
    usleep(sleeptime);
  }
}

// Fire a function once the 'select'-able resource is ready
// This will only fire once
void ev_io( void *resource, void *function, void *extra_arg ) {
  
}

// Register an event
void ev_on( char *eventName, void *function )
{
  // Create a new listener
  Listener listener = { eventName, function };
  namedListenerCount++;
  
  char *newListLocation;
  
  // Make sure we have the memory
  if ( namedListenerList ) {
    newListLocation = realloc( namedListenerList, eventPointerSize * namedListenerCount );
    if ( newListLocation ) {
      namedListenerList = (void *)newListLocation;
    } else {
      exit(1);
    }
  } else {
    namedListenerList = malloc( eventPointerSize * namedListenerCount );
  }
  
  // 'Register' the listener
  *(namedListenerList+((namedListenerCount-1)*eventPointerSize)) = listener;
}

// Trigger an event
// Triggers instantly. This is BLOCKING
void ev_fire( char *eventName, void *data )
{
  int i;
  Listener *listener;
  
  // Loop through listeners
  for( i = 0 ; i < namedListenerCount ; i++ ) {
    listener = namedListenerList + ( eventPointerSize * i );
    
    // Skip if the event name does not match
    if ( strcmp(eventName, listener->eventName) ) continue;
    
    // Call the listener
    functionPointer = listener->function;
    (*functionPointer)(data);
  }
}