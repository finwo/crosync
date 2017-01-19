#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#include "defines.h"
#include "driver.h"
#include "driver_usage.h"
#include "events.h"
#include "messages.h"

int main( int argc, char **argv )
{
  
  int opt, port = 80;
  char *address = "127.0.0.1";
  char *driver  = "filesystem";
  
  // Parse command line arguments
  while((opt=getopt(argc, argv, "hp:a:d:i"))!=-1) {
    switch(opt) {
      case 'a':
        address = optarg;
        break;
      case 'd':
        driver = optarg;
        if(!driver_select(driver)) {
          printf("Chosen driver does not exist!\n");
          exit(1);
        }
        break;
      case 'i':
        (*driver_info)();
        exit(0);
        break;
      case 'p':
        port = atoi(optarg);
        break;
      case 'h':
      default:
        print_help(*argv, address, port, driver);
        exit(0);
    }
  }
  
  printf("Address: %s\n", address);
  printf("Port:    %d\n", port);
  printf("Driver:  %s\n", driver);
  
  return 0;
}