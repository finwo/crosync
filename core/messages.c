#include <stdio.h>

#include "driver.h"
#include "messages.h"

void print_help( char *name, char *bind_address, int port )
{
  printf("\n");
  printf("Usage: %s [options]\n", name);
  printf("\n");
  printf("Common options:\n");
  printf("  -h  Print this help\n");
  printf("  -l  Set listening address (currently %s)\n", bind_address);
  printf("  -p  Set listening port    (currently %d)\n", port);
  printf("\n");
  printf("Storage driver options:\n");
  printf("  -a  Pass driver argument\n");
  printf("  -d  Set storage driver\n");
  printf("  -i  Print selected driver info\n");
  printf("\n");
  printf("Drivers:\n");
  driver_list();
  printf("\n");
}