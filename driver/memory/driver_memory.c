#include <stdio.h>
#include <stdlib.h>

#include "driver.h"

void driver_memory_info()
{
  printf("\n");
  printf("Driver: memory\n");
  printf("\n");
  printf("  The passed argument is a threshold in bytes for garbage collector\n");
  printf("    to start. The garbage periodically checks the size of stored data\n");
  printf("    and removes the oldest objects from the storage. By default, this\n");
  printf("    limit is set to 64M\n");
  printf("\n");
  printf("  Keep in mind that this storage driver is not persistent. ALL data\n");
  printf("    be lost upon program shutdown.\n");
  printf("\n");
  printf("  Besides numbers, some multiplier characters can be used to define\n");
  printf("    the storage size:\n");
  printf("\n");
  printf("      Char  Value       Readable value\n");
  printf("      K     1024        1 KiB\n");
  printf("      M     1048576     1 MiB\n");
  printf("      G     1073741824  1 GiB\n");
  printf("\n");
}

// Register the driver to main
void driver_memory_register (void) __attribute__ ((constructor));
void driver_memory_register(void) {
    Driver *driver = malloc(sizeof(Driver));
    driver->name = "memory";
    driver->info = &driver_memory_info;
    driver_register(driver);
}