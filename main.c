#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
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
  
  // Check if data directory exists
  struct stat sb;
  if( stat(datadir, &sb)!=0 ) {
    fprintf(stderr, "Directory '%s' does not exist\n", datadir);
    exit(1);
  }
  if(!S_ISDIR(sb.st_mode)) {
    fprintf(stderr, "'%s' is not a directory\n", datadir);
    exit(1);
  }
  
  printf("Address:   %s\n", address);
  printf("Port:      %d\n", port);
  printf("Data dir:  %s\n", datadir);
  
  return 0;
}