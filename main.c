#include <stdio.h>

#include "defines.h"
#include "events.h"

void println( char *string ) {
  printf("%s\n", string);
}

int main( int argc, char **argv )
{
  // Register the event
  event_on("println", &println);
  
  // Fire the event
  event_fire("println", "Hello World");
  
  return 0;
}