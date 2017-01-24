#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "messages.h"

int main(int argc, char **argv)
{

  unsigned char flags = 0;
  char *driver = "filesystem";
  int opt, port = 8080;
  
  while((opt=getopt(argc, argv, "hp:d:"))!=-1) {
    switch(opt) {
      case 'd':
        driver = optarg;
        break;
      case 'h':
        print_usage(*argv);
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
