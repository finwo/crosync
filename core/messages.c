#include <stdio.h>

#include "driver.h"
#include "messages.h"

void print_help( char *name, char *bind_address, int port, char *driver )
{
  printf("\n");
  printf("Usage: %s [options]\n", name);
  printf("\n");
  printf("Options:\n");
  printf("  -h  Print this help\n");
  printf("  -a  Set listening address      (currently %s)\n", bind_address);
  printf("  -d  Set storage driver         (currently %s)\n", driver);
  printf("  -i  Print selected driver info\n");
  printf("  -p  Set listening port         (currently %d)\n", port);
  printf("\n");
  printf("Drivers:\n");
  driver_list();
  printf("\n");
}