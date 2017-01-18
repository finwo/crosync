#include <stdlib.h>
#include <string.h>

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

// Tell the compiler how to call events
void (*functionPointer)(void *data);

// A list and it's size
Listener *listeners;
int listeners_count = 0;

// We'll use this more than once
int pointerSize = sizeof(Listener *);

// Register an event
void event_on( char *eventName, void *function )
{
  // Create a new listener
  Listener listener = { eventName, function };
  listeners_count++;
  
  char *newListLocation;
  
  // Make sure we have the memory
  if ( listeners ) {
    newListLocation = realloc( listeners, pointerSize * listeners_count );
    if ( newListLocation ) {
      listeners = (void *)newListLocation;
    } else {
      exit(1);
    }
  } else {
    listeners = malloc( pointerSize * listeners_count );
  }
  
  // 'Register' the listener
  *(listeners+((listeners_count-1)*pointerSize)) = listener;
}

// Trigger an event
void event_fire( char *eventName, void *data )
{
  int i;
  Listener *listener;
  
  // Loop through listeners
  for( i = 0 ; i < listeners_count ; i++ ) {
    listener = listeners + ( pointerSize * i );
    
    // Skip if the event name does not match
    if ( strcmp(eventName, listener->eventName) ) continue;
    
    // Call the listener
    functionPointer = listener->function;
    (*functionPointer)(data);
  }
}