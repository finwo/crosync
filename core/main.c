#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "http.h"
#include "messages.h"

int main(int argc, char **argv)
{
  // Pre-define some vars
  int opt, port = 8080;
  dyad_Stream *serv;
  driver_select("filesystem");
  int running = 1;
  
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
  
  http_start(port);
  
  while(running) {
    running &= http_poll();
  }
  
  return 0;
}
