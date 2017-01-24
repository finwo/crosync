#include <stdio.h>
#include <stdlib.h>

#include "driver.h"

void driver_filesystem_info()
{
  printf("\n");
  printf("Driver: filesystem\n");
  printf("\n");
  printf("  The passed argument should contain a path to the directory which will\n");
  printf("    hold all data. If the directory does not exist yet, it will be\n");
  printf("    created.\n");
  printf("\n");
}

// Register the driver to main
void driver_filesystem_register (void) __attribute__ ((constructor));
void driver_filesystem_register(void) {
    Driver *driver = malloc(sizeof(Driver));
    driver->name = "filesystem";
    driver->info = &driver_filesystem_info;
    driver_register(driver);
}