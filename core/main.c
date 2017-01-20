#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#include "defines.h"
#include "driver.h"
#include "events.h"
#include "messages.h"

int main( int argc, char **argv )
{
  
  int opt;
  char *bind_address = "tcp://127.0.0.1:80";
  char *driver = "filesystem", *driver_arg;
  
  unsigned char flags = 0;
  
  // Parse command line arguments
  while((opt=getopt(argc, argv, "a:d:hil:"))!=-1) {
    switch(opt) {
      case 'a':
        driver_arg = optarg;
        break;
      case 'd':
        driver = optarg;
        break;
      case 'i':
        flags |= 1;
        break;
      case 'l':
        bind_address = optarg;
        break;
      
        
      case 'h':
      default:
        print_help(*argv, bind_address);
        exit(0);
    }
  }
  
  // Let's see if the driver exists
  if(!driver_select(driver)) {
    fprintf(stderr, "Selected driver '%s' does not exist\n", driver);
    exit(1);
  }
  
  // Print driver info if requested
  if(flags&1) {
    driver_info();
    exit(0);
  }
  
  printf("Address: %s\n", bind_address);
  printf("Driver:  %s\n", driver);
  
  return 0;
}
