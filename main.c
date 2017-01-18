#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "defines.h"
#include "events.h"
#include "messages.h"

int main( int argc, char **argv )
{
  
  int opt, port = 80;
  char *address = "127.0.0.1";
  char *datadir = "/srv/crud/data";
  
  // Parse command line arguments
  while((opt=getopt(argc, argv, "hp:a:d:"))!=-1) {
    switch(opt) {
      case 'a':
        address = optarg;
        break;
      case 'd':
        datadir = optarg;
        break;
      case 'p':
        port = atoi(optarg);
        break;
      case 'h':
      default:
        print_help(*argv, address, port, datadir);
        exit(0);
    }
  }
  
  printf("Address:   %s\n", address);
  printf("Port:      %d\n", port);
  printf("Data dir:  %s\n", datadir);
  
  return 0;
}