#include <stdio.h>

#include "driver.h"

void print_usage(char *programName)
{
  printf("\n");
  printf("Usage: %s [options]\n", programName);
  printf("\n");
  printf("Options:\n");
  printf("  -a  Storage driver argument\n");
  printf("  -d  Select storage driver (default: filesystem)\n");
  printf("  -p  Set listening port    (default: 8080)\n");
  printf("\n");
  printf("Drivers:\n");
  drivers_list();
  printf("\n");
}