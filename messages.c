#include <stdio.h>

#include "messages.h"

void print_help( char *name, char *bind_address, int port, char *dir )
{
  printf("\n");
  printf("Usage: %s [options]\n", name);
  printf("\n");
  printf("Options:\n");
  printf("  -h  Print this help\n");
  printf("  -a  Set listening address (currently %s)\n", bind_address);
  printf("  -d  Set data directory    (currently %s)\n", dir);
  printf("  -p  Set listening port    (currently %d)\n", port);
  printf("\n");
}