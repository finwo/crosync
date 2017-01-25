#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "messages.h"

int main(int argc, char **argv)
{
  // Pre-define some vars
  int opt, port = 8080;
  driver_select("filesystem");
  
  // Parse command-line arguments
  while((opt=getopt(argc, argv, "d:hip:"))!=-1) {
    switch(opt) {
      case 'd':
        if(!driver_select(optarg)) {
          fprintf(stderr, "Driver '%s' does not exist\n", optarg);
          exit(2);
        }
        break;
      case 'h':
        print_usage(*argv);
        exit(0);
      case 'i':
        driver_info();
        exit(0);
      case 'p':
        port = atoi(optarg);
        break;
      default:
        print_usage(*argv);
        exit(1);
    }
  }
  
  return 0;
}
